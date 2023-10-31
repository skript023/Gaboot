cmake_minimum_required(VERSION 3.19)

project(Gaboot CXX)

set(SRC_DIR "${PROJECT_SOURCE_DIR}/src")

include(CheckIncludeFileCXX)

if (MSVC)
    if("${CMAKE_MAKE_PROGRAM}" MATCHES "ninja")
        message("Ninja build system is being used.")
    else()
        include("J:/database/Tools/vcpkg/scripts/buildsystems/vcpkg.cmake")
        message("Ninja build system is not being used.")
    endif()
else()
    
endif()

#include external dependencies
include(scripts/git.cmake)
message("\nFetching modules")
include(scripts/bcrypt.cmake)
include(scripts/jwt.cmake)
include(scripts/cpr.cmake)
include(scripts/g3log.cmake)
include(scripts/fmtlib.cmake)
include(scripts/jsonvalidator.cmake)
include(scripts/opencv.cmake)
include(scripts/drogon.cmake)

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

set(ENABLE_ZLIB OFF)
if (NOT MSVC)
    find_package(unofficial-libmariadb QUIET)
    if (unofficial-libmariadb_FOUND)
        message("Unofficial maria is found")
    endif()
endif()

if (MSVC)
  add_compile_options(/bigobj)
else ()
  #add_compile_options(-Wa,-mbig-obj)
endif ()

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(Gaboot "${SRC_DIR}/main.cc")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(TARGET Gaboot PROPERTY CXX_STANDARD 17) # 23 Because std::format is not avalible in std:c++20 for some reason. Maybe it's because i use v142 toolset.
# set_property(TARGET Gaboot PROPERTY ISPC_INSTRUCTION_SETS avx2-i32x4)

source_group(TREE ${SRC_DIR} PREFIX "src" FILES ${SRC_MAIN})

target_include_directories(Gaboot PRIVATE 
  ${SRC_DIR}
  ${DROGON_INCLUDE_DIRS}
  ${openssl_INCLUDE_DIRS}
  "${fmtlib_SOURCE_DIR}/include"
  "${jwt_SOURCE_DIR}/include"
  "${nlohmann_json_SOURCE_DIR}/single_include"
  "${bcrypt_SOURCE_DIR}/include"
  "${jsonvalidator_SOURCE_DIR}/src"
)

target_precompile_headers(Gaboot PRIVATE "${SRC_DIR}/pch.h")

target_link_libraries(Gaboot PRIVATE Drogon::Drogon bcrypt OpenSSL::SSL OpenSSL::Crypto cpr fmt::fmt nlohmann_json_schema_validator g3log opencv_core opencv_imgcodecs opencv_highgui)

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