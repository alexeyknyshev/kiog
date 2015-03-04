# Include the directory itself as a path to include directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
 
file(GLOB HEADER_FILES 	"${CMAKE_CURRENT_SOURCE_DIR}/Og/*.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/Og/Gorilla/*.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/Og/Gorilla/RectPacking/*.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/Og/Input/*.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/Og/Ogre/*.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/Og/Ogre/Shape/mkOgreShape.h"
						#"${CMAKE_CURRENT_SOURCE_DIR}/Og/Ogre/Shape/*.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/Og/Ogre/CollisionTools/*.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/Og/Widget/*.h")

file(GLOB SOURCE_FILES 	"${CMAKE_CURRENT_SOURCE_DIR}/Og/*.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/Og/Gorilla/*.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/Og/Gorilla/RectPacking/*.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/Og/Input/*.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/Og/Ogre/*.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/Og/Ogre/Shape/mkOgreShape.cpp"
						#"${CMAKE_CURRENT_SOURCE_DIR}/Og/Ogre/Shape/*.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/Og/Ogre/CollisionTools/*.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/Og/Widget/*.cpp")
						
if (BUILD_SOUND)
	file(GLOB SOUND_HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Sound/*.h")
	set(SOURCE_FILES ${HEADER_FILES} ${SOUND_HEADER_FILES})
	
	file(GLOB SOUND_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Sound/*.cpp")
	set(SOURCE_FILES ${SOURCE_FILES} ${SOUND_SOURCE_FILES})
endif ()

if (BUILD_SOUND)
	add_definitions("-DSOUND_EXPORT")
	add_definitions("-DKIOG_SOUND")
endif ()

add_definitions("-DOG_EXPORT")


if (MSVC)
	add_definitions("-D_CRT_SECURE_NO_WARNINGS")
endif ()

add_library(kiog SHARED ${SOURCE_FILES} ${HEADER_FILES})

target_link_libraries(kiog optimized ${OGRE_LIBRARIES} debug ${OGRE_LIBRARIES_DEBUG})
target_link_libraries(kiog optimized ${OIS_LIBRARIES} debug ${OIS_LIBRARIES_DEBUG})
target_link_libraries(kiog ${KIUI_LIBRARIES})

include_directories(${OGRE_INCLUDE_DIR})
include_directories(${OGRE_INCLUDE_DIR}/OGRE)
include_directories(${OGRE_INCLUDE_DIR}/Components/RTShaderSystem/)
include_directories(${OIS_INCLUDE_DIR})
include_directories(${KIUI_INCLUDE_DIR})

#install(FILES ${HEADER_FILES} DESTINATION "include")
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/ DESTINATION "include")

if (WIN32)
	install(TARGETS kiog ARCHIVE DESTINATION "lib" RUNTIME DESTINATION "lib")
else ()
	install(TARGETS kiog LIBRARY DESTINATION "lib")
endif ()