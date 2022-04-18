if (EOSIO_COMPILE_TEST_CONTRACTS)
   add_contract(led.boot led.boot ${CMAKE_CURRENT_SOURCE_DIR}/src/led.boot.cpp)

   target_include_directories(led.boot
      PUBLIC
      ${CMAKE_CURRENT_SOURCE_DIR}/include)

   set_target_properties(led.boot
      PROPERTIES
      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")

   configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/ricardian/led.boot.contracts.md.in ${CMAKE_CURRENT_BINARY_DIR}/ricardian/led.boot.contracts.md @ONLY )

   target_compile_options( led.boot PUBLIC -R${CMAKE_CURRENT_SOURCE_DIR}/ricardian -R${CMAKE_CURRENT_BINARY_DIR}/ricardian )
else()
   configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/bin/led.boot.abi ${CMAKE_CURRENT_BINARY_DIR}/ COPYONLY )
   configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/bin/led.boot.wasm ${CMAKE_CURRENT_BINARY_DIR}/ COPYONLY )
endif()

