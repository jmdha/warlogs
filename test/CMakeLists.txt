FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(
    ${PROJECT_NAME}_test
    ${CMAKE_CURRENT_LIST_DIR}/event_parse.cpp
    ${CMAKE_CURRENT_LIST_DIR}/utils.cpp
)

target_link_libraries(
    ${PROJECT_NAME}_test
    GTest::gtest_main
    warlogs
)
include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}_test)
