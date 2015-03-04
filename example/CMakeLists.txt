file(GLOB SOURCE_FILES example.cpp)

add_executable(example ${SOURCE_FILES})

include_directories(${OGRE_INCLUDE_DIR}/OGRE)
include_directories(${KIUI_INCLUDE_DIR})
include_directories(${KIOG_SOURCE_DIR}/src)

target_link_libraries(example ${KIUI_LIBRARIES})
target_link_libraries(example kiog)

install(TARGETS kiog RUNTIME DESTINATION "example/")
install(FILES ${KIUI_LIBRARIES} ${OGRE_LIBRARIES_DEBUG} ${OIS_LIBRARIES_DEBUG} DESTINATION "example/")
install(FILES ${OGRE_LIBRARIES_DEBUG} DESTINATION "example/")
install(FILES ${OGRE_LIBRARIES_DEBUG} DESTINATION "example/")