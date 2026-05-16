cmake_minimum_required(VERSION 3.20)

project(
        OpenPitch
        VERSION 0.1.0
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(OpenPitch
        main.cpp
        Match/Match.cpp
)

target_include_directories(OpenPitch PRIVATE
        source
)