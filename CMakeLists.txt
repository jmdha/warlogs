cmake_minimum_required(
    VERSION 3.2
)

project(
    warlogs
    VERSION 0.0.1
    DESCRIPTION "World of Warcraft log parsing"
    LANGUAGES C
)

include(FetchContent)

add_library(
    ${PROJECT_NAME}
    INTERFACE
)

target_include_directories(
    ${PROJECT_NAME}
    INTERFACE
    .
)

set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES
    LINKER_LANGUAGE C
)

set_property(
    TARGET ${PROJECT_NAME}
    PROPERTY
    C_STANDARD 17
)

if(warlogs_test)
FetchContent_Declare(utest GIT_REPOSITORY https://github.com/sheredom/utest.h.git)
FetchContent_MakeAvailable(utest)
add_executable(test test/test.c test/parse.c test/parse_timestamp.c test/match.c warlogs.h)
target_link_libraries(test m)
target_include_directories(test PRIVATE ${utest_SOURCE_DIR})
endif()
