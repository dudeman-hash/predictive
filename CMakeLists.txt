cmake_minimum_required(VERSION 3.25)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

project(predictive_input_buffer VERSION 4.7.0 LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(
    Geode
    GIT_REPOSITORY https://github.com
    GIT_TAG        v3.13.0
)
FetchContent_MakeAvailable(Geode)

add_geode_mod(
    ${PROJECT_NAME}
    ${PROJECT_VERSION}
    SOURCES 
        src/main.cpp
)
