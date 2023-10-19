cmake_minimum_required(VERSION 3.19)
project(Gaboot_test CXX)

add_executable(Gaboot_test test_main.cc)

# ##############################################################################
# If you include the drogon source code locally in your project, use this method
# to add drogon 
# target_link_libraries(${PROJECT_NAME}_test PRIVATE drogon)
#
# and comment out the following lines
target_link_libraries(Gaboot_test PRIVATE Drogon::Drogon)

ParseAndAddDrogonTests(Gaboot_test)

set_property(TARGET Gaboot_test PROPERTY CXX_STANDARD 17)