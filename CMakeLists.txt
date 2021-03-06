cmake_minimum_required(VERSION 3.12.4)

project(
    random_spaceship_game
    VERSION 1.0.0
    DESCRIPTION "Proposed solution for the spaceship generator game https://www.bfilipek.com/2020/05/spaceshipgen.html"
    LANGUAGES CXX
)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
find_package(Sanitizers)

add_executable(given_implem given_implem.cpp)
target_compile_features(given_implem PRIVATE cxx_std_11)

set(parts_file ${CMAKE_SOURCE_DIR}/vehicle_parts.txt)

add_executable(spaceship-generator main.cpp)
target_compile_features(spaceship-generator PRIVATE cxx_std_20)
target_compile_definitions(spaceship-generator PRIVATE PARTS_FILE="${parts_file}")
add_sanitizers(spaceship-generator)
# More warnings and treats them as errors
if(MSVC)
  target_compile_options(spaceship-generator PRIVATE /W4 /WX)
else()
  target_compile_options(spaceship-generator PRIVATE -Wall -Wextra -pedantic -Werror)
endif()