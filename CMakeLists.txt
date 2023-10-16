cmake_minimum_required(VERSION 3.20)

project(Gaboot CXX ASM_MASM)

set(SRC_DIR "${PROJECT_SOURCE_DIR}/src")

include(CheckIncludeFileCXX)
message("bcrypt dir : ${bcrypt_SOURCE_DIR}")
include(scripts/bcrypt.cmake)
#include("J:/database/Tools/vcpkg/scripts/buildsystems/vcpkg.cmake")
set(CMAKE_TOOLCHAIN_FILE "J:/database/Tools/vcpkg/scripts/buildsystems/vcpkg.cmake")
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
"${SRC_DIR}"
"J:/database/Tools/vcpkg/installed/x64-windows/include"
"${bcrypt_SOURCE_DIR}/include"
)


target_precompile_headers(Gaboot PRIVATE "${SRC_DIR}/pch.hpp")

find_package(Drogon CONFIG REQUIRED)
target_link_libraries(Gaboot PRIVATE Drogon::Drogon bcrypt)

# Warnings as errors
set_property(TARGET Gaboot PROPERTY COMPILE_WARNING_AS_ERROR ON)
target_sources(Gaboot PRIVATE ${SRC_MAIN})

add_compile_definitions(Gaboot 
"_CRT_SECURE_NO_WARNINGS"
"NOMINMAX"
"WIN32_LEAN_AND_MEAN"
)
add_subdirectory(test)