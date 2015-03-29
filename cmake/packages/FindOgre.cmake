# Custom cmake module for kiog to find Ogre

include(FindPackageHandleStandardArgs)

set(OGRE_INCLUDE_PATHS
		/usr/local/include
		/usr/X11/include
		/usr/include
		/opt/local/include
)

set(OGRE_LIB_PATHS
		/usr/local
		/usr/X11
		/usr
)

if(WIN32)
	set(OGRE_INCLUDE_PATHS $ENV{PROGRAMFILES}/OGRE/include ${OGRE_INCLUDE_PATHS})
	set(OGRE_LIB_PATHS $ENV{PROGRAMFILES}/OGRE/lib/debug $ENV{PROGRAMFILES}/OGRE/lib/release ${OGRE_LIB_PATHS})
endif()


find_path(OGRE_INCLUDE_DIR NAMES OGRE/Ogre.h PATHS ${OGRE_INCLUDE_PATHS} NO_DEFAULT_PATH)

find_library(OGRE_LIBRARIES NAMES OgreMain PATHS ${OGRE_LIB_PATHS} PATH_SUFFIXES a lib64 lib NO_DEFAULT_PATH)

if (CMAKE_CONFIGURATION_TYPES MATCHES "Debug")
    find_library(OGRE_LIBRARIES_DEBUG NAMES OgreMain_d PATHS ${OGRE_LIB_PATHS} PATH_SUFFIXES a lib64 lib NO_DEFAULT_PATH)
endif ()

find_package_handle_standard_args(OGRE DEFAULT_MSG OGRE_LIBRARIES OGRE_INCLUDE_DIR)