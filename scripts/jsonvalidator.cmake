include(FetchContent)

FetchContent_Declare(
    jsonvalidator
    GIT_REPOSITORY https://github.com/pboettch/json-schema-validator.git
    GIT_TAG        main
    GIT_PROGRESS TRUE
) 
message("Json Validator")

FetchContent_MakeAvailable(jsonvalidator)