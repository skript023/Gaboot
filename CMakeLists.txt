cmake_minimum_required(VERSION 3.20)

project(Gaboot CXX ASM_MASM)

set(SRC_DIR "${PROJECT_SOURCE_DIR}/src")

include(CheckIncludeFileCXX)

if("${CMAKE_MAKE_PROGRAM}" MATCHES "ninja")
    message("Ninja build system is being used.")
else()
    include("J:/database/Tools/vcpkg/scripts/buildsystems/vcpkg.cmake")
    message("Ninja build system is not being used.")
endif()

#include external dependencies
include(scripts/bcrypt.cmake)
include(scripts/jwt.cmake)
include(scripts/cpr.cmake)

# Gaboot
message(STATUS "Gaboot")
file(GLOB_RECURSE SRC_MAIN
    "${SRC_DIR}/**.hpp"
    "${SRC_DIR}/**.h"
    "${SRC_DIR}/**.cpp"
    "${SRC_DIR}/**.cc"
    "${SRC_DIR}/**.cxx"
    "${SRC_DIR}/**.asm"
)

if (MSVC)
  add_compile_options(/bigobj)
else ()
  add_compile_options(-Wa,-mbig-obj)
endif ()

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(Gaboot "${SRC_DIR}/main.cc")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(TARGET Gaboot PROPERTY CXX_STANDARD 23) # 23 Because std::format is not avalible in std:c++20 for some reason. Maybe it's because i use v142 toolset.
# set_property(TARGET Gaboot PROPERTY ISPC_INSTRUCTION_SETS avx2-i32x4)

source_group(TREE ${SRC_DIR} PREFIX "src" FILES ${SRC_MAIN})

target_include_directories(Gaboot PRIVATE 
  ${SRC_DIR}
  ${DROGON_INCLUDE_DIRS}
  ${openssl_INCLUDE_DIRS}
  "${jwt_SOURCE_DIR}/include"
  "${nlohmann_json_SOURCE_DIR}/single_include"
  "${bcrypt_SOURCE_DIR}/include"
)

target_precompile_headers(Gaboot PRIVATE "${SRC_DIR}/pch.hpp")

find_package(OpenSSL REQUIRED)
find_package(Drogon CONFIG REQUIRED)

target_link_libraries(Gaboot PRIVATE Drogon::Drogon bcrypt OpenSSL::SSL OpenSSL::Crypto cpr)

# Warnings as errors
set_property(TARGET Gaboot PROPERTY COMPILE_WARNING_AS_ERROR ON)
target_sources(Gaboot PRIVATE ${SRC_MAIN})

add_compile_definitions(Gaboot 
  "_CRT_SECURE_NO_WARNINGS"
  "NOMINMAX"
  "WIN32_LEAN_AND_MEAN"
)

# Add unit test build
add_subdirectory(test)