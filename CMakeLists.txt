cmake_minimum_required(VERSION 3.27)
project(pcclub LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")

include_directories(
    include
    include/core
    include/io
    include/services
    include/utils
)

file(GLOB_RECURSE SOURCES 
    "src/*.cpp"
    "src/core/*.cpp"
    "src/io/*.cpp" 
    "src/services/*.cpp"
		"src/app/*.cpp"
		"src/utils/*.cpp"
)

list(FILTER SOURCES EXCLUDE REGEX ".*_test.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})

install(TARGETS ${PROJECT_NAME} DESTINATION bin)

