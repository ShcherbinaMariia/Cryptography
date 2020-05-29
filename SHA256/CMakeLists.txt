cmake_minimum_required(VERSION 3.14)
set(CMAKE_CXX_STANDARD 17)

project(SHA256)

find_package(nlohmann_json 3.2.0 REQUIRED)

# library for working with sha256 and LongHex

set(SOURCE_LIB sha256.cpp long_hex.cpp)
add_library(SHA256 STATIC ${SOURCE_LIB})

target_link_libraries(SHA256 nlohmann_json::nlohmann_json)