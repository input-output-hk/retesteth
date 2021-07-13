
cmake_minimum_required(VERSION 3.9.3)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/toolchain.cmake)

project(retesteth VERSION 0.1.1)
set(VERSION_SUFFIX "eip1559")

set(Boost_USE_MULTITHREADED ON)
find_package(PkgConfig REQUIRED)
find_package(yaml-cpp CONFIG REQUIRED libyamlcpp)
find_package(Boost CONFIG REQUIRED filesystem program_options system)
pkg_check_modules(cryptopp REQUIRED IMPORTED_TARGET libcryptopp)
pkg_check_modules(yaml-cpp REQUIRED yaml-cpp)
pkg_check_modules(CURL REQUIRED IMPORTED_TARGET libcurl)
set(CURL_TARGETS PkgConfig::CURL)
find_package(libscrypt CONFIG REQUIRED)
find_package(Threads)


#Cmake files configuration
set(ETH_CMAKE_DIR "${CMAKE_CURRENT_LIST_DIR}/cmake" CACHE PATH "The path to the cmake directory")
list(APPEND CMAKE_MODULE_PATH ${ETH_CMAKE_DIR})
include(EthOptions)
include(EthDependencies)
include(EthCompilerSettings)

include(ProjectSecp256k1)
include(ProjectLibFF)
configure_project()

add_subdirectory(libdevcore)
add_subdirectory(libdevcrypto)
add_subdirectory(retesteth)

