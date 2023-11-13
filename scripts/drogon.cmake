message("\nInstalling drogon library")

if (MSVC)
    # Set the path to vcpkg for Windows
    set(VCPKG_ROOT "$ENV{VCPKG_ROOT}")
    set(VCPKG_EXECUTABLE "./vcpkg.exe")
    message("MSVC is used.")
else()
    set(VCPKG_ROOT "/mnt/j/database/Tools/vcpkg")
    set(VCPKG_EXECUTABLE "./vcpkg")
    message("GCC is used")
endif()

message(STATUS "Drogon attempting to finding package, vcpkg found in ${VCPKG_ROOT}")

set(ENABLE_ZLIB OFF)
if (NOT MSVC)
    find_package(unofficial-libmariadb QUIET)
    if (unofficial-libmariadb_FOUND)
        message("Unofficial maria is found")
    endif()
endif()

find_package(Drogon)

if(NOT Drogon_FOUND)
    message(STATUS "Drogon not found. Attempting to install it.")
    execute_process(
    COMMAND cd "${VCPKG_ROOT}" && ${VCPKG_EXECUTABLE} install drogon[core,ctl,orm,mysql]
    RESULT_VARIABLE VCPKG_RESULT
    )

    if(VCPKG_RESULT)
        message(FATAL_ERROR "vcpkg failed with error code: ${VCPKG_RESULT}")
    else()
        message(STATUS "Drogon successfully installed")
    endif()

    find_package(Drogon)

    if(OpenCV_FOUND)
        find_package(OpenSSL REQUIRED)
        message(STATUS "Drogon is found.")
    else()
        message(FATAL_ERROR "Drogon not found even after attempting to install it.")
    endif()
else()
    find_package(OpenSSL REQUIRED)
    message(STATUS "Drogon is already exist, located in : ${DROGON_INCLUDE_DIRS}")
endif()