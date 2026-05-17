cmake_minimum_required(VERSION 3.20)

project(
        OpenPitch
        VERSION 0.1.0
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_OSX_ARCHITECTURES
        "x86_64;arm64"
)

add_library(OpenPitch SHARED
        Engine/Match/Match.cpp
)

target_include_directories(OpenPitch PUBLIC
        .
)

target_compile_definitions(OpenPitch PRIVATE
        OPENPITCH_EXPORTS
)

add_executable(OpenPitchApp
        main.cpp
)

target_link_libraries(OpenPitchApp PRIVATE
        OpenPitch
)