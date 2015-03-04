# KIOG BUILD SYSTEM

cmake_minimum_required(VERSION 2.6)
set(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS TRUE)
cmake_policy(SET CMP0003 NEW)

project(KIOG)

set(CMAKE_MODULE_PATH ${KIOG_SOURCE_DIR}/cmake/modules ${KIOG_SOURCE_DIR}/cmake/packages ${CMAKE_MODULE_PATH})

set(WARNING_LEVEL "/W4")

find_package(Ogre)
find_package(OIS)
find_package(kiui)
		
add_subdirectory(src)
add_subdirectory(example)