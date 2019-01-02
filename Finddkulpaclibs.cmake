# ===================================================================================
#  dkulpaclibs CMake configuration file
#
#             ** File generated automatically, do not modify **
#
#  Usage from an external project:
#    In your CMakeLists.txt, add these lines:
#       find_package(dkulpaclibs)
#       
#
#    FIND_PACKAGE(dkulpaclibs REQUIRED)
#    TARGET_LINK_LIBRARIES(MY_TARGET_NAME dkulpaclibs_misc_LIB dkulpaclibs_hw_LIB)
#
#
# ===================================================================================

set(dkulpaclibs_ROOT
        "${dkulpamisc_ROOT}"
        CACHE
        PATH "/usr/local/")

#MISC
find_library(dkulpaclibs_misc_LIB
        NAMES dkulpaclibs_misc
        PATHS "${dkulpaclibs_ROOT}"
        PATH_SUFFIXES lib)

find_path(dkulpaclibs_misc_INC
        NAMES dkulpaclibs/misc/Clock.h dkulpaclibs/misc/debug/Log.h
        PATHS "${dkulpaclibs_ROOT}"
        PATH_SUFFIXES include)

#HW Drivers
find_library(dkulpaclibs_hw_LIB
        NAMES dkulpaclibs_hw
        PATHS "${dkulpaclibs_ROOT}"
        PATH_SUFFIXES lib)

find_path(dkulpaclibs_hw_INC
        NAMES dkulpaclibs/hw/Camera.h dkulpaclibs/hw/bluetooth/BtRFCOMMServer.h 
        PATHS "${dkulpaclibs_ROOT}"
        PATH_SUFFIXES include)

message(${dkulpaclibs_hw_INC})

include_directories(${dkulpaclibs_misc_INC})
include_directories(${dkulpaclibs_hw_INC})
