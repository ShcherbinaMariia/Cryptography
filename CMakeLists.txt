cmake_minimum_required(VERSION 3.14)
set(CMAKE_CXX_STANDARD 11)

project(cryptography)

set(SOURCE_EXE main.cpp cryptography.cpp elliptic_curve.cpp ElGamal.cpp mapping.cpp rng.cpp)
include_directories(BigInteger)

add_executable(cryptography ${SOURCE_EXE})
add_subdirectory(BigInteger)

target_link_libraries(cryptography bigint)

include_directories(SHA256)

find_package(nlohmann_json 3.2.0 REQUIRED)

add_executable(blockchain blockchain.cpp blockchain_client.cpp merkle_tree.cpp prime_rng.cpp rng.cpp cryptography.cpp rsa.cpp blockchain_main.cpp)
add_subdirectory(SHA256)

target_link_libraries(blockchain bigint SHA256 nlohmann_json::nlohmann_json)