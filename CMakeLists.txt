cmake_minimum_required(VERSION 3.5)

# https://github.com/Sarcasm/cmake-superbuild/
option (USE_SUPERBUILD "Whether or not a superbuild should be invoked" ON)

if (USE_SUPERBUILD)
  project(j7s-dds-superbuild NONE)
  include(cmake/j7s-dds.cmake)
  return()
else()
  project(j7s-dds)
endif()

find_package(CycloneDDS-CXX REQUIRED)
find_package(blinkt_interface REQUIRED)
find_package(argparse REQUIRED)

idlcxx_generate(TARGET Msgs_lib FILES "./idl/LedState.idl")

add_executable(j7s-sub src/j7s-sub.cpp)
target_link_libraries(j7s-sub CycloneDDS-CXX::ddscxx Msgs_lib blinkt_interface::blinkt)
set_property(TARGET j7s-sub PROPERTY CXX_STANDARD 17)

add_executable(j7s-pub src/j7s-pub.cpp)
target_link_libraries(j7s-pub CycloneDDS-CXX::ddscxx Msgs_lib argparse::argparse)
