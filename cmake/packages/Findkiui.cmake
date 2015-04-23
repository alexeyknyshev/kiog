# Custom cmake module for kiog to find kiui

include(FindPackageHandleStandardArgs)

set(KIUI_INCLUDE_PATHS
		/usr/local/include
		/usr/X11/include
		/usr/include
		/opt/local/include
)

set(KIUI_LIB_PATHS
		/usr/local
		/usr/X11
		/usr
)

if (WIN32)
	set(KIUI_INCLUDE_PATHS $ENV{PROGRAMFILES}/KIUI/include ${KIUI_INCLUDE_PATHS})
	set(KIUI_LIB_PATHS $ENV{PROGRAMFILES}/KIUI/lib ${KIUI_LIB_PATHS})
endif ()


find_path(KIUI_INCLUDE_DIR NAMES kiui/Ui/mkUi.h PATHS ${KIUI_INCLUDE_PATHS} NO_DEFAULT_PATH)

find_library(KIUI_LIBRARIES NAMES kiui PATHS ${KIUI_LIB_PATHS} PATH_SUFFIXES a lib64 lib NO_DEFAULT_PATH)

find_package_handle_standard_args(KIUI DEFAULT_MSG KIUI_LIBRARIES KIUI_INCLUDE_DIR)