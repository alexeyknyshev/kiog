# Custom cmake module for kiog to find OIS

include(FindPackageHandleStandardArgs)

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

if(WIN32)
	set(OIS_INCLUDE_PATHS $ENV{PROGRAMFILES}/OIS/include ${OIS_INCLUDE_PATHS})
	set(OIS_LIB_PATHS $ENV{PROGRAMFILES}/OIS/lib ${OIS_LIB_PATHS})
endif()


find_path(OIS_INCLUDE_DIR NAMES OIS.h PATHS ${OIS_INCLUDE_PATHS} NO_DEFAULT_PATH)

find_library(OIS_LIBRARIES NAMES OIS PATHS ${OIS_LIB_PATHS} PATH_SUFFIXES a lib64 lib NO_DEFAULT_PATH)
find_library(OIS_LIBRARIES_DEBUG NAMES OIS_d PATHS ${OIS_LIB_PATHS} PATH_SUFFIXES a lib64 lib NO_DEFAULT_PATH)

find_package_handle_standard_args(OIS DEFAULT_MSG OIS_LIBRARIES OIS_INCLUDE_DIR)