if( EOSIO_COMPILE_TEST_CONTRACTS )
   set(EOSIO_WASM_OLD_BEHAVIOR "Off")
   find_package( eosio.cdt REQUIRED )
endif()

add_subdirectory(led.bios)
add_subdirectory(led.boot)
