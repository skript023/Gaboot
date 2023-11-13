message("\nInstalling openCV library")
  
if (MSVC)
    # Set the path to vcpkg for Windows
    set(VCPKG_ROOT "$ENV{VCPKG_ROOT}")
    message("MSVC is used.")
else()
    set(VCPKG_ROOT "/mnt/j/database/Tools/vcpkg")
    message("GCC is used.")
endif()

message(STATUS "OpenCV attempting to finding package, vcpkg found in ${VCPKG_ROOT}")

find_package(OpenCV)

if(NOT OpenCV_FOUND)
    
    message(STATUS "OpenCV not found. Attempting to install it.")
    execute_process(
    COMMAND cd "${VCPKG_ROOT}" && vcpkg install opencv
    RESULT_VARIABLE VCPKG_RESULT
    )

    if(VCPKG_RESULT)
        message(FATAL_ERROR "vcpkg failed with error code: ${VCPKG_RESULT}")
    else()
        message(STATUS "OpenCV has been successfully installed.")
    endif()

    find_package(OpenCV CONFIG REQUIRED)

    if(OpenCV_FOUND)
        message(STATUS "OpenCV is found.")
        # You can use OpenCV here
    else()
        message(FATAL_ERROR "OpenCV not found even after attempting to install it.")
    endif()
else()
    message(STATUS "OpenCV is already exist.")
endif()


#sudo apt install libx11-dev libxft-dev
#sudo apt-get install autoconf
#sudo apt-get install libtool
#sudo apt-get install bison
#sudo apt-get install gperf
#sudo apt-get install libgles2-mesa-dev
#sudo apt install libxext-dev
#sudo apt install libxrandr-dev
#sudo apt-get install libxi-dev
#sudo apt-get install libxcursor-dev
#sudo apt-get install libxdamage-dev
#sudo apt-get install libxinerama-dev

#find_package(OpenCV CONFIG REQUIRED)
#1> [CMake]   # note: 10 additional targets are not displayed.
#1> [CMake]   target_link_libraries(main PRIVATE opencv_ml opencv_dnn opencv_core opencv_flann)