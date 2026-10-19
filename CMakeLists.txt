cmake_minimum_required(VERSION 3.25)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

project(predictive_input_buffer VERSION 1.0.0 LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(
    Geode
    GIT_REPOSITORY https://github.com
    GIT_TAG        main
)
FetchContent_MakeAvailable(Geode)

add_geode_mod(
    ${PROJECT_NAME}
    ${PROJECT_VERSION}
    SOURCES 
        src/main.cpp
)
