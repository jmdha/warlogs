cmake_minimum_required(
    VERSION 3.2
)

project(
    warlogs
    VERSION 0.0.1
    DESCRIPTION "World of Warcraft log parsing"
    LANGUAGES CXX
)

add_library(
    ${PROJECT_NAME}
    SHARED
    ${CMAKE_CURRENT_LIST_DIR}/include/${PROJECT_NAME}/event.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/${PROJECT_NAME}/utils.hpp
    ${CMAKE_CURRENT_LIST_DIR}/src/event.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/utils.cpp
)

set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES
    LINKER_LANGUAGE CXX
)

set_property(
    TARGET ${PROJECT_NAME}
    PROPERTY
    CXX_STANDARD 17
)

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
)

install(
    DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/include/${PROJECT_NAME}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
)

add_subdirectory(test)
