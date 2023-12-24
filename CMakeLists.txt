# Preamble
cmake_minimum_required(VERSION 3.20)
project(trik-desktop-gamepad VERSION 1.0.0 LANGUAGES CXX)

# Project wide setup
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Prevent from build in source dir 
include(NoInSourceBuilds)

# Externally provided content
include(QtHelper)
add_subdirectory(dependencies)

# Main targets built by this project
add_subdirectory(src)

# Process fonts, images, translations
add_subdirectory(share)

# TO DO:
# add_subdirectory(tests)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    add_subdirectory(packaging)
endif()
