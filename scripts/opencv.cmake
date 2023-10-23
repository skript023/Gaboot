include(FetchContent)

FetchContent_Declare(
    opencv
    GIT_REPOSITORY https://github.com/opencv/opencv.git
    GIT_TAG        d142a796d8404541c06b3e778c34b363f5ac4c0c
    GIT_PROGRESS TRUE
)

message("Open CV")
FetchContent_MakeAvailable(opencv)