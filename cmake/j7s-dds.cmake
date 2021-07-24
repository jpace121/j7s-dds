include (ExternalProject)

set (DEPENDENCIES)
set (EXTRA_CMAKE_ARGS)

list(APPEND DEPENDENCIES ep_blinkt)
ExternalProject_Add(ep_blinkt
  PREFIX ep_blinkt
  SOURCE_DIR "${PROJECT_SOURCE_DIR}/ext/j7s-blinkt/blinkt_interface/"
  INSTALL_COMMAND ""
  BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/blinkt
)
list (APPEND EXTRA_CMAKE_ARGS
  -Dblinkt_interface_DIR=${CMAKE_CURRENT_BINARY_DIR}/blinkt/CMakeFiles
)

ExternalProject_Add (ep_j7s_dds
  PREFIX ep_j7s_dds
  DEPENDS ${DEPENDENCIES}
  SOURCE_DIR "${PROJECT_SOURCE_DIR}"
  CMAKE_ARGS -DUSE_SUPERBUILD=OFF ${EXTRA_CMAKE_ARGS}
  INSTALL_COMMAND ""
  BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})
