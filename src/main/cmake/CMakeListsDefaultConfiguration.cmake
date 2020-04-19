
#################################### OPTIONS ##################################

OPTION(BALAU_ENABLE_ZLIB "Enable ZLib library wrappers for gzip support" ON)
OPTION(BALAU_ENABLE_ZIP  "Enable LibZip library wrappers Zipper and Unzipper" ON)
OPTION(BALAU_ENABLE_HTTP "Enable HTTP components (requires Boost version 1.68.0 or greater)" ON)
OPTION(BALAU_ENABLE_CURL "Enable use of Curl library" ON)
OPTION(BALAU_ENABLE_THREAD_LOCAL_LOGGING_ALLOCATOR "Enable thread local logging allocator" OFF)
OPTION(BALAU_ENABLE_TRACE_LOGGING "Enable trace logging in ASIO" OFF)

OPTION(ENABLE_BACKTRACE "Enable use of backtrace library" ON)
OPTION(ENABLE_OLD_ABI "Enable use of old GLIBCXX ABI" OFF)
OPTION(ENABLE_BOOST_STATIC_LIBRARIES "Force use of Boost static libraries" OFF)

OPTION(ENABLE_CODE_COVERAGE "Enable code coverage compiler flags" OFF)

if (ENABLE_OLD_ABI)
	ADD_DEFINITIONS("-D_GLIBCXX_USE_CXX11_ABI=0")
	message(STATUS "_GLIBCXX_USE_CXX11_ABI set to 0 - compiling with old ABI")
endif ()

################################### COMPILER ##################################

# Common compiler switches used in targets.
set(BALAU_CXX_FLAGS "-fPIC -Wall -pedantic -Wextra")

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	set(BALAU_CXX_FLAGS "${BALAU_CXX_FLAGS} -Wno-missing-braces")
	set(BALAU_CXX_FLAGS "${BALAU_CXX_FLAGS} -Wno-deprecated-declarations")
	set(BALAU_CXX_FLAGS "${BALAU_CXX_FLAGS} -fno-omit-frame-pointer")
	set(BALAU_CXX_FLAGS "${BALAU_CXX_FLAGS} -Wno-unused-result")
	set(BALAU_CXX_FLAGS "${BALAU_CXX_FLAGS} -Wno-unknown-pragmas")
	set(BALAU_CXX_FLAGS "${BALAU_CXX_FLAGS} -Wno-unused-function")
	set(BALAU_CXX_FLAGS "${BALAU_CXX_FLAGS} -Wno-unused-parameter") # Required for Boost Process headers.
	set(BALAU_CXX_FLAGS "${BALAU_CXX_FLAGS} -Wno-ignored-qualifiers") # Required for Boost Beast headers.
	set(BALAU_CXX_FLAGS "${BALAU_CXX_FLAGS} -Wno-unused-variable")
endif()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	# using Clang
	set(BALAU_CXX_FLAGS "${BALAU_CXX_FLAGS} -Wno-unused-lambda-capture") # Required for Boost Process headers.
	set(BALAU_CXX_FLAGS "${BALAU_CXX_FLAGS} -Wno-unused-private-field")
	set(BALAU_CXX_FLAGS_DEBUG "${BALAU_CXX_FLAGS_DEBUG} -fstandalone-debug")

	if (CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 7.0.0)
		set(BALAU_CXX_FLAGS "${BALAU_CXX_FLAGS} -Wno-return-std-move") # Required for Boost ASIO headers.
	endif()
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	# using GCC
	set(BALAU_CXX_FLAGS "${BALAU_CXX_FLAGS} -Wno-unused-but-set-parameter")

	if (CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 7.0.0)
	set(BALAU_CXX_FLAGS "${BALAU_CXX_FLAGS} -Wno-implicit-fallthrough") # Required for Boost Locale library.
	endif()
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
	# using Intel C++
	message(FATAL_ERROR "Balau has not been tested with Intel C++.
		Please test and provide a pull request with any required changes.")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	# using Visual Studio C++
	message(FATAL_ERROR "Balau has not been ported to the Windows platform.
		Please provide a pull request with any required changes if a Windows port is required.")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${BALAU_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${BALAU_CXX_FLAGS_DEBUG} -DBALAU_DEBUG -DBALAU_STACK_TRACES -O0")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (NOT "${CMAKE_BUILD_TYPE}" STREQUAL "Release")
	set(BALAU_DEBUG ON)
endif ()

################################ CODE COVERAGE ################################

if (ENABLE_CODE_COVERAGE)
	if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
		message(STATUS "CLang code coverage enabled.")
		set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fprofile-instr-generate -fcoverage-mapping")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-instr-generate -fcoverage-mapping")
	elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
		set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --coverage -fprofile-arcs -ftest-coverage")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -fprofile-arcs -ftest-coverage")
	elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
		message(FATAL_ERROR "Balau has not configured for code coverage with Intel C++.
		Please test and provide a pull request with any required changes.")
	elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
		message(FATAL_ERROR "Balau has not configured for code coverage with the Windows platform.
		Please test and provide a pull request with any required changes.")
	endif()
endif()

############################### BOOST LIBRARIES ###############################

if (USE_BOOST_STATIC_LIBRARIES)
	set(Boost_USE_STATIC_LIBS ON)
endif ()

if (ENABLE_HTTP_COMPONENTS)
	set(MINIMUM_BOOST_VERSION 1.68.0)
else ()
	set(MINIMUM_BOOST_VERSION 1.61.0)
endif ()

set(Boost_DETAILED_FAILURE_MSG ON)

find_package(Boost ${MINIMUM_BOOST_VERSION} COMPONENTS thread chrono date_time filesystem system serialization iostreams stacktrace_backtrace)

if (Boost_STACKTRACE_BACKTRACE_FOUND)
	find_package(Backtrace)

	if (Backtrace_FOUND)
		message(STATUS "Stack traces will be available (Boost support present) and libbacktrace found.")
		find_package(Boost ${MINIMUM_BOOST_VERSION} REQUIRED COMPONENTS thread chrono date_time filesystem system serialization iostreams stacktrace_backtrace)
		set(BALAU_ENABLE_STACKTRACES ON)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBOOST_STACKTRACE_LINK -DBOOST_STACKTRACE_USE_BACKTRACE")
		set(ALL_LIBS ${ALL_LIBS} ${Boost_LIBRARIES} ${Backtrace_LIBRARIES})
		include_directories(${Backtrace_INCLUDE_DIRS})
	else ()
		message(STATUS "Stack traces could be available (Boost support present) but libbacktrace not found.")
		find_package(Boost ${MINIMUM_BOOST_VERSION} REQUIRED COMPONENTS thread chrono date_time filesystem system serialization iostreams)
		set(BALAU_ENABLE_STACKTRACES OFF)
		set(ALL_LIBS ${ALL_LIBS} ${Boost_LIBRARIES})
	endif ()

else ()
	find_package(Boost ${MINIMUM_BOOST_VERSION} REQUIRED COMPONENTS thread chrono date_time filesystem system serialization iostreams)
	message(STATUS "No stack traces will be available (no Boost support).")
	set(BALAU_ENABLE_STACKTRACES OFF)
	set(ALL_LIBS ${ALL_LIBS} ${Boost_LIBRARIES})
endif ()

include_directories(${Boost_INCLUDE_DIRS})
message(STATUS "Boost include dirs: ${Boost_INCLUDE_DIRS}")

# Boost Beast API changes after Boost 1.70.0.
if(Boost_VERSION VERSION_GREATER_EQUAL 1.70.0)
	set(BOOST_BEAST_NEW_API ON)
endif ()

# Enable HTTP components only if Boost is 1.68.0 or greater.
if(Boost_VERSION VERSION_GREATER_EQUAL 1.68.0)
	message(STATUS "Boost version ${Boost_VERSION} HTTP support available: Balau HTTP components are enabled.")
	set(BOOST_BEAST_NEW_API ON)
else ()
	message(STATUS "Boost version ${Boost_VERSION} HTTP support not available: Balau HTTP components are not enabled.")
	set(BALAU_ENABLE_HTTP OFF)
	set(BALAU_ENABLE_CURL OFF)
endif ()

##################################### ICU #####################################

#
# ###### ICU LIBRARIES #######
#
#  - Data Library:  dt/data
#      Data required by the Common and I18n libraries. There are many ways to
#      package and customize this data, but by default this is all you need.
#
#  - Common Library:  uc
#      Base library required by all other ICU libraries.
#
#  - Internationalization:  in/i18n
#      A library that contains many locale based internationalization (i18n) functions.
#
#  - ICU I/O (Unicode stdio) Library:  io
#      An optional library that provides a stdio like API with Unicode support.
#
#  - Tool Utility Library:  tu
#      An internal library that contains internal APIs that are only used by ICU's tools.
#      If you do not use ICU's tools, you do not need this library.
#

find_package(ICU 60.2 REQUIRED COMPONENTS i18n uc data)
include_directories(${ICU_INCLUDE_DIRS})
set(ALL_LIBS ${ALL_LIBS} ${ICU_LIBRARIES})
message(STATUS "ICU include dirs: ${ICU_INCLUDE_DIRS}")

############################# THREADING LIBRARIES #############################

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
set(ALL_LIBS ${ALL_LIBS} Threads::Threads)

############################ COMPRESSION LIBRARIES ############################

if (BALAU_ENABLE_ZLIB)
	find_package(ZLIB REQUIRED)
	include_directories(${ZLIB_INCLUDE_DIR})
	find_package_handle_standard_args(ZLIB DEFAULT_MSG ZLIB_LIBRARY ZLIB_INCLUDE_DIR)
	set(ALL_LIBS ${ALL_LIBS} ${ZLIB_LIBRARIES})
	message(STATUS "ZLIB include dir: ${ZLIB_INCLUDE_DIR}")
	message(STATUS "ZLIB libraries: ${ZLIB_LIBRARIES}")
else ()
	message(STATUS "ZLIB features not enabled.")
endif ()

if (BALAU_ENABLE_ZIP)
	find_path(LIBZIP_INCLUDE_DIR zip.h zipconf.h)
	find_library(LIBZIP_LIBRARY NAMES zip libzip)
	find_package_handle_standard_args(LIBZIP DEFAULT_MSG LIBZIP_LIBRARY LIBZIP_INCLUDE_DIR)

	if (LIBZIP_FOUND)
		include_directories(${LIBZIP_INCLUDE_DIR})
		set(ALL_LIBS ${ALL_LIBS} ${LIBZIP_LIBRARY})
		message(STATUS "LIBZIP include dir: ${LIBZIP_INCLUDE_DIR}")
		message(STATUS "LIBZIP library: ${LIBZIP_LIBRARY}")
	else ()
		message(FATAL "LibZip library not found.")
	endif ()
else ()
	message(STATUS "LibZip features not enabled.")
endif ()

################################ CURL LIBRARY #################################

if (BALAU_ENABLE_CURL)
	find_path(LIBCURL_INCLUDE_DIR curl/curl.h)
	find_library(LIBCURL_LIBRARY NAMES curl libcurl)
	find_package_handle_standard_args(LIBCURL DEFAULT_MSG LIBCURL_LIBRARY LIBCURL_INCLUDE_DIR)

	if (LIBCURL_FOUND)
		include_directories(${LIBCURL_INCLUDE_DIR})
		set(ALL_LIBS ${ALL_LIBS} ${LIBCURL_LIBRARY})
		message(STATUS "LIBCURL include dir: ${LIBCURL_INCLUDE_DIR}")
		message(STATUS "LIBCURL library: ${LIBCURL_LIBRARY}")
	else ()
		message(FATAL "LibCurl library not found.")
	endif ()
else ()
	message(STATUS "LibCurl features not enabled.")
endif ()

############################### OTHER LIBRARIES ###############################

find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})
set(ALL_LIBS ${ALL_LIBS} ${OPENSSL_LIBRARIES})
message(STATUS "OpenSSL include dir: ${OPENSSL_INCLUDE_DIR}")
message(STATUS "OpenSSL libraries: ${OPENSSL_LIBRARIES}")
set(ALL_LIBS ${ALL_LIBS} rt bz2)
set(ALL_LIBS ${ALL_LIBS} ${CMAKE_DL_LIBS})
link_directories(/usr/lib64)

################################## BACKPORTS ##################################

include(CheckIncludeFileCXX)

check_include_file_cxx(string_view STRING_VIEW_AVAILABLE)

if (STRING_VIEW_AVAILABLE)
	message(STATUS "std::string_view is available.")
else ()
	message(STATUS "std::string_view is not available. Using boost::string_view")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBALAU_USE_BOOST_STRING_VIEW")
endif ()
