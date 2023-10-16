cmake_minimum_required(VERSION 3.20)
project(Gaboot_test CXX ASM_MASM)

add_executable(${PROJECT_NAME} test_main.cc)

# ##############################################################################
# If you include the drogon source code locally in your project, use this method
# to add drogon 
# target_link_libraries(${PROJECT_NAME}_test PRIVATE drogon)
#
# and comment out the following lines
target_link_libraries(${PROJECT_NAME} PRIVATE Drogon::Drogon)

ParseAndAddDrogonTests(${PROJECT_NAME})
set_property(TARGET Gaboot_test PROPERTY CXX_STANDARD 23)