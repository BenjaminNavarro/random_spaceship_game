![CI](https://github.com/BenjaminNavarro/random_spaceship_game/workflows/CI/badge.svg?branch=master)

Spaceship generator
===================

This project contains my take on the *Spaceship Generator for May the 4th in C++* game proposed by Bartłomiej Filipek on his [blog](https://www.bfilipek.com/2020/05/spaceshipgen.html). 

Objective
=========

Assemble a spaceship by taking parts at random from a list. Each spaceship contains:
 * 1 engine
 * 1 fuselage part
 * 1 cabin
 * 1 armor part
 * [0-1] small wings
 * [0-1] large wings
 * [0-4] weapons

A *legacy* C++ prototype is given and can be found in *given_implem.cpp*.
The goal is to rewrite it in the best possible way using modern C++ techniques and with the standard library as the only dependency. C++20 is allowed but partial support at the time of writing forbid the use of some features, such as `std::format`. The main modern C++ features used are:
 * [C++20] templated lambda functions
 * [C++20] `std::string`'s `start_with` and `ends_with`
 * [C++17] `std::optional`
 * [C++17] `std::string_view`
 * [C++17] `[[nodiscard]]` attribute
 * [C++17] `if` with init-statement
 * [C++14] generic lambdas
 * [C++11] range-based `for` loops

In the proposed solution, the part list is used to describe a stock of parts and spaceships will be built as long as enough parts are in stock, instead of building just one spaceship

Build & Run
===========

In order to build this project you need the following tools:
 * GCC >= 9.3
 * CMake >= 3.12.4

Clone or download this repository and then:
```bash
cd random_spaceship_game
mkdir build && cd build
cmake ..
cmake --build . --parallel
```

You will end up with two executables inside the *build* folder:
 * `spaceship-generator`: the proposed solution
 * `given_implem`: the given *legacy* implementation (for comparison)

`spaceship-generator` takes two optional arguments, given in any order:
 * **--verbose**: print more information to the console compared to the original implementation
 * **file_path**: use a part list different from the *vehicle_parts.txt* file present in the root directory

Using the original part file, a non-verbose output can be:
```
The wing type [X-style] is not a valid spaceship wing type, skipping
The wing type [no] is not a valid spaceship wing type, skipping
The part type [shield] is not a valid spaceship part, skipping
The part type [shield] is not a valid spaceship part, skipping
A ship with big rocket engine, falcon-style fuselage, small cabin, large plane wings, liquid armor and no weapons
A ship with lightspeed engine, small streamline fuselage, large cabin, small V-shaped wings, rocket resistant armor and 1 weapon: bubble gum launcher weapon
```

While a verbose one can be:
```
Loading parts from /home/user/random_spaceship_game/vehicle_parts.txt
The wing type [X-style] is not a valid spaceship wing type, skipping
The wing type [no] is not a valid spaceship wing type, skipping
The part type [shield] is not a valid spaceship part, skipping
The part type [shield] is not a valid spaceship part, skipping
Trying to build a spaceship using the following available parts:
engines:
	- super ionizing engine
	- small rocket engine
	- big rocket engine
	- lightspeed engine
fuselages:
	- falcon-style fuselage
	- small streamline fuselage
	- tie-fighter fuselage
cabins:
	- large cabin
	- small cabin
armors:
	- liquid armor
	- 100% energy field armor
	- laser armor
	- rocket resistant armor
small wings:
	- small V-shaped wings
large wings:
	- large plane wings
weapons:
	- laser cannon weapon
	- rocket launcher weapon
	- bubble gum launcher weapon
	- string tokens cannon weapon

We built the following spaceship: A ship with small rocket engine, tie-fighter fuselage, small cabin, large plane wings, small V-shaped wings, liquid armor and 2 weapons: rocket launcher weapon and laser cannon weapon
Trying to build a spaceship using the following available parts:
engines:
	- super ionizing engine
	- big rocket engine
	- lightspeed engine
fuselages:
	- falcon-style fuselage
	- small streamline fuselage
cabins:
	- large cabin
armors:
	- 100% energy field armor
	- laser armor
	- rocket resistant armor
small wings:
large wings:
weapons:
	- bubble gum launcher weapon
	- string tokens cannon weapon

We built the following spaceship: A ship with super ionizing engine, small streamline fuselage, large cabin, 100% energy field armor and 1 weapon: bubble gum launcher weapon
Trying to build a spaceship using the following available parts:
engines:
	- big rocket engine
	- lightspeed engine
fuselages:
	- falcon-style fuselage
cabins:
armors:
	- laser armor
	- rocket resistant armor
small wings:
large wings:
weapons:
	- string tokens cannon weapon

Sorry, not enough parts to build a spaceship, exiting
```
