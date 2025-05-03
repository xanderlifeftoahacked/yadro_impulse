cmake_minimum_required(VERSION 3.27)
project(pcclub_tests LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")

find_package(GTest REQUIRED)

set(PROJECT_SOURCE_DIR "${CMAKE_SOURCE_DIR}/../src")
set(PROJECT_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/../include")

file(GLOB_RECURSE SRC_FILES
    "${PROJECT_SOURCE_DIR}/core/*.cpp"
    "${PROJECT_SOURCE_DIR}/io/*.cpp"
    "${PROJECT_SOURCE_DIR}/services/*.cpp"
    "${PROJECT_SOURCE_DIR}/utils/*.cpp"
)

file(GLOB TEST_SOURCES "${CMAKE_SOURCE_DIR}/*.cpp")

add_executable(${PROJECT_NAME}
    ${TEST_SOURCES}
    ${SRC_FILES}
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${PROJECT_INCLUDE_DIR}
    ${PROJECT_INCLUDE_DIR}/core
    ${PROJECT_INCLUDE_DIR}/io
    ${PROJECT_INCLUDE_DIR}/services
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    GTest::GTest
    GTest::Main
)

enable_testing()
add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})

