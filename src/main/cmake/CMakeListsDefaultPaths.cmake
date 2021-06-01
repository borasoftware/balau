
# This file must be included before the project() declaration.

############################# CMAKE CONFIGURATION #############################

message(STATUS "CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}")
message(STATUS "CMAKE_BINARY_DIR: ${CMAKE_BINARY_DIR}")

## Disallow in-source builds.
if ("${CMAKE_BINARY_DIR}" STREQUAL "${CMAKE_SOURCE_DIR}")
	message(FATAL_ERROR
		"No in-source builds please."
		"    mkdir build; cd build; cmake .."
		"Please first remove the files created by this failed run with"
		"    rm -rf CMakeCache.txt CMakeFiles"
	)
endif ()

#
# User environment variables that automatically set CMAKE_PREFIX_PATH and
# CMAKE_INSTALL_PREFIX if they have not already been specified on the
# command line.
#
# These allow developers to set the prefix path and install path a single time
# in .bashrc, instead of requiring them to be specified on the cmake command line.
#
set(CPP_USER_CMAKE_PREFIX_PATH "$ENV{CPP_USER_CMAKE_PREFIX_PATH}")
set(CPP_USER_CMAKE_INSTALL_PREFIX "$ENV{CPP_USER_CMAKE_INSTALL_PREFIX}")

message(STATUS "CPP_USER_CMAKE_PREFIX_PATH: ${CPP_USER_CMAKE_PREFIX_PATH}")
message(STATUS "CPP_USER_CMAKE_INSTALL_PREFIX: ${CPP_USER_CMAKE_INSTALL_PREFIX}")

if (NOT DEFINED CMAKE_PREFIX_PATH)
	if (NOT "${CPP_USER_CMAKE_PREFIX_PATH}" STREQUAL "")
		message(STATUS "Setting CMAKE_PREFIX_PATH to: ${CPP_USER_CMAKE_PREFIX_PATH}")
		set(CMAKE_PREFIX_PATH "${CPP_USER_CMAKE_PREFIX_PATH}" CACHE PATH "..." FORCE)
	endif ()
endif ()

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
	if (NOT "${CPP_USER_CMAKE_INSTALL_PREFIX}" STREQUAL "")
		message(STATUS "Setting CMAKE_INSTALL_PREFIX to: ${CPP_USER_CMAKE_INSTALL_PREFIX}")
		set(CMAKE_INSTALL_PREFIX "${CPP_USER_CMAKE_INSTALL_PREFIX}" CACHE PATH "..." FORCE)
	endif ()
endif()

message(STATUS "CMAKE_PREFIX_PATH:    ${CMAKE_PREFIX_PATH}")
message(STATUS "CMAKE_INSTALL_PREFIX: ${CMAKE_INSTALL_PREFIX}")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
