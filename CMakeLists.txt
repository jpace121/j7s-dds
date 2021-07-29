cmake_minimum_required(VERSION 3.5)

# https://github.com/Sarcasm/cmake-superbuild/
option (USE_SUPERBUILD "Whether or not a superbuild should be invoked" ON)

if (USE_SUPERBUILD)
  project(j7s-dds-superbuild NONE)
  include(external-deps.cmake)
  return()
else()
  project(j7s-dds)
endif()

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-Wall -Wextra -Wpedantic)

find_package(CycloneDDS-CXX REQUIRED)
find_package(blinkt_interface REQUIRED)
find_package(argparse REQUIRED)

idlcxx_generate(TARGET Msgs_lib FILES "./idl/LedState.idl")

add_library(j7s-dds-utils SHARED src/utils.cpp)
target_include_directories(j7s-dds-utils PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
target_link_libraries(j7s-dds-utils)

add_executable(j7s-sub src/j7s-sub.cpp)
set_property(TARGET j7s-sub PROPERTY CXX_STANDARD 17)
target_include_directories(j7s-sub PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
target_link_libraries(j7s-sub
  PRIVATE Msgs_lib CycloneDDS-CXX::ddscxx blinkt_interface::blinkt argparse::argparse)

add_executable(j7s-pub src/j7s-pub.cpp)
target_link_libraries(j7s-pub CycloneDDS-CXX::ddscxx Msgs_lib argparse::argparse j7s-dds-utils)
