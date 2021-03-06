# KIOG BUILD SYSTEM

cmake_minimum_required(VERSION 2.8.3)
set(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS TRUE)
cmake_policy(SET CMP0003 NEW)

project(KIOG)

set(CMAKE_MODULE_PATH ${KIOG_SOURCE_DIR}/cmake/modules ${KIOG_SOURCE_DIR}/cmake/packages ${CMAKE_MODULE_PATH})

if (MSVC)
    add_compile_options(/W4)
#else ()
#    add_compile_options("-Wall -Wno-long-long -pedantic")
endif ()

if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -std=gnu++11)
    add_definitions("-DSTD_NO_MAKE_UNIQUE")
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -std=c++11)
    add_definitions("-DSTD_NO_MAKE_UNIQUE")
endif ()

find_package(Ogre 2.0 REQUIRED)
find_package(OIS REQUIRED)
find_package(kiui REQUIRED)

add_definitions(-DKIOG_EXAMPLE_RESSOURCE_PATH="${CMAKE_INSTALL_PREFIX}/data/")
		
add_subdirectory(src)

set(KIOG_BUILD_EXAMPLE yes CACHE BOOL "Build example kiog app")
if (KIOG_BUILD_EXAMPLE)
    add_subdirectory(example)
endif()

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/data/ DESTINATION kiog/data)