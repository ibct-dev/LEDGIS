file(GLOB HEADERS "include/eosio/tps_test_gen_plugin/*.hpp")
add_library( tps_test_gen_plugin
             tps_test_gen_plugin.cpp
             ${HEADERS} )
             
target_link_libraries( tps_test_gen_plugin appbase fc http_plugin chain_plugin eosio_testing )
target_include_directories( tps_test_gen_plugin PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
                            tps_test_gen_plugin PUBLIC ${CMAKE_SOURCE_DIR}/libraries/testing/include
                            tps_test_gen_plugin PUBLIC ${CMAKE_BINARY_DIR}/unittests/include )
