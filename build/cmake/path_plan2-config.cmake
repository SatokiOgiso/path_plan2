# path_plan2 CMake config file
#
# This file sets the following variables:
# path_plan2_FOUND - Always TRUE.
# path_plan2_INCLUDE_DIRS - Directories containing the path_plan2 include files.
# path_plan2_IDL_DIRS - Directories containing the path_plan2 IDL files.
# path_plan2_LIBRARIES - Libraries needed to use path_plan2.
# path_plan2_DEFINITIONS - Compiler flags for path_plan2.
# path_plan2_VERSION - The version of path_plan2 found.
# path_plan2_VERSION_MAJOR - The major version of path_plan2 found.
# path_plan2_VERSION_MINOR - The minor version of path_plan2 found.
# path_plan2_VERSION_REVISION - The revision version of path_plan2 found.
# path_plan2_VERSION_CANDIDATE - The candidate version of path_plan2 found.

message(STATUS "Found path_plan2-1.0.0")
set(path_plan2_FOUND TRUE)

find_package(<dependency> REQUIRED)

#set(path_plan2_INCLUDE_DIRS
#    "/usr/local/include/path_plan2-1"
#    ${<dependency>_INCLUDE_DIRS}
#    )
#
#set(path_plan2_IDL_DIRS
#    "/usr/local/include/path_plan2-1/idl")
set(path_plan2_INCLUDE_DIRS
    "/usr/local/include/"
    ${<dependency>_INCLUDE_DIRS}
    )
set(path_plan2_IDL_DIRS
    "/usr/local/include//idl")


if(WIN32)
    set(path_plan2_LIBRARIES
        "/usr/local/components/lib/libpath_plan2.a"
        ${<dependency>_LIBRARIES}
        )
else(WIN32)
    set(path_plan2_LIBRARIES
        "/usr/local/components/lib/libpath_plan2.so"
        ${<dependency>_LIBRARIES}
        )
endif(WIN32)

set(path_plan2_DEFINITIONS ${<dependency>_DEFINITIONS})

set(path_plan2_VERSION 1.0.0)
set(path_plan2_VERSION_MAJOR 1)
set(path_plan2_VERSION_MINOR 0)
set(path_plan2_VERSION_REVISION 0)
set(path_plan2_VERSION_CANDIDATE )

