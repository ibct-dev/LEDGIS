# will be implictly used for any compilation unit if not overrided by SYSTEM_INCLUDE_FOLDERS parameter
# these directories go as -isystem <dir> to avoid warnings from code of third-party libraries

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/led.bios/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/led.bios/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/led.msig/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/led.msig/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/led.system/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/led.system/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/led.token/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/led.token/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/led.wrap/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/led.wrap/)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/old_versions/v1.6.0-rc3/led.bios/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/old_versions/v1.6.0-rc3/led.bios/)
