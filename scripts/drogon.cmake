message("\nInstalling drogon library")

if (MSVC)
    # Set the path to vcpkg for Windows
    set(VCPKG_EXECUTABLE "$ENV{VCPKG_ROOT}/vcpkg.exe")
else()
    set(VCPKG_EXECUTABLE "$ENV{VCPKG_ROOT}/vcpkg")
    message("GCC is used")
endif()

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
    COMMAND ${VCPKG_EXECUTABLE} install drogon[core,ctl,orm,mysql] --clean-after-build
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