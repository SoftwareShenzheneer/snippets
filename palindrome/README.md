# CMake template
CMake is a more modern approach to handling builds and to familiarise myself a little bit more with CMake from the ground up, I decided to create a template project. This can be used for any (small) project as well, just substitute the relevant files.
___

## Table of contents
- [Project description](#project-description)
- [Install](#install)
- [Use](#use)
___

## Project Description
CMake template project containing a main application and 3 module directories.

___

## Install
Ensure CMake is installed. Test with:
```
cmake --version
```
Then the build.sh script can be run.
___

## Use
1. Remove modX directories
2. Replace with own directories, ensure to copy over CMakeList.txt files
3. Configure correct directory structure in CMakeLists.txt files
4. Run build.sh from top-level directory with (currently) the following options:
```
./build.sh - performs a build with default parameters: debug, not clean
./build.sh clean - performs a clean build with default build setting Debug
./build.sh debug/Debug/release/Release - performs a build with preferred build setting, not clean
```


