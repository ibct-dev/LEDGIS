/**
 *  @file
 *  @copyright defined in eos/LICENSE
 */
#include <eosio/tps_test_gen_plugin/tps_test_gen_plugin.hpp>
#include <eosio/chain_plugin/chain_plugin.hpp>
#include <eosio/chain/wast_to_wasm.hpp>

#include <fc/variant.hpp>
#include <fc/io/json.hpp>
#include <fc/exception/exception.hpp>
#include <fc/reflect/variant.hpp>
#include <fc/io/json.hpp>

#include <boost/asio/high_resolution_timer.hpp>
#include <boost/algorithm/clamp.hpp>

#include <Inline/BasicTypes.h>
#include <IR/Module.h>
#include <IR/Validate.h>
#include <WAST/WAST.h>
#include <WASM/WASM.h>
#include <Runtime/Runtime.h>

#include <contracts.hpp>

using namespace eosio::testing;

namespace eosio
{
namespace detail
{
struct tps_test_gen_empty
{
};
struct tps_test_gen_status
{
   string status;
};
} // namespace detail
} // namespace eosio

FC_REFLECT(eosio::detail::tps_test_gen_empty, );
FC_REFLECT(eosio::detail::tps_test_gen_status, (status));

namespace eosio
{

static appbase::abstract_plugin &_tps_test_gen_plugin = app().register_plugin<tps_test_gen_plugin>();

using namespace eosio::chain;
using io_work_t = boost::asio::executor_work_guard<boost::asio::io_context::executor_type>;

#define CALL(api_name, api_handle, call_name, INVOKE, http_response_code)       \
   {                                                                            \
      std::string("/v1/" #api_name "/" #call_name),                             \
          [this](string, string body, url_response_callback cb) mutable {       \
             try                                                                \
             {                                                                  \
                if (body.empty())                                               \
                   body = "{}";                                                 \
                INVOKE                                                          \
                cb(http_response_code, fc::variant(result));                    \
             }                                                                  \
             catch (...)                                                        \
             {                                                                  \
                http_plugin::handle_exception(#api_name, #call_name, body, cb); \
             }                                                                  \
          }                                                                     \
   }

#define INVOKE_V_R_R_R(api_handle, call_name, in_param0, in_param1, in_param2)                                        \
   const auto &vs = fc::json::json::from_string(body).as<fc::variants>();                                             \
   auto status = api_handle->call_name(vs.at(0).as<in_param0>(), vs.at(1).as<in_param1>(), vs.at(2).as<in_param2>()); \
   eosio::detail::tps_test_gen_status result = {status};

#define INVOKE_V_R_R(api_handle, call_name, in_param0, in_param1)             \
   const auto &vs = fc::json::json::from_string(body).as<fc::variants>();     \
   api_handle->call_name(vs.at(0).as<in_param0>(), vs.at(1).as<in_param1>()); \
   eosio::detail::tps_test_gen_empty result;

#define INVOKE_V_V(api_handle, call_name) \
   api_handle->call_name();               \
   eosio::detail::tps_test_gen_empty result;

#define CALL_ASYNC(api_name, api_handle, call_name, INVOKE, http_response_code)                                                    \
   {                                                                                                                               \
      std::string("/v1/" #api_name "/" #call_name),                                                                                \
          [this](string, string body, url_response_callback cb) mutable {                                                          \
             if (body.empty())                                                                                                     \
                body = "{}";                                                                                                       \
             /*plugin processes many transactions, report only first to avoid http_plugin having to deal with multiple responses*/ \
             auto times_called = std::make_shared<std::atomic<size_t>>(0);                                                         \
             auto result_handler = [times_called{std::move(times_called)}, cb, body](const fc::exception_ptr &e) mutable {         \
                if (++(*times_called) > 1)                                                                                         \
                   return;                                                                                                         \
                if (e)                                                                                                             \
                {                                                                                                                  \
                   try                                                                                                             \
                   {                                                                                                               \
                      e->dynamic_rethrow_exception();                                                                              \
                   }                                                                                                               \
                   catch (...)                                                                                                     \
                   {                                                                                                               \
                      http_plugin::handle_exception(#api_name, #call_name, body, cb);                                              \
                   }                                                                                                               \
                }                                                                                                                  \
                else                                                                                                               \
                {                                                                                                                  \
                   cb(http_response_code, fc::variant(eosio::detail::tps_test_gen_empty()));                                       \
                }                                                                                                                  \
             };                                                                                                                    \
             INVOKE                                                                                                                \
          }                                                                                                                        \
   }

#define INVOKE_ASYNC_R_R(api_handle, call_name, in_param0, in_param1)     \
   const auto &vs = fc::json::json::from_string(body).as<fc::variants>(); \
   api_handle->call_name(vs.at(0).as<in_param0>(), vs.at(1).as<in_param1>(), result_handler);

struct tps_test_gen_plugin_impl
{

   uint64_t _total_us = 0;
   uint64_t _txcount = 0;

   std::shared_ptr<boost::asio::io_context> gen_ioc;
   optional<io_work_t> gen_ioc_work;
   uint16_t thread_pool_size;
   optional<boost::asio::thread_pool> thread_pool;
   std::shared_ptr<boost::asio::high_resolution_timer> timer;
   name newaccountT;

   void push_next_transaction(const std::shared_ptr<std::vector<signed_transaction>> &trxs, const std::function<void(const fc::exception_ptr &)> &next)
   {
      chain_plugin &cp = app().get_plugin<chain_plugin>();

      for (size_t i = 0; i < trxs->size(); ++i)
      {
         cp.accept_transaction(packed_transaction(trxs->at(i)), [=](const fc::static_variant<fc::exception_ptr, transaction_trace_ptr> &result) {
            if (result.contains<fc::exception_ptr>())
            {
               next(result.get<fc::exception_ptr>());
            }
            else
            {
               if (result.contains<transaction_trace_ptr>() && result.get<transaction_trace_ptr>()->receipt)
               {
                  _total_us += result.get<transaction_trace_ptr>()->receipt->cpu_usage_us;
                  ++_txcount;
               }
            }
         });
      }
   }

   void push_transactions(std::vector<signed_transaction> &&trxs, const std::function<void(fc::exception_ptr)> &next)
   {
      auto trxs_copy = std::make_shared<std::decay_t<decltype(trxs)>>(std::move(trxs));
      app().post(priority::low, [this, trxs_copy, next]() {
         push_next_transaction(trxs_copy, next);
      });
   }

   void create_test_accounts(const std::string &init_name, const std::string &init_priv_key, const std::function<void(const fc::exception_ptr &)> &next)
   {
      ilog("create_test_accounts");
      std::vector<signed_transaction> trxs;
      trxs.reserve(2);

      try
      {
         name creator(init_name);

         abi_def currency_abi_def = fc::json::from_string(contracts::led_system_abi().data()).as<abi_def>();

         controller &cc = app().get_plugin<chain_plugin>().chain();
         auto chainid = app().get_plugin<chain_plugin>().get_chain_id();
         auto abi_serializer_max_time = app().get_plugin<chain_plugin>().get_abi_serializer_max_time();

         abi_serializer led_system_serializer{fc::json::from_string(contracts::led_system_abi().data()).as<abi_def>(), abi_serializer_max_time};

         fc::crypto::private_key tps_test_receiver_A_priv_key = fc::crypto::private_key::regenerate(fc::sha256(std::string(64, 'a')));
         fc::crypto::public_key tps_text_receiver_A_pub_key = tps_test_receiver_A_priv_key.get_public_key();
         fc::crypto::private_key creator_priv_key = fc::crypto::private_key(init_priv_key);

         //create some test accounts
         {
            signed_transaction trx;

            //create "T" account
            {
               auto owner_auth = eosio::chain::authority{1, {{tps_text_receiver_A_pub_key, 1}}, {}};
               auto active_auth = eosio::chain::authority{1, {{tps_text_receiver_A_pub_key, 1}}, {}};

               trx.actions.emplace_back(vector<chain::permission_level>{{creator, "active"}}, newaccount{creator, newaccountT, owner_auth, active_auth});
            }

            trx.expiration = cc.head_block_time() + fc::seconds(180);
            trx.set_reference_block(cc.head_block_id());
            trx.sign(creator_priv_key, chainid);
            trxs.emplace_back(std::move(trx));
         }

         //set newaccountT contract to led.token & initialize it
         {
            signed_transaction trx;

            vector<uint8_t> wasm = contracts::led_system_wasm();

            setcode handler;
            handler.account = newaccountT;
            handler.code.assign(wasm.begin(), wasm.end());

            trx.actions.emplace_back(vector<chain::permission_level>{{newaccountT, "active"}}, handler);

            {
               setabi handler;
               handler.account = newaccountT;
               handler.abi = fc::raw::pack(json::from_string(contracts::led_system_abi().data()).as<abi_def>());
               trx.actions.emplace_back(vector<chain::permission_level>{{newaccountT, "active"}}, handler);
            }

            trx.expiration = cc.head_block_time() + fc::seconds(180);
            trx.set_reference_block(cc.head_block_id());
            trx.max_net_usage_words = 5000;
            trx.sign(tps_test_receiver_A_priv_key, chainid);
            trxs.emplace_back(std::move(trx));
         }
      }
      catch (const fc::exception &e)
      {
         next(e.dynamic_copy_exception());
         return;
      }

      push_transactions(std::move(trxs), next);
   }

   string start_generation(const std::string &salt, const uint64_t &period, const uint64_t &batch_size)
   {
      ilog("Starting transaction test plugin");
      if (running)
         return "start_generation already running";
      if (period < 1 || period > 2500)
         return "period must be between 1 and 2500";
      if (batch_size < 1 || batch_size > 250)
         return "batch_size must be between 1 and 250";
      if (batch_size & 1)
         return "batch_size must be even";
      ilog("Starting transaction test plugin valid");

      running = true;

      fc::crypto::private_key hashed_data_A = fc::crypto::private_key::regenerate(fc::sha256(std::string(64, 'a')));
      fc::crypto::private_key hashed_data_B = fc::crypto::private_key::regenerate(fc::sha256(std::string(64, 'b')));

      controller &cc = app().get_plugin<chain_plugin>().chain();
      auto abi_serializer_max_time = app().get_plugin<chain_plugin>().get_abi_serializer_max_time();
      abi_serializer led_system_serializer{fc::json::from_string(contracts::led_system_abi().data()).as<abi_def>(), abi_serializer_max_time};

      //create the actions here
      act_a.account = newaccountT;
      act_a.name = N(submitdapa);
      act_a.authorization = vector<permission_level>{{newaccountT, config::active_name}};
      act_a.data = led_system_serializer.variant_to_binary("submitdapa",
                                                           fc::json::from_string(fc::format_string("{\"proposal_business\":\"testa\",\"hashed_data\":\"${hashed_data}\",\"state\":\"${l}\"}",
                                                                                                   fc::mutable_variant_object()("hashed_data", hashed_data_A)("l",salt))),
                                                           abi_serializer_max_time);

      act_b.account = newaccountT;
      act_b.name = N(submitdapa);
      act_b.authorization = vector<permission_level>{{newaccountT, config::active_name}};
      act_b.data = led_system_serializer.variant_to_binary("submitdapa",
                                                           fc::json::from_string(fc::format_string("{\"proposal_business\":\"testb\",\"hashed_data\":\"${hashed_data}\",\"state\":\"${l}\"}",
                                                                                                   fc::mutable_variant_object()("hashed_data", hashed_data_B)("l",salt))),
                                                           abi_serializer_max_time);

      timer_timeout = period;
      batch = batch_size / 2;
      nonce_prefix = 0;

      gen_ioc = std::make_shared<boost::asio::io_context>();
      gen_ioc_work.emplace(boost::asio::make_work_guard(*gen_ioc));
      thread_pool.emplace(thread_pool_size);
      for (uint16_t i = 0; i < thread_pool_size; i++)
         boost::asio::post(*thread_pool, [ioc = gen_ioc]() { ioc->run(); });
      timer = std::make_shared<boost::asio::high_resolution_timer>(*gen_ioc);

      ilog("Started transaction test plugin; generating ${p} transactions every ${m} ms by ${t} load generation threads",
           ("p", batch_size)("m", period)("t", thread_pool_size));

      boost::asio::post(*gen_ioc, [this]() {
         arm_timer(boost::asio::high_resolution_timer::clock_type::now());
      });
      return "success";
   }

   void arm_timer(boost::asio::high_resolution_timer::time_point s)
   {
      timer->expires_at(s + std::chrono::milliseconds(timer_timeout));
      boost::asio::post(*gen_ioc, [this]() {
         send_transaction([this](const fc::exception_ptr &e) {
            if (e)
            {
               elog("pushing transaction failed: ${e}", ("e", e->to_detail_string()));
               if (running)
                  stop_generation();
            }
         },
                          nonce_prefix++);
      });
      timer->async_wait([this](const boost::system::error_code &ec) {
         if (!running || ec)
            return;
         arm_timer(timer->expires_at());
      });
   }

   void send_transaction(std::function<void(const fc::exception_ptr &)> next, uint64_t nonce_prefix)
   {
      std::vector<signed_transaction> trxs;
      trxs.reserve(2 * batch);

      try
      {
         controller &cc = app().get_plugin<chain_plugin>().chain();
         auto chainid = app().get_plugin<chain_plugin>().get_chain_id();

         static fc::crypto::private_key a_priv_key = fc::crypto::private_key::regenerate(fc::sha256(std::string(64, 'a')));

         static uint64_t nonce = static_cast<uint64_t>(fc::time_point::now().sec_since_epoch()) << 32;

         uint32_t reference_block_num = cc.last_irreversible_block_num();
         if (tps_reference_block_lag >= 0)
         {
            reference_block_num = cc.head_block_num();
            if (reference_block_num <= (uint32_t)tps_reference_block_lag)
            {
               reference_block_num = 0;
            }
            else
            {
               reference_block_num -= (uint32_t)tps_reference_block_lag;
            }
         }

         block_id_type reference_block_id = cc.get_block_id_for_num(reference_block_num);

         for (unsigned int i = 0; i < batch; ++i)
         {
            {
               signed_transaction trx;
               trx.actions.push_back(act_a);
               trx.context_free_actions.emplace_back(action({}, config::null_account_name, "nonce", fc::raw::pack(std::to_string(nonce_prefix) + std::to_string(nonce++))));
               trx.set_reference_block(reference_block_id);
               trx.expiration = cc.head_block_time() + fc::seconds(30);
               trx.max_net_usage_words = 100;
               trx.sign(a_priv_key, chainid);
               trxs.emplace_back(std::move(trx));
            }

            {
               signed_transaction trx;
               trx.actions.push_back(act_b);
               trx.context_free_actions.emplace_back(action({}, config::null_account_name, "nonce", fc::raw::pack(std::to_string(nonce_prefix) + std::to_string(nonce++))));
               trx.set_reference_block(reference_block_id);
               trx.expiration = cc.head_block_time() + fc::seconds(30);
               trx.max_net_usage_words = 100;
               trx.sign(a_priv_key, chainid);
               trxs.emplace_back(std::move(trx));
            }
         }
      }
      catch (const fc::exception &e)
      {
         next(e.dynamic_copy_exception());
      }

      ilog("send ${c} transactions", ("c", trxs.size()));
      push_transactions(std::move(trxs), next);
   }

   void stop_generation()
   {
      if (!running)
         throw fc::exception(fc::invalid_operation_exception_code);
      timer->cancel();
      running = false;
      if (gen_ioc_work)
         gen_ioc_work->reset();
      if (gen_ioc)
         gen_ioc->stop();
      if (thread_pool)
      {
         thread_pool->join();
         thread_pool->stop();
      }
      ilog("Stopping transaction generation test");

      if (_txcount)
      {
         ilog("${d} transactions executed, ${t}us / transaction", ("d", _txcount)("t", _total_us / (double)_txcount));
         _txcount = _total_us = 0;
      }
   }

   bool running{false};

   unsigned timer_timeout;
   unsigned batch;
   uint64_t nonce_prefix;

   action act_a;
   action act_b;

   int32_t tps_reference_block_lag;
};

tps_test_gen_plugin::tps_test_gen_plugin() {}
tps_test_gen_plugin::~tps_test_gen_plugin() {}

void tps_test_gen_plugin::set_program_options(options_description &, options_description &cfg)
{
   cfg.add_options()("tps-reference-block-lag", bpo::value<int32_t>()->default_value(0), "Lag in number of blocks from the head block when selecting the reference block for transactions (-1 means Last Irreversible Block)")("tps-test-gen-threads", bpo::value<uint16_t>()->default_value(2), "Number of worker threads in tps_test_gen thread pool")("tps-test-gen-account-prefix", bpo::value<string>()->default_value("tps.test."), "Prefix to use for accounts generated and used by this plugin");
}

void tps_test_gen_plugin::plugin_initialize(const variables_map &options)
{
   try
   {
      my.reset(new tps_test_gen_plugin_impl);
      my->tps_reference_block_lag = options.at("tps-reference-block-lag").as<int32_t>();
      my->thread_pool_size = options.at("tps-test-gen-threads").as<uint16_t>();
      const std::string thread_pool_account_prefix = options.at("tps-test-gen-account-prefix").as<std::string>();
      my->newaccountT = thread_pool_account_prefix + "t";
      EOS_ASSERT(my->thread_pool_size > 0, chain::plugin_config_exception,
                 "tps-test-gen-threads ${num} must be greater than 0", ("num", my->thread_pool_size));
   }
   FC_LOG_AND_RETHROW()
}

void tps_test_gen_plugin::plugin_startup()
{
   app().get_plugin<http_plugin>().add_api({CALL_ASYNC(tps_test_gen, my, create_test_accounts, INVOKE_ASYNC_R_R(my, create_test_accounts, std::string, std::string), 200),
                                            CALL(tps_test_gen, my, stop_generation, INVOKE_V_V(my, stop_generation), 200),
                                            CALL(tps_test_gen, my, start_generation, INVOKE_V_R_R_R(my, start_generation, std::string, uint64_t, uint64_t), 200)});
}

void tps_test_gen_plugin::plugin_shutdown()
{
   try
   {
      my->stop_generation();
   }
   catch (fc::exception &e)
   {
   }
}

} // namespace eosio
