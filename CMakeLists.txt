cmake_minimum_required(VERSION 3.28)
project(PlanesGuessingGame LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

add_executable(game src/main.cpp
        src/Airplane.cpp
        include/Airplane.hpp
        src/Player.cpp
        include/Player.hpp)
target_include_directories(game PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_compile_features(game PRIVATE cxx_std_20)
target_link_libraries(game PRIVATE SFML::Graphics)
