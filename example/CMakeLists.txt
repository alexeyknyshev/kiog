file(GLOB SOURCE_FILES example.cpp)

add_executable(kiog_example ${SOURCE_FILES})

include_directories(${OGRE_INCLUDE_DIR}/OGRE)
include_directories(${KIUI_INCLUDE_DIR})
include_directories(${KIUI_INCLUDE_DIR}/kiui)
include_directories(${KIOG_SOURCE_DIR}/src)

target_link_libraries(kiog_example ${KIUI_LIBRARIES})
target_link_libraries(kiog_example kiog)

install(TARGETS kiog_example RUNTIME DESTINATION "bin")