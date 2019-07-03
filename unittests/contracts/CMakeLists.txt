# will be implictly used for any compilation unit if not overrided by SYSTEM_INCLUDE_FOLDERS parameter
# these directories go as -isystem <dir> to avoid warnings from code of third-party libraries

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/ecrio.bios/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/ecrio.bios/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/ecrio.msig/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/ecrio.msig/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/ecrio.system/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/ecrio.system/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/ecrio.token/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/ecrio.token/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/ecrio.wrap/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/ecrio.wrap/)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/old_versions/v1.6.0-rc3/ecrio.bios/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/old_versions/v1.6.0-rc3/ecrio.bios/)
