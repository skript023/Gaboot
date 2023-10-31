
if (MSVC)
    # Set the path to vcpkg for Windows
    set(VCPKG_EXECUTABLE "J:/database/Tools/vcpkg/vcpkg.exe")
else()
    set(VCPKG_EXECUTABLE "/mnt/j/database/Tools/vcpkg/vcpkg")
    message("GCC is used")
endif()

find_package(Drogon)

if(NOT Drogon_FOUND)
    message(STATUS "Drogon not found. Attempting to install it.")
    execute_process(
    COMMAND "${VCPKG_EXECUTABLE}" install drogon[core,ctl,orm,mysql]
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
        message(STATUS "Drogon module loaded.")
    else()
        message(FATAL_ERROR "Drogon not found even after attempting to install it.")
    endif()
else()
    find_package(OpenSSL REQUIRED)
    message("Drogon module loaded.")
endif()