project(j7s-dds LANGUAGES CXX)
cmake_minimum_required(VERSION 3.5)

add_subdirectory(./ext/j7s-blinkt/blinkt_interface)

find_package(CycloneDDS-CXX REQUIRED)

idlcxx_generate(TARGET Msgs_lib FILES "./idl/LedState.idl")

add_executable(j7s-sub src/j7s-sub.cpp)
target_link_libraries(j7s-sub CycloneDDS-CXX::ddscxx Msgs_lib blinkt)
set_property(TARGET j7s-sub PROPERTY CXX_STANDARD 17)
