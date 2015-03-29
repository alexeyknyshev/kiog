# Custom cmake module for kiog to find OIS

include(FindPackageHandleStandardArgs)

set(OIS_LIB_NAME OIS)
set(OIS_LIB_NAME_DEBUG OIS)

if (WIN32)
	set(OIS_INCLUDE_PATHS $ENV{PROGRAMFILES}/OIS/include ${OIS_INCLUDE_PATHS})
	set(OIS_LIB_PATHS $ENV{PROGRAMFILES}/OIS/lib ${OIS_LIB_PATHS})
    set(OIS_LIB_NAME_DEBUG OIS_d)
else ()
    set(OIS_INCLUDE_PATHS
            /usr/local/include
            /usr/X11/include
            /usr/include
            /opt/local/include
    )
    set(OIS_LIB_PATHS
            /usr/local
            /usr/X11
            /usr
    )
endif ()

find_path(OIS_INCLUDE_DIR NAMES OIS/OIS.h OIS.h PATHS ${OIS_INCLUDE_PATHS} NO_DEFAULT_PATH)

find_library(OIS_LIBRARIES NAMES ${OIS_LIB_NAME} PATHS ${OIS_LIB_PATHS} PATH_SUFFIXES a lib64 lib NO_DEFAULT_PATH)
find_library(OIS_LIBRARIES_DEBUG NAMES ${OIS_LIB_NAME_DEBUG} PATHS ${OIS_LIB_PATHS} PATH_SUFFIXES a lib64 lib NO_DEFAULT_PATH)

find_package_handle_standard_args(OIS DEFAULT_MSG OIS_LIBRARIES OIS_INCLUDE_DIR)