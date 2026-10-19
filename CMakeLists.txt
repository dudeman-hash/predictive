cmake_minimum_required(VERSION 3.25)

project(predictive_input_buffer VERSION 4.7.0 LANGUAGES CXX)

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
