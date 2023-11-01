include(FetchContent)

set(JWT_BUILD_EXAMPLES OFF)
set(JWT_BUILD_TESTS OFF)

FetchContent_Declare(
    jwt
    GIT_REPOSITORY https://github.com/Thalhammer/jwt-cpp.git
    GIT_TAG ce1f9df3a9f861d136d6f0c93a6f811c364d1d3d
    GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(jwt)
message("\nFetching jwt library")