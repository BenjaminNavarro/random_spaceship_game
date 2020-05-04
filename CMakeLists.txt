cmake_minimum_required(VERSION 3.0.2)

project(
    random_spaceship_game
    VERSION 1.0.0
    DESCRIPTION "Proposed solution for the spaceship generator game https://www.bfilipek.com/2020/05/spaceshipgen.html"
    LANGUAGES CXX
)

add_executable(given_implem given_implem.cpp)
target_compile_features(given_implem PRIVATE cxx_std_11)

set(parts_file ${CMAKE_SOURCE_DIR}/vehicle_parts.txt)

add_executable(spaceship-generator main.cpp)
target_compile_features(spaceship-generator PRIVATE cxx_std_20)
target_compile_definitions(spaceship-generator PRIVATE PARTS_FILE="${parts_file}")
