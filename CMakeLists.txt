cmake_minimum_required(
    VERSION 3.22.1
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
    ${CMAKE_CURRENT_LIST_DIR}/include/${PROJECT_NAME}/types.hpp
)

set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES
    LINKER_LANGUAGE CXX
)

target_include_directories(
    ${PROJECT_NAME}
    INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
)

install(
    DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/include/${PROJECT_NAME}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
)
