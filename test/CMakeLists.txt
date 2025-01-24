cmake_minimum_required(VERSION 3.2)

project(
    warlogs_test
)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
    ${PROJECT_NAME}
    ${CMAKE_CURRENT_LIST_DIR}/event_parse.cpp
)

target_link_libraries(
    ${PROJECT_NAME}
    GTest::gtest_main
    warlogs
)
