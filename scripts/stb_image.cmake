# this package no longer used

include(FetchContent)

FetchContent_Declare(
    stb_image
    GIT_REPOSITORY https://github.com/nothings/stb.git
    GIT_TAG        beebb24b945efdea3b9bba23affb8eb3ba8982e7
    GIT_PROGRESS TRUE
)

message("STB Image")
FetchContent_MakeAvailable(stb_image)