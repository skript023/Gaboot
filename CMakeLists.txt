cmake_minimum_required(VERSION 3.19)

project(Gaboot CXX)

set(SRC_DIR "${PROJECT_SOURCE_DIR}/src")

include(CheckIncludeFileCXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (WIN32)
    message("Windows detected, add windows toolchain from ${PROJECT_SOURCE_DIR}/conan/Linux/build/Release/generators/conan_toolchain.cmake")
    include("${PROJECT_SOURCE_DIR}/conan/Windows/build/Release/generators/conan_toolchain.cmake")
elseif (UNIX)
    message("Linux detected, add linux toolchain from ${PROJECT_SOURCE_DIR}/conan/Linux/build/Release/generators/conan_toolchain.cmake")
    set(CMAKE_TOOLCHAIN_FILE "${PROJECT_SOURCE_DIR}/conan/Linux/build/Release/generators/conan_toolchain.cmake")
endif()

#include vcpkg dependencies
message("\nFetching packages")
find_package(Drogon REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(OpenCV CONFIG REQUIRED)

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

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  enable_language("RC")
  set (ICON ${SRC_DIR}/gaboot_icon.rc)
endif()

if (MSVC)
  add_compile_options(/bigobj)
  add_compile_options(/utf-8)
else ()
  #add_compile_options(-Wa,-mbig-obj)
  add_compile_options(-finput-charset=UTF-8)
  add_compile_options(-fexec-charset=UTF-8)
endif ()

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# Specify the destination directory (out/build/windows-x64)
if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(DESTINATION_DIR ${CMAKE_CURRENT_BINARY_DIR}/../Windows-x64)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(DESTINATION_DIR ${CMAKE_CURRENT_BINARY_DIR}/../Linux-x64)
endif()

# Add a custom command to copy .env and config.json to the build directory
if (EXISTS "${DESTINATION_DIR}/config.json" AND EXISTS "${DESTINATION_DIR}/.env")
    add_custom_command(
        OUTPUT ${DESTINATION_DIR}/.env
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SOURCE_DIR}/.env ${DESTINATION_DIR}/.env
        DEPENDS ${SOURCE_DIR}/.env
        COMMENT "Copying .env to build directory"
    )

    add_custom_command(
        OUTPUT ${DESTINATION_DIR}/config.json
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SOURCE_DIR}/config.json ${DESTINATION_DIR}/config.json
        DEPENDS ${SOURCE_DIR}/config.json
        COMMENT "Copying config.json to build directory"
    )
endif()

# Add a custom command to copy the folder and its contents
if (EXISTS "${SOURCE_DIR}/src/assets")
    add_custom_target(copy_folder_to_build ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${SOURCE_DIR}/src/assets" "${DESTINATION_DIR}/assets"
        COMMENT "Copying assets to build directory"
    )
endif()

# Add a custom target to make sure the custom command is executed before building your main target
if (EXISTS "${DESTINATION_DIR}/config.json" AND EXISTS "${DESTINATION_DIR}/.env")
    add_custom_target(CopyConfigFiles DEPENDS ${DESTINATION_DIR}/config.json ${DESTINATION_DIR}/.env)
endif()

# Compile main.cc as executable
add_executable(Gaboot "${SRC_DIR}/main.cc" ${VIEWSRC} ${ICON})

if (EXISTS "${DESTINATION_DIR}/config.json" AND EXISTS "${DESTINATION_DIR}/.env")
    # Add dependencies to make sure the custom commands are executed before building YourTarget
    add_dependencies(Gaboot CopyConfigFiles)
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(TARGET Gaboot PROPERTY CXX_STANDARD 20)
# set_property(TARGET Gaboot PROPERTY ISPC_INSTRUCTION_SETS avx2-i32x4)

source_group(TREE ${SRC_DIR} PREFIX "src" FILES ${SRC_MAIN})

target_include_directories(Gaboot PRIVATE 
  ${SRC_DIR}
  "${SRC_DIR}/module"
  "${SRC_DIR}/util"
  "${SRC_DIR}/middleware"
  "${SRC_DIR}/interfaces"
  "${SRC_DIR}/interfaces/payments"
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
#add_subdirectory(test)