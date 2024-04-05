include(FetchContent)

FetchContent_Declare(
    jsonvalidator
    GIT_REPOSITORY https://github.com/pboettch/json-schema-validator.git
    GIT_TAG        2.3.0
    GIT_PROGRESS TRUE
) 
message("\nFetching json validator library")

FetchContent_MakeAvailable(jsonvalidator)
set_property(TARGET nlohmann_json_schema_validator PROPERTY CXX_STANDARD 20)