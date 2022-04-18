if (EOSIO_COMPILE_TEST_CONTRACTS)
   add_contract(led.bios led.bios ${CMAKE_CURRENT_SOURCE_DIR}/src/led.bios.cpp)

   target_include_directories(led.bios
      PUBLIC
      ${CMAKE_CURRENT_SOURCE_DIR}/include)

   set_target_properties(led.bios
      PROPERTIES
      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")

   configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/ricardian/led.bios.contracts.md.in ${CMAKE_CURRENT_BINARY_DIR}/ricardian/led.bios.contracts.md @ONLY )

   target_compile_options( led.bios PUBLIC -R${CMAKE_CURRENT_SOURCE_DIR}/ricardian -R${CMAKE_CURRENT_BINARY_DIR}/ricardian )
else()
   configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/bin/led.bios.abi ${CMAKE_CURRENT_BINARY_DIR}/ COPYONLY )
   configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/bin/led.bios.wasm ${CMAKE_CURRENT_BINARY_DIR}/ COPYONLY )
endif()
