include(FetchContent)

set(BUILD_SHARED_LIBS OFF)

FetchContent_Declare(
    fmtlib
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG        c089f7d497bcce35a0fd6da0512f8585ed1cc002
    GIT_PROGRESS TRUE
) 
message("fmtlib")

FetchContent_MakeAvailable(fmtlib)