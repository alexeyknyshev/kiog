# Custom cmake module for kiog to find Ogre

include(FindPackageHandleStandardArgs)

set(OGRE_LIB_NAME OgreMain)
set(OGRE_LIB_NAME_DEBUG OgreMain)

if (WIN32)
	set(OGRE_INCLUDE_PATHS $ENV{PROGRAMFILES}/OGRE/include ${OGRE_INCLUDE_PATHS})
	set(OGRE_LIB_PATHS $ENV{PROGRAMFILES}/OGRE/lib/debug $ENV{PROGRAMFILES}/OGRE/lib/release ${OGRE_LIB_PATHS})
    set(OGRE_LIB_NAME_DEBUG OgreMain_d)
else ()
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
endif ()

find_path(OGRE_INCLUDE_DIR NAMES OGRE/Ogre.h PATHS ${OGRE_INCLUDE_PATHS} NO_DEFAULT_PATH)

find_library(OGRE_LIBRARIES NAMES ${OGRE_LIB_NAME} PATHS ${OGRE_LIB_PATHS} PATH_SUFFIXES a lib64 lib NO_DEFAULT_PATH)
find_library(OGRE_LIBRARIES_DEBUG NAMES ${OGRE_LIB_NAME_DEBUG} PATHS ${OGRE_LIB_PATHS} PATH_SUFFIXES a lib64 lib NO_DEFAULT_PATH)

if (OGRE_INCLUDE_DIR)
    include(PreprocessorUtils)
    file(READ ${OGRE_INCLUDE_DIR}/OGRE/OgrePrerequisites.h OGRE_TEMP_VERSION)
    get_preprocessor_entry(OGRE_TEMP_VERSION OGRE_VERSION_MAJOR OGRE_VERSION_MAJOR)
    get_preprocessor_entry(OGRE_TEMP_VERSION OGRE_VERSION_MINOR OGRE_VERSION_MINOR)
    get_preprocessor_entry(OGRE_TEMP_VERSION OGRE_VERSION_PATCH OGRE_VERSION_PATCH)
    get_preprocessor_entry(OGRE_TEMP_VERSION OGRE_VERSION_NAME OGRE_VERSION_NAME)
    set(OGRE_VERSION "${OGRE_VERSION_MAJOR}.${OGRE_VERSION_MINOR}.${OGRE_VERSION_PATCH}")
endif ()

find_package_handle_standard_args(  Ogre
                                    FOUND_VAR OGRE_FOUND
                                    REQUIRED_VARS OGRE_LIBRARIES OGRE_INCLUDE_DIR
                                    VERSION_VAR OGRE_VERSION  )