include(FetchContent)

message("Bcrypt CPP")
FetchContent_Declare(
    bcrypt
    GIT_REPOSITORY https://github.com/hilch/Bcrypt.cpp.git
    GIT_TAG 064cb04fc5e19ba9705d82c00bc9f389d84248c1
    GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(bcrypt)

set_property(TARGET bcrypt PROPERTY CXX_STANDARD 23)
