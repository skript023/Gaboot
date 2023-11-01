cmake_minimum_required(VERSION 3.19)

project(Gaboot CXX)

set(SRC_DIR "${PROJECT_SOURCE_DIR}/src")

include(CheckIncludeFileCXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if("${CMAKE_MAKE_PROGRAM}" MATCHES "ninja")
    message("Ninja build system is being used.")
else()
    include("$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
    message("Ninja build system is not being used.")
endif()

#generate version file
include(scripts/git.cmake)

#include external dependencies
message("\nFetching modules")
include(scripts/bcrypt.cmake)
include(scripts/jwt.cmake)
include(scripts/cpr.cmake)
include(scripts/g3log.cmake)
include(scripts/fmtlib.cmake)
include(scripts/jsonvalidator.cmake)

#include vcpkg dependencies
message("\nFetching packages")
include(scripts/opencv.cmake)
include(scripts/drogon.cmake)

# Gaboot
message(STATUS "Gaboot")
file(GLOB_RECURSE SRC_MAIN
    "${SRC_DIR}/**.hpp"
    "${SRC_DIR}/**.h"
    "${SRC_DIR}/**.hxx"
    "${SRC_DIR}/**.hh"
    "${SRC_DIR}/**.cpp"
    "${SRC_DIR}/**.cc"
    "${SRC_DIR}/**.cxx"
    "${SRC_DIR}/**.asm"
)

if (MSVC)
  add_compile_options(/bigobj)
else ()
  #add_compile_options(-Wa,-mbig-obj)
endif ()

add_executable(Gaboot "${SRC_DIR}/main.cc")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(TARGET Gaboot PROPERTY CXX_STANDARD 17)
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