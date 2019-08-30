/**
 *  @file
 *  @copyright defined in eos/LICENSE
 */
#pragma once

#include <eosio/chain/types.hpp>
#include <eosio/chain/contract_types.hpp>

namespace eosio { namespace chain {

   class apply_context;

   /**
    * @defgroup native_action_handlers Native Action Handlers
    */
   ///@{
   void apply_legis_newaccount(apply_context&);
   void apply_legis_updateauth(apply_context&);
   void apply_legis_deleteauth(apply_context&);
   void apply_legis_linkauth(apply_context&);
   void apply_legis_unlinkauth(apply_context&);

   /*
   void apply_legis_postrecovery(apply_context&);
   void apply_legis_passrecovery(apply_context&);
   void apply_legis_vetorecovery(apply_context&);
   */

   void apply_legis_setcode(apply_context&);
   void apply_legis_setabi(apply_context&);

   void apply_legis_canceldelay(apply_context&);
   ///@}  end action handlers

//    /**
//     * @defgroup native_action_handlers Native Action Handlers
//     */
//    ///@{
//    void apply_eosio_newaccount(apply_context&);
//    void apply_eosio_updateauth(apply_context&);
//    void apply_eosio_deleteauth(apply_context&);
//    void apply_eosio_linkauth(apply_context&);
//    void apply_eosio_unlinkauth(apply_context&);

//    /*
//    void apply_eosio_postrecovery(apply_context&);
//    void apply_eosio_passrecovery(apply_context&);
//    void apply_eosio_vetorecovery(apply_context&);
//    */

//    void apply_eosio_setcode(apply_context&);
//    void apply_eosio_setabi(apply_context&);

//    void apply_eosio_canceldelay(apply_context&);
//    ///@}  end action handlers

} } /// namespace eosio::chain
