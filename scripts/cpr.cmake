include(FetchContent)

set(BUILD_TESTING_BEFORE ${BUILD_TESTING})
set(CURL_DISABLE_TESTS OFF) 
set(BUILD_SHARED_LIBS OFF)

FetchContent_Declare(
    cpr
    GIT_REPOSITORY https://github.com/libcpr/cpr.git
    GIT_TAG        99e3e23033f2fc6ef8a041d6980098aab7e77014
    GIT_PROGRESS TRUE
) 
message("\nFetching cpr library")
FetchContent_MakeAvailable(cpr)

set(BUILD_TESTING ${BUILD_TESTING_BEFORE} CACHE INTERNAL "" FORCE)