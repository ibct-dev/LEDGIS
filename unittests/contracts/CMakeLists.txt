# will be implictly used for any compilation unit if not overrided by SYSTEM_INCLUDE_FOLDERS parameter
# these directories go as -isystem <dir> to avoid warnings from code of third-party libraries

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/legis.bios/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/legis.bios/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/legis.msig/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/legis.msig/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/legis.system/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/legis.system/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/legis.token/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/legis.token/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/legis.wrap/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/legis.wrap/)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/old_versions/v1.6.0-rc3/legis.bios/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/old_versions/v1.6.0-rc3/legis.bios/)
