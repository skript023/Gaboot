include(FetchContent)

FetchContent_Declare(
    jsonvalidator
    GIT_REPOSITORY https://github.com/pboettch/json-schema-validator.git
    GIT_TAG        main
    GIT_PROGRESS TRUE
) 
message("\nFetching json validator library")

FetchContent_MakeAvailable(jsonvalidator)