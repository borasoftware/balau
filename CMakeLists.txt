##
## Balau core C++ library
##
## Copyright (C) 2008 Bora Software (contact@borasoftware.com)
##
## Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
## See the LICENSE file for the full license text.
##

##
## See src/doc/manual/NonCode/Building.bdml for information
## on building the Balau library and its dependencies.
##

cmake_minimum_required(VERSION 3.10.2)

#
# When updating the version, also update:
#  - BalauConfig.cmake
#  - BalauConfigVersion.cmake
#  - title-page.bdml
#
project(Balau VERSION 2019.5.2)

configure_file(src/doc/api/Doxyfile.in Doxyfile)

############################# CMAKE CONFIGURATION #############################

#
# Disallow in-source builds
#
if ("${PROJECT_BINARY_DIR}" STREQUAL "${PROJECT_SOURCE_DIR}")
	message(FATAL_ERROR
		"
No in-source builds please."
		"
  mkdir build; cd build; cmake .."
		"
Please first remove the files created by this failed run with"
		"
  rm -rf CMakeCache.txt CMakeFiles"
	)
endif ()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

############################## USER CONFIGURATION #############################

#
# Default user prefix path, install path, include and lib directories if not already set.
#
# By default, the project uses the user's $ENV{HOME}/usr directory.
#
#   CMAKE_PREFIX_PATH    : Used by the FIND_XXX() commands (default = $ENV{HOME}/usr)
#   CMAKE_INSTALL_PREFIX : Location to install the library (default = $ENV{HOME}/usr)
#   USER_INCLUDE_DIR     : Additional include directories  (default = $ENV{HOME}/usr/include)
#   USER_LIB_DIR         : Additional library directories  (default = $ENV{HOME}/usr/lib)
#

if (NOT DEFINED CMAKE_PREFIX_PATH)
	set(CMAKE_PREFIX_PATH $ENV{HOME}/usr)
endif ()

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
	set(CMAKE_INSTALL_PREFIX "$ENV{HOME}/usr" CACHE PATH "..." FORCE)
endif()

if (NOT DEFINED USER_INCLUDE_DIR)
	set(USER_INCLUDE_DIR $ENV{HOME}/usr/include)
endif ()

if (NOT DEFINED USER_LIB_DIR)
	set(USER_LIB_DIR $ENV{HOME}/usr/lib)
endif ()

include_directories(${USER_INCLUDE_DIR})
link_directories(${USER_LIB_DIR})

message(STATUS "CMAKE_PREFIX_PATH:    ${CMAKE_PREFIX_PATH}")
message(STATUS "CMAKE_INSTALL_PREFIX: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "USER_INCLUDE_DIR:     ${USER_INCLUDE_DIR}")
message(STATUS "USER_LIB_DIR:         ${USER_LIB_DIR}")

#
# Turn on thread local logging allocator if user has this set.
#
if (NOT DEFINED BALAU_LOGGING_THREAD_LOCAL_ALLOCATOR)
    string(TOLOWER "$ENV{BALAU_LOGGING_THREAD_LOCAL_ALLOCATOR}" BALAU_LOGGING_THREAD_LOCAL_ALLOCATOR_LOWER)
	if ("${BALAU_LOGGING_THREAD_LOCAL_ALLOCATOR_LOWER}" STREQUAL "true")
		set(BALAU_LOGGING_THREAD_LOCAL_ALLOCATOR 1)
	else ()
		set(BALAU_LOGGING_THREAD_LOCAL_ALLOCATOR 0)
	endif ()
else ()
	string(TOLOWER "${BALAU_LOGGING_THREAD_LOCAL_ALLOCATOR}" BALAU_LOGGING_THREAD_LOCAL_ALLOCATOR_LOWER)
	if ("${BALAU_LOGGING_THREAD_LOCAL_ALLOCATOR_LOWER}" STREQUAL "true")
		set(BALAU_LOGGING_THREAD_LOCAL_ALLOCATOR 1)
	else ()
		set(BALAU_LOGGING_THREAD_LOCAL_ALLOCATOR 0)
	endif ()
endif ()

message(STATUS "Thread local logging allocator: ${BALAU_LOGGING_THREAD_LOCAL_ALLOCATOR}")

#
# Balau test app variables
#

add_definitions("-DBALAU_TEST_EMAIL_HOST_=$ENV{BALAU_TEST_EMAIL_HOST}")
add_definitions("-DBALAU_TEST_EMAIL_PORT_=$ENV{BALAU_TEST_EMAIL_PORT}")
add_definitions("-DBALAU_TEST_EMAIL_SENDER_USER_=$ENV{BALAU_TEST_EMAIL_SENDER_USER}")
add_definitions("-DBALAU_TEST_EMAIL_SENDER_PASSWORD_=$ENV{BALAU_TEST_EMAIL_SENDER_PASSWORD}")
add_definitions("-DBALAU_TEST_EMAIL_TO_=$ENV{BALAU_TEST_EMAIL_TO}")

################################## DEFINITIONS ################################

add_definitions("-DBALAU_VERSION_=${PROJECT_VERSION}")
add_definitions("-DBALAU_BASE_FOLDER_=${CMAKE_SOURCE_DIR}")
add_definitions("-DBALAU_SOURCE_FOLDER_=${CMAKE_SOURCE_DIR}/src")
add_definitions("-DBALAU_SOURCE_MAIN_FOLDER_=${CMAKE_SOURCE_DIR}/src/main")
add_definitions("-DBALAU_SOURCE_TEST_FOLDER_=${CMAKE_SOURCE_DIR}/src/test")
add_definitions("-DBALAU_SOURCE_TEST_RESOURCES_FOLDER_=${CMAKE_SOURCE_DIR}/src/test/resources")
add_definitions("-DBALAU_TARGET_FOLDER_=${CMAKE_BINARY_DIR}")
add_definitions("-DBALAU_TEST_RESULTS_FOLDER_=${CMAKE_BINARY_DIR}/testResults")

add_definitions("-DBALAU_LOGGING_THREAD_LOCAL_ALLOCATOR=${BALAU_LOGGING_THREAD_LOCAL_ALLOCATOR}")

if (DEFINED BALAU_LOGGING_THREAD_LOCAL_ALLOCATOR_BUFFER_SIZE_KB)
	add_definitions("-DBALAU_LOGGING_THREAD_LOCAL_ALLOCATOR_BUFFER_SIZE_KB=${BALAU_LOGGING_THREAD_LOCAL_ALLOCATOR_BUFFER_SIZE_KB}")
endif ()

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/testResults)

include_directories(src/main/cpp)
include_directories(${CMAKE_BINARY_DIR})

################################### COMPILER ##################################

#
# Common compiler switches used in targets.
#
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
	set(BALAU_CXX_FLAGS "${BALAU_CXX_FLAGS} -Wno-return-std-move") # Required for Boost ASIO headers.
endif()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	# using Clang
	set(BALAU_CXX_FLAGS "${BALAU_CXX_FLAGS} -Wno-unused-lambda-capture") # Required for Boost Process headers.
	set(BALAU_CXX_FLAGS "${BALAU_CXX_FLAGS} -Wno-unused-private-field")
	set(BALAU_CXX_FLAGS_DEBUG "${BALAU_CXX_FLAGS_DEBUG} -fstandalone-debug")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	# using GCC
	set(BALAU_CXX_FLAGS "${BALAU_CXX_FLAGS} -Wno-unused-but-set-parameter")
	set(BALAU_CXX_FLAGS "${BALAU_CXX_FLAGS} -Wno-implicit-fallthrough") # Required for Boost Locale library.
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
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${BALAU_CXX_FLAGS_DEBUG} -DBALAU_DEBUG -DBALAU_STACK_TRACES -O0 -g3")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")


#
# Turn on trace logging if BALAU_TRACE is defined.
#
if (BALAU_TRACE)
	message(STATUS "BALAU_TRACE: trace logging turned on.")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DBOOST_ASIO_ENABLE_HANDLER_TRACKING")
endif ()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

############################### BOOST LIBRARIES ###############################

set(Boost_DETAILED_FAILURE_MSG ON)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

if (CMAKE_BUILD_TYPE MATCHES Debug)
	find_package(Boost 1.68.0 COMPONENTS thread chrono date_time filesystem system serialization iostreams stacktrace_backtrace)

	if (Boost_STACKTRACE_BACKTRACE_FOUND)
		find_package(Boost 1.68.0 REQUIRED COMPONENTS thread chrono date_time filesystem system serialization iostreams stacktrace_backtrace)
		message(STATUS "DEBUG MODE: adding libbacktrace to link libraries.")
		set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DBOOST_STACKTRACE_LINK -DBOOST_STACKTRACE_USE_BACKTRACE")
		set(ALL_LIBS ${ALL_LIBS} ${Boost_LIBRARIES} backtrace)
	else ()
		find_package(Boost 1.68.0 REQUIRED COMPONENTS thread chrono date_time filesystem system serialization iostreams)
		set(ALL_LIBS ${ALL_LIBS} ${Boost_LIBRARIES})
	endif ()
else ()
	find_package(Boost 1.68.0 REQUIRED COMPONENTS thread chrono date_time filesystem system serialization iostreams)
	message(STATUS "Release mode: no stack traces will be available.")
	set(ALL_LIBS ${ALL_LIBS} ${Boost_LIBRARIES})
endif ()

include_directories(${Boost_INCLUDE_DIRS})
message(STATUS "Boost include dirs: ${Boost_INCLUDE_DIRS}")

#
# Undefine this in end code before including any Balau headers in order to
# print relative paths in stack traces instead of just the filename.
#
add_definitions("-DBALAU_STACK_TRACES_STRIP_PATH")

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

############################### OTHER LIBRARIES ###############################

find_package(ZLIB REQUIRED)
include_directories(${ZLIB_INCLUDE_DIR})
set(ALL_LIBS ${ALL_LIBS} ${ZLIB_LIBRARIES})
message(STATUS "ZLIB include dir: ${ZLIB_INCLUDE_DIR}")
message(STATUS "ZLIB libraries: ${ZLIB_LIBRARIES}")

find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})
set(ALL_LIBS ${ALL_LIBS} ${OPENSSL_LIBRARIES})
message(STATUS "OpenSSL include dir: ${OPENSSL_INCLUDE_DIR}")
message(STATUS "OpenSSL libraries: ${OPENSSL_LIBRARIES}")

# TODO
#find_package(LIBZIP REQUIRED)
#include_directories(${LIBZIP_INCLUDE_DIR})
#set(ALL_LIBS ${ALL_LIBS} ${LIBZIP_LIBRARIES})
#message(STATUS "LIBZIP include dir: ${LIBZIP_INCLUDE_DIR}")
#message(STATUS "LIBZIP libraries: ${LIBZIP_LIBRARIES}")

set(ALL_LIBS ${ALL_LIBS} rt zip curl)
set(ALL_LIBS ${ALL_LIBS} ${CMAKE_DL_LIBS})

################################## BACKPORTS ##################################

include(CheckIncludeFileCXX)

check_include_file_cxx(string_view STRING_VIEW_AVAILABLE)

if (STRING_VIEW_AVAILABLE)
	message(STATUS "std::string_view is available. Setting -DBALAU_STD_STRING_VIEW_AVAILABLE")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBALAU_STD_STRING_VIEW_AVAILABLE")
else ()
	message(STATUS "std::string_view is not available. Using boost::string_view")
endif ()

############################### STATUS PRINTOUT ###############################

message(STATUS "Include directories:")
get_property(ALL_INCLUDE_DIRECTORIES DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)

foreach (INCLUDE_DIRECTORY ${ALL_INCLUDE_DIRECTORIES})
	message(STATUS "    ${INCLUDE_DIRECTORY}")
endforeach ()

message(STATUS "Libraries:")

foreach (LIB ${ALL_LIBS})
	message(STATUS "    ${LIB}")
endforeach ()

message(STATUS "BALAU_CXX_FLAGS: ${BALAU_CXX_FLAGS}")

################################# BALAU LIBRARY ###############################

set(THIRD_PARTY_SOURCE_FILES

	src/main/cpp/Balau/ThirdParty/Boost/Beast/Http/BasicFileBody.hpp

	src/main/cpp/Balau/ThirdParty/Date/date.hpp

	src/main/cpp/Balau/ThirdParty/HashLibrary/crc32.cpp
	src/main/cpp/Balau/ThirdParty/HashLibrary/crc32.h
	src/main/cpp/Balau/ThirdParty/HashLibrary/hash.h
	src/main/cpp/Balau/ThirdParty/HashLibrary/hmac.h
	src/main/cpp/Balau/ThirdParty/HashLibrary/keccak.cpp
	src/main/cpp/Balau/ThirdParty/HashLibrary/keccak.h
	src/main/cpp/Balau/ThirdParty/HashLibrary/md5.cpp
	src/main/cpp/Balau/ThirdParty/HashLibrary/md5.h
	src/main/cpp/Balau/ThirdParty/HashLibrary/sha1.cpp
	src/main/cpp/Balau/ThirdParty/HashLibrary/sha1.h
	src/main/cpp/Balau/ThirdParty/HashLibrary/sha256.cpp
	src/main/cpp/Balau/ThirdParty/HashLibrary/sha256.h
	src/main/cpp/Balau/ThirdParty/HashLibrary/sha3.cpp
	src/main/cpp/Balau/ThirdParty/HashLibrary/sha3.h
)

set(BALAU_SOURCE_FILES
	src/main/cpp/Balau/Application/BindingBuilder.hpp
	src/main/cpp/Balau/Application/CommandLine.hpp
	src/main/cpp/Balau/Application/Injectable.hpp
	src/main/cpp/Balau/Application/Injector.hpp
	src/main/cpp/Balau/Application/ApplicationConfiguration.hpp
	src/main/cpp/Balau/Application/EnvironmentConfiguration.hpp
	src/main/cpp/Balau/Application/EnvironmentProperties.hpp
	src/main/cpp/Balau/Application/InjectorConfiguration.hpp
	src/main/cpp/Balau/Application/Impl/Binding.hpp
	src/main/cpp/Balau/Application/Impl/BindingKey.hpp
	src/main/cpp/Balau/Application/Impl/BindingMap.hpp
	src/main/cpp/Balau/Application/Impl/EnvironmentConfigurationBuilder.hpp
	src/main/cpp/Balau/Application/Impl/EnvironmentConfigurationBuilderUtils.hpp
	src/main/cpp/Balau/Application/Impl/InjectorLogger.cpp
	src/main/cpp/Balau/Application/Impl/InjectorLogger.hpp
	src/main/cpp/Balau/Application/Impl/InjectorMacros.hpp
	src/main/cpp/Balau/Application/Impl/PropertyBindingBuilderFactory.cpp
	src/main/cpp/Balau/Application/Impl/PropertyBindingBuilderFactory.hpp
	src/main/cpp/Balau/Application/Impl/PropertyString.hpp
	src/main/cpp/Balau/Application/Impl/PropertyTypeSpecificationVisitor.cpp
	src/main/cpp/Balau/Application/Impl/PropertyTypeSpecificationVisitor.hpp
	src/main/cpp/Balau/Documentation/ConfigurationWebApp.cpp
	src/main/cpp/Balau/Documentation/ConfigurationWebApp.hpp
	src/main/cpp/Balau/Concurrent/CyclicBarrier.cpp
	src/main/cpp/Balau/Concurrent/CyclicBarrier.hpp
	src/main/cpp/Balau/Concurrent/Fork.hpp
	src/main/cpp/Balau/Concurrent/LazyValue.hpp
	src/main/cpp/Balau/Concurrent/Semaphore.hpp
	src/main/cpp/Balau/Concurrent/SingleTimeExecutor.hpp
	src/main/cpp/Balau/Concurrent/ThreadLocalInstance.hpp
	src/main/cpp/Balau/Container/ArrayBlockingQueue.hpp
	src/main/cpp/Balau/Container/BlockingQueue.hpp
	src/main/cpp/Balau/Container/DependencyGraph.hpp
	src/main/cpp/Balau/Container/ObjectTrie.hpp
	src/main/cpp/Balau/Container/SynchronizedQueue.hpp
	src/main/cpp/Balau/Container/Queue.hpp
	src/main/cpp/Balau/Container/Impl/ContainerLogger.cpp
	src/main/cpp/Balau/Container/Impl/ContainerLogger.hpp
	src/main/cpp/Balau/Dev/Assert.hpp
	src/main/cpp/Balau/Exception/BalauException.hpp
	src/main/cpp/Balau/Exception/ContainerExceptions.hpp
	src/main/cpp/Balau/Exception/CommandLineExceptions.hpp
	src/main/cpp/Balau/Exception/InjectorExceptions.hpp
	src/main/cpp/Balau/Exception/IOExceptions.hpp
	src/main/cpp/Balau/Exception/LoggingExceptions.hpp
	src/main/cpp/Balau/Exception/NetworkExceptions.hpp
	src/main/cpp/Balau/Exception/ParsingExceptions.hpp
	src/main/cpp/Balau/Exception/ResourceExceptions.hpp
	src/main/cpp/Balau/Exception/SystemExceptions.hpp
	src/main/cpp/Balau/Exception/TestExceptions.hpp
	src/main/cpp/Balau/Interprocess/MSharedMemoryObject.hpp
	src/main/cpp/Balau/Interprocess/USharedMemoryObject.hpp
	src/main/cpp/Balau/Interprocess/SharedMemoryQueue.hpp
	src/main/cpp/Balau/Interprocess/SharedMemoryUtils.cpp
	src/main/cpp/Balau/Interprocess/SharedMemoryUtils.hpp
	src/main/cpp/Balau/Interprocess/Impl/SharedMemoryQueueImpl.cpp
	src/main/cpp/Balau/Interprocess/Impl/SharedMemoryQueueImpl.hpp
	src/main/cpp/Balau/Lang/Common/AbstractScanner.hpp
	src/main/cpp/Balau/Lang/Common/CodeSpan.hpp
	src/main/cpp/Balau/Lang/Common/ScannedTokens.hpp
	src/main/cpp/Balau/Lang/Property/PropertyParserService.hpp
	src/main/cpp/Balau/Lang/Property/Lang/PropertyAst.hpp
	src/main/cpp/Balau/Lang/Property/Lang/PropertyToken.cpp
	src/main/cpp/Balau/Lang/Property/Lang/PropertyToken.hpp
	src/main/cpp/Balau/Lang/Property/Parser/PropertyParser.cpp
	src/main/cpp/Balau/Lang/Property/Parser/PropertyParser.hpp
	src/main/cpp/Balau/Lang/Property/Parser/PropertyScanner.cpp
	src/main/cpp/Balau/Lang/Property/Parser/PropertyScanner.hpp
	src/main/cpp/Balau/Lang/Property/Util/PropertyAstToString.hpp
	src/main/cpp/Balau/Lang/Property/Util/PropertyVisitor.hpp
	src/main/cpp/Balau/Logging/Logger.cpp
	src/main/cpp/Balau/Logging/Logger.hpp
	src/main/cpp/Balau/Logging/LoggingLevel.hpp
	src/main/cpp/Balau/Logging/Impl/BalauLogger.cpp
	src/main/cpp/Balau/Logging/Impl/BalauLogger.hpp
	src/main/cpp/Balau/Logging/Impl/LoggerAllocator.cpp
	src/main/cpp/Balau/Logging/Impl/LoggerAllocator.hpp
	src/main/cpp/Balau/Logging/Impl/LoggerConfigurationVisitor.hpp
	src/main/cpp/Balau/Logging/Impl/LoggerForwardDeclarations.hpp
	src/main/cpp/Balau/Logging/Impl/LoggerHolder.cpp
	src/main/cpp/Balau/Logging/Impl/LoggerHolder.hpp
	src/main/cpp/Balau/Logging/Impl/LoggerItemParameters.hpp
	src/main/cpp/Balau/Logging/Impl/LoggerItems.cpp
	src/main/cpp/Balau/Logging/Impl/LoggerItems.hpp
	src/main/cpp/Balau/Logging/Impl/LoggerMessageItems.hpp
	src/main/cpp/Balau/Logging/Impl/LoggerPropertyVisitor.hpp
	src/main/cpp/Balau/Logging/Impl/LoggingState.cpp
	src/main/cpp/Balau/Logging/Impl/LoggingState.hpp
	src/main/cpp/Balau/Logging/Impl/LoggingStreams.cpp
	src/main/cpp/Balau/Logging/Impl/LoggingStreams.hpp
	src/main/cpp/Balau/Network/Http/Client/HttpClient.cpp
	src/main/cpp/Balau/Network/Http/Client/HttpClient.hpp
	src/main/cpp/Balau/Network/Http/Client/HttpsClient.hpp
	src/main/cpp/Balau/Network/Http/Client/WsClient.hpp
	src/main/cpp/Balau/Network/Http/Server/HttpRequest.hpp
	src/main/cpp/Balau/Network/Http/Server/HttpResponse.hpp
	src/main/cpp/Balau/Network/Http/Server/HttpServer.cpp
	src/main/cpp/Balau/Network/Http/Server/HttpServer.hpp
	src/main/cpp/Balau/Network/Http/Server/HttpServerConfiguration.hpp
	src/main/cpp/Balau/Network/Http/Server/HttpSession.cpp
	src/main/cpp/Balau/Network/Http/Server/HttpSession.hpp
	src/main/cpp/Balau/Network/Http/Server/HttpWebApp.cpp
	src/main/cpp/Balau/Network/Http/Server/HttpWebApp.hpp
	src/main/cpp/Balau/Network/Http/Server/NetworkTypes.hpp
	src/main/cpp/Balau/Network/Http/Server/WsSession.hpp
	src/main/cpp/Balau/Network/Http/Server/WsWebApp.hpp
	src/main/cpp/Balau/Network/Http/Server/HttpWebApps/CannedHttpWebApp.cpp
	src/main/cpp/Balau/Network/Http/Server/HttpWebApps/CannedHttpWebApp.hpp
	src/main/cpp/Balau/Network/Http/Server/HttpWebApps/EmailSendingHttpWebApp.cpp
	src/main/cpp/Balau/Network/Http/Server/HttpWebApps/EmailSendingHttpWebApp.hpp
	src/main/cpp/Balau/Network/Http/Server/HttpWebApps/FailingHttpWebApp.cpp
	src/main/cpp/Balau/Network/Http/Server/HttpWebApps/FailingHttpWebApp.hpp
	src/main/cpp/Balau/Network/Http/Server/HttpWebApps/FileServingHttpWebApp.cpp
	src/main/cpp/Balau/Network/Http/Server/HttpWebApps/FileServingHttpWebApp.hpp
	src/main/cpp/Balau/Network/Http/Server/HttpWebApps/RedirectingHttpWebApp.cpp
	src/main/cpp/Balau/Network/Http/Server/HttpWebApps/RedirectingHttpWebApp.hpp
	src/main/cpp/Balau/Network/Http/Server/HttpWebApps/RoutingHttpWebApp.cpp
	src/main/cpp/Balau/Network/Http/Server/HttpWebApps/RoutingHttpWebApp.hpp
	src/main/cpp/Balau/Network/Http/Server/HttpWebApps/Impl/CurlEmailSender.cpp
	src/main/cpp/Balau/Network/Http/Server/HttpWebApps/Impl/CurlEmailSender.hpp
	src/main/cpp/Balau/Network/Http/Server/HttpWebApps/Impl/CurlInitializer.cpp
	src/main/cpp/Balau/Network/Http/Server/HttpWebApps/Impl/CurlInitializer.hpp
	src/main/cpp/Balau/Network/Http/Server/ClientSession.hpp
	src/main/cpp/Balau/Network/Http/Server/Impl/ClientSessions.hpp
	src/main/cpp/Balau/Network/Http/Server/Impl/HttpSessions.hpp
	src/main/cpp/Balau/Network/Http/Server/Impl/HttpWebAppFactory.hpp
	src/main/cpp/Balau/Network/Http/Server/Impl/Listener.hpp
	src/main/cpp/Balau/Network/Http/Server/WsWebApps/EchoingWsWebApp.cpp
	src/main/cpp/Balau/Network/Http/Server/WsWebApps/EchoingWsWebApp.hpp
	src/main/cpp/Balau/Network/Http/Server/WsWebApps/RoutingWsWebApp.cpp
	src/main/cpp/Balau/Network/Http/Server/WsWebApps/RoutingWsWebApp.hpp
	src/main/cpp/Balau/Network/Utilities/MimeTypes.cpp
	src/main/cpp/Balau/Network/Utilities/MimeTypes.hpp
	src/main/cpp/Balau/Network/Utilities/root_certificates.hpp
	src/main/cpp/Balau/Network/Utilities/UrlDecode.hpp
	src/main/cpp/Balau/Resource/ByteReadResource.hpp
	src/main/cpp/Balau/Resource/ByteWriteResource.hpp
	src/main/cpp/Balau/Resource/File.hpp
	src/main/cpp/Balau/Resource/FileByteReadResource.cpp
	src/main/cpp/Balau/Resource/FileByteReadResource.hpp
	src/main/cpp/Balau/Resource/FileByteWriteResource.cpp
	src/main/cpp/Balau/Resource/FileByteWriteResource.hpp
	src/main/cpp/Balau/Resource/FileUtf32To8WriteResource.cpp
	src/main/cpp/Balau/Resource/FileUtf32To8WriteResource.hpp
	src/main/cpp/Balau/Resource/FileUtf8To32ReadResource.cpp
	src/main/cpp/Balau/Resource/FileUtf8To32ReadResource.hpp
	src/main/cpp/Balau/Resource/Http.hpp
	src/main/cpp/Balau/Resource/HttpByteReadResource.cpp
	src/main/cpp/Balau/Resource/HttpByteReadResource.hpp
	src/main/cpp/Balau/Resource/Https.hpp
	src/main/cpp/Balau/Resource/HttpsByteReadResource.cpp
	src/main/cpp/Balau/Resource/HttpsByteReadResource.hpp
	src/main/cpp/Balau/Resource/HttpsUtf8To32ReadResource.cpp
	src/main/cpp/Balau/Resource/HttpsUtf8To32ReadResource.hpp
	src/main/cpp/Balau/Resource/HttpUtf8To32ReadResource.cpp
	src/main/cpp/Balau/Resource/HttpUtf8To32ReadResource.hpp
	src/main/cpp/Balau/Resource/Resource.hpp
	src/main/cpp/Balau/Resource/StringUri.hpp
	src/main/cpp/Balau/Resource/StringUriByteReadResource.cpp
	src/main/cpp/Balau/Resource/StringUriByteReadResource.hpp
	src/main/cpp/Balau/Resource/StringUriByteWriteResource.cpp
	src/main/cpp/Balau/Resource/StringUriByteWriteResource.hpp
	src/main/cpp/Balau/Resource/StringUriUtf8To32ReadResource.cpp
	src/main/cpp/Balau/Resource/StringUriUtf8To32ReadResource.hpp
	src/main/cpp/Balau/Resource/StringUriUtf32To8WriteResource.cpp
	src/main/cpp/Balau/Resource/StringUriUtf32To8WriteResource.hpp
	src/main/cpp/Balau/Resource/Uri.cpp
	src/main/cpp/Balau/Resource/Uri.hpp
	src/main/cpp/Balau/Resource/UriComponents.cpp
	src/main/cpp/Balau/Resource/UriComponents.hpp
	src/main/cpp/Balau/Resource/UriDispatcher.hpp
	src/main/cpp/Balau/Resource/Url.hpp
	src/main/cpp/Balau/Resource/Utf32To8WriteResource.hpp
	src/main/cpp/Balau/Resource/Utf8To32ReadResource.hpp
	src/main/cpp/Balau/Resource/ZipEntry.cpp
	src/main/cpp/Balau/Resource/ZipEntry.hpp
	src/main/cpp/Balau/Resource/ZipEntryByteReadResource.cpp
	src/main/cpp/Balau/Resource/ZipEntryByteReadResource.hpp
	src/main/cpp/Balau/Resource/ZipEntryUtf8To32ReadResource.cpp
	src/main/cpp/Balau/Resource/ZipEntryUtf8To32ReadResource.hpp
	src/main/cpp/Balau/Resource/ZipFile.hpp
	src/main/cpp/Balau/Resource/Impl/HttpSource.cpp
	src/main/cpp/Balau/Resource/Impl/HttpSource.hpp
	src/main/cpp/Balau/Resource/Impl/HttpsSource.cpp
	src/main/cpp/Balau/Resource/Impl/HttpsSource.hpp
	src/main/cpp/Balau/Resource/Impl/ZipEntrySource.cpp
	src/main/cpp/Balau/Resource/Impl/ZipEntrySource.hpp
	src/main/cpp/Balau/Serialization/SerializationMacros.hpp
	src/main/cpp/Balau/System/Clock.hpp
	src/main/cpp/Balau/System/SystemClock.hpp
	src/main/cpp/Balau/System/Sleep.hpp
	src/main/cpp/Balau/System/ThreadName.cpp
	src/main/cpp/Balau/System/ThreadName.hpp
	src/main/cpp/Balau/Testing/Assertions.hpp
	src/main/cpp/Balau/Testing/Matchers.hpp
	src/main/cpp/Balau/Testing/TestRunner.hpp
	src/main/cpp/Balau/Testing/ExecutionModel.hpp
	src/main/cpp/Balau/Testing/Writers.cpp
	src/main/cpp/Balau/Testing/Writers.hpp
	src/main/cpp/Balau/Testing/Impl/CompositeWriter.hpp
	src/main/cpp/Balau/Testing/Impl/FlattenedTestCase.hpp
	src/main/cpp/Balau/Testing/Impl/HexContainerRendererImpl.hpp
	src/main/cpp/Balau/Testing/Impl/MatcherClasses.hpp
	src/main/cpp/Balau/Testing/Impl/MultiProcessTestRunnerExecutor.hpp
	src/main/cpp/Balau/Testing/Impl/ProcessPerTestTestRunnerExecutor.hpp
	src/main/cpp/Balau/Testing/Impl/SingleThreadedTestRunnerExecutor.hpp
	src/main/cpp/Balau/Testing/Impl/StandardRenderers.hpp
	src/main/cpp/Balau/Testing/Impl/TestCase.hpp
	src/main/cpp/Balau/Testing/Impl/TestGroupBase.cpp
	src/main/cpp/Balau/Testing/Impl/TestGroupBase.hpp
	src/main/cpp/Balau/Testing/Impl/TestMethodBase.hpp
	src/main/cpp/Balau/Testing/Impl/TestResult.hpp
	src/main/cpp/Balau/Testing/Impl/TestResultQueue.hpp
	src/main/cpp/Balau/Testing/Impl/TestRunnerExecutor.hpp
	src/main/cpp/Balau/Testing/Impl/TestRunnerLimits.hpp
	src/main/cpp/Balau/Testing/Impl/WorkerProcessesTestRunnerExecutor.hpp
	src/main/cpp/Balau/Testing/Impl/WorkerThreadsTestRunnerExecutor.hpp
	src/main/cpp/Balau/Testing/Renderers/HexCharRenderer.hpp
	src/main/cpp/Balau/Testing/Renderers/HexContainerRenderer.hpp
	src/main/cpp/Balau/Testing/Renderers/HexUnsignedCharRenderer.hpp
	src/main/cpp/Balau/Testing/Renderers/HexSignedCharRenderer.hpp
	src/main/cpp/Balau/Testing/Renderers/HexUnsignedShortRenderer.hpp
	src/main/cpp/Balau/Testing/Renderers/HexSignedShortRenderer.hpp
	src/main/cpp/Balau/Testing/Renderers/HexUnsignedIntRenderer.hpp
	src/main/cpp/Balau/Testing/Renderers/HexSignedIntRenderer.hpp
	src/main/cpp/Balau/Testing/Renderers/HexUnsignedLongRenderer.hpp
	src/main/cpp/Balau/Testing/Renderers/HexSignedLongRenderer.hpp
	src/main/cpp/Balau/Testing/Renderers/HexUnsignedLongLongRenderer.hpp
	src/main/cpp/Balau/Testing/Renderers/HexSignedLongLongRenderer.hpp
	src/main/cpp/Balau/Testing/Util/NetworkTesting.cpp
	src/main/cpp/Balau/Testing/Util/NetworkTesting.hpp
	src/main/cpp/Balau/Type/Character.hpp
	src/main/cpp/Balau/Type/FromString.hpp
	src/main/cpp/Balau/Type/MovableOnScopeExit.hpp
	src/main/cpp/Balau/Type/OnScopeExit.hpp
	src/main/cpp/Balau/Type/StdTypes.cpp
	src/main/cpp/Balau/Type/StdTypes.hpp
	src/main/cpp/Balau/Type/ToString.hpp
	src/main/cpp/Balau/Type/ToStringA.hpp
	src/main/cpp/Balau/Type/ToStringS.hpp
	src/main/cpp/Balau/Type/UUID.hpp
	src/main/cpp/Balau/Type/Impl/FromStringImpl.hpp
	src/main/cpp/Balau/Type/Impl/ToStringImpl.hpp
	src/main/cpp/Balau/Util/Compression.cpp
	src/main/cpp/Balau/Util/Compression.hpp
	src/main/cpp/Balau/Util/Containers.hpp
	src/main/cpp/Balau/Util/DateTime.hpp
	src/main/cpp/Balau/Util/Enums.hpp
	src/main/cpp/Balau/Util/Files.hpp
	src/main/cpp/Balau/Util/Hashing.hpp
	src/main/cpp/Balau/Util/Macros.hpp
	src/main/cpp/Balau/Util/Memory.hpp
	src/main/cpp/Balau/Util/PrettyPrint.cpp
	src/main/cpp/Balau/Util/PrettyPrint.hpp
	src/main/cpp/Balau/Util/Random.hpp
	src/main/cpp/Balau/Util/Streams.hpp
	src/main/cpp/Balau/Util/Strings.hpp
	src/main/cpp/Balau/Util/User.hpp
	src/main/cpp/Balau/Util/Vectors.hpp
	src/main/cpp/Balau/Util/Impl/RandomImpl.hpp
	src/main/cpp/Balau/Util/Impl/MacrosImpl.hpp

	src/main/cmake/BalauConfig.cmake
	src/main/cmake/BalauConfigVersion.cmake

	src/doc
)

add_library(Balau STATIC ${THIRD_PARTY_SOURCE_FILES} ${BALAU_SOURCE_FILES})

############################# BALAU DOCUMENTATION #############################

#
# This target requires doxygen.
#
add_custom_target(
	BalauApiDoc
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
	COMMAND doxygen ${CMAKE_BINARY_DIR}/Doxyfile
	COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/${PROJECT_VERSION}/bdml
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/src/doc/bdml ${CMAKE_BINARY_DIR}/${PROJECT_VERSION}/bdml
	COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/${PROJECT_VERSION}/resources
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/src/doc/resources ${CMAKE_BINARY_DIR}/${PROJECT_VERSION}/resources
	COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/${PROJECT_VERSION}/api
	COMMAND ${CMAKE_COMMAND} -E rename ${CMAKE_BINARY_DIR}/api ${CMAKE_BINARY_DIR}/${PROJECT_VERSION}/api
)

#
# This target requires xsltproc.
#
add_custom_target(
	BalauManual
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
	COMMAND sh ../src/doc/scripts/makeHtml.sh ${PROJECT_VERSION}
)

############################ INSTALLABLE FILE LISTS ###########################

install(DIRECTORY src/main/cpp/Balau/ DESTINATION "include/Balau-${PROJECT_VERSION}/Balau" FILES_MATCHING PATTERN "*.hpp")
install(TARGETS Balau DESTINATION "lib/Balau-${PROJECT_VERSION}")
install(FILES src/main/cmake/BalauConfig.cmake DESTINATION "lib/Balau-${PROJECT_VERSION}")
install(FILES src/main/cmake/BalauConfigVersion.cmake DESTINATION "lib/Balau-${PROJECT_VERSION}")
install(DIRECTORY src/main/resources/BalauConfig/ DESTINATION "include/Balau-${PROJECT_VERSION}/BalauConfig" FILES_MATCHING PATTERN "*.thconf")

################################# BALAU TESTS #################################

set(BALAU_TESTS_SOURCE_FILES
	src/test/resources

	src/test/cpp/TestResources.cpp
	src/test/cpp/TestResources.hpp
	src/test/cpp/TestMain.cpp

	src/test/cpp/Balau/Application/CommandLineTest.cpp
	src/test/cpp/Balau/Application/CommandLineTest.hpp
	src/test/cpp/Balau/Application/EnvironmentConfigurationTest.cpp
	src/test/cpp/Balau/Application/EnvironmentConfigurationTest.hpp
	src/test/cpp/Balau/Application/InjectorTest.cpp
	src/test/cpp/Balau/Application/InjectorTest.hpp
	src/test/cpp/Balau/Application/ToStringTest.cpp
	src/test/cpp/Balau/Application/ToStringTest.hpp
	src/test/cpp/Balau/Application/Impl/EnvironmentConfigurationBuilderTest.cpp
	src/test/cpp/Balau/Application/Impl/EnvironmentConfigurationBuilderTest.hpp
	src/test/cpp/Balau/Documentation/ConfigurationWebAppTest.cpp
	src/test/cpp/Balau/Documentation/ConfigurationWebAppTest.hpp
	src/test/cpp/Balau/Container/ArrayBlockingQueueTest.cpp
	src/test/cpp/Balau/Container/ArrayBlockingQueueTest.hpp
	src/test/cpp/Balau/Container/DependencyGraphTest.cpp
	src/test/cpp/Balau/Container/DependencyGraphTest.hpp
	src/test/cpp/Balau/Container/ObjectTrieTest.cpp
	src/test/cpp/Balau/Container/ObjectTrieTest.hpp
	src/test/cpp/Balau/Interprocess/SharedMemoryQueueTest.cpp
	src/test/cpp/Balau/Interprocess/SharedMemoryQueueTest.hpp
	src/test/cpp/Balau/Lang/Common/ScannedTokensTest.cpp
	src/test/cpp/Balau/Lang/Common/ScannedTokensTest.hpp
	src/test/cpp/Balau/Lang/Property/Parser/PropertyParserTest.cpp
	src/test/cpp/Balau/Lang/Property/Parser/PropertyParserTest.hpp
	src/test/cpp/Balau/Logging/LoggerTest.cpp
	src/test/cpp/Balau/Logging/LoggerTest.hpp
	src/test/cpp/Balau/Logging/Impl/LoggingStreamsTest.cpp
	src/test/cpp/Balau/Logging/Impl/LoggingStreamsTest.hpp
	src/test/cpp/Balau/Network/Http/Client/HttpClientTest.cpp
	src/test/cpp/Balau/Network/Http/Client/HttpClientTest.hpp
	src/test/cpp/Balau/Network/Http/Client/HttpsClientTest.cpp
	src/test/cpp/Balau/Network/Http/Client/HttpsClientTest.hpp
	src/test/cpp/Balau/Network/Http/Server/HttpServerTest.cpp
	src/test/cpp/Balau/Network/Http/Server/HttpServerTest.hpp
	src/test/cpp/Balau/Network/Http/Server/HttpWebApps/FileServingHttpWebAppTest.cpp
	src/test/cpp/Balau/Network/Http/Server/HttpWebApps/FileServingHttpWebAppTest.hpp
	src/test/cpp/Balau/Network/Http/Server/HttpWebApps/EmailSendingHttpWebAppTest.cpp
	src/test/cpp/Balau/Network/Http/Server/HttpWebApps/EmailSendingHttpWebAppTest.hpp
	src/test/cpp/Balau/Network/Http/Server/HttpWebApps/RedirectingHttpWebAppTest.cpp
	src/test/cpp/Balau/Network/Http/Server/HttpWebApps/RedirectingHttpWebAppTest.hpp
	src/test/cpp/Balau/Network/Http/Server/HttpWebApps/RoutingHttpWebAppTest.cpp
	src/test/cpp/Balau/Network/Http/Server/HttpWebApps/RoutingHttpWebAppTest.hpp
	src/test/cpp/Balau/Network/Http/Server/WsWebApps/ChatWsWebAppTest.cpp
	src/test/cpp/Balau/Network/Http/Server/WsWebApps/ChatWsWebAppTest.hpp
	src/test/cpp/Balau/Network/Http/Server/WsWebApps/EchoingWsWebAppTest.cpp
	src/test/cpp/Balau/Network/Http/Server/WsWebApps/EchoingWsWebAppTest.hpp
	src/test/cpp/Balau/Network/Utilities/UrlDecodeTest.cpp
	src/test/cpp/Balau/Network/Utilities/UrlDecodeTest.hpp
	src/test/cpp/Balau/Resource/FileByteReadResourceTest.cpp
	src/test/cpp/Balau/Resource/FileByteReadResourceTest.hpp
	src/test/cpp/Balau/Resource/FileByteWriteResourceTest.cpp
	src/test/cpp/Balau/Resource/FileByteWriteResourceTest.hpp
	src/test/cpp/Balau/Resource/FileTest.cpp
	src/test/cpp/Balau/Resource/FileTest.hpp
	src/test/cpp/Balau/Resource/FileUtf32To8WriteResourceTest.cpp
	src/test/cpp/Balau/Resource/FileUtf32To8WriteResourceTest.hpp
	src/test/cpp/Balau/Resource/FileUtf8To32ReadResourceTest.cpp
	src/test/cpp/Balau/Resource/FileUtf8To32ReadResourceTest.hpp
	src/test/cpp/Balau/Resource/HttpByteReadResourceTest.cpp
	src/test/cpp/Balau/Resource/HttpByteReadResourceTest.hpp
	src/test/cpp/Balau/Resource/HttpsByteReadResourceTest.cpp
	src/test/cpp/Balau/Resource/HttpsByteReadResourceTest.hpp
	src/test/cpp/Balau/Resource/HttpsUtf8To32ReadResourceTest.cpp
	src/test/cpp/Balau/Resource/HttpsUtf8To32ReadResourceTest.hpp
	src/test/cpp/Balau/Resource/HttpUtf8To32ReadResourceTest.cpp
	src/test/cpp/Balau/Resource/HttpUtf8To32ReadResourceTest.hpp
	src/test/cpp/Balau/Resource/StringByteReadResourceTest.cpp
	src/test/cpp/Balau/Resource/StringByteReadResourceTest.hpp
	src/test/cpp/Balau/Resource/StringUtf8To32ReadResourceTest.cpp
	src/test/cpp/Balau/Resource/StringUtf8To32ReadResourceTest.hpp
	src/test/cpp/Balau/Resource/UriComponentsTest.cpp
	src/test/cpp/Balau/Resource/UriComponentsTest.hpp
	src/test/cpp/Balau/Resource/ZipEntryByteReadResourceTest.cpp
	src/test/cpp/Balau/Resource/ZipEntryByteReadResourceTest.hpp
	src/test/cpp/Balau/Resource/ZipEntryUtf8To32ReadResourceTest.cpp
	src/test/cpp/Balau/Resource/ZipEntryUtf8To32ReadResourceTest.hpp
	src/test/cpp/Balau/Resource/ZipFileTest.cpp
	src/test/cpp/Balau/Resource/ZipFileTest.hpp
	src/test/cpp/Balau/System/SystemClockTest.cpp
	src/test/cpp/Balau/System/SystemClockTest.hpp
	src/test/cpp/Balau/Testing/AssertionsTest.cpp
	src/test/cpp/Balau/Testing/AssertionsTest.hpp
	src/test/cpp/Balau/Testing/AssertionsTestData.hpp
	src/test/cpp/Balau/ThirdParty/HashLibrary/HashLibraryTest.cpp
	src/test/cpp/Balau/ThirdParty/HashLibrary/HashLibraryTest.hpp
	src/test/cpp/Balau/Type/CharacterTest.cpp
	src/test/cpp/Balau/Type/CharacterTest.hpp
	src/test/cpp/Balau/Util/CompressionTest.cpp
	src/test/cpp/Balau/Util/CompressionTest.hpp
	src/test/cpp/Balau/Util/DateTimeTest.cpp
	src/test/cpp/Balau/Util/DateTimeTest.hpp
	src/test/cpp/Balau/Util/PrettyPrintTest.cpp
	src/test/cpp/Balau/Util/PrettyPrintTest.hpp
	src/test/cpp/Balau/Util/RandomTest.cpp
	src/test/cpp/Balau/Util/RandomTest.hpp
	src/test/cpp/Balau/Util/StreamsTest.cpp
	src/test/cpp/Balau/Util/StreamsTest.hpp
	src/test/cpp/Balau/Util/StringsTest.cpp
	src/test/cpp/Balau/Util/StringsTest.hpp
)

add_executable(BalauTests ${BALAU_TESTS_SOURCE_FILES})
target_link_libraries(BalauTests Balau ${ALL_LIBS})

#
# Copy logging test resource to build/bin directory.
#
add_custom_command(
	TARGET BalauTests
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
	${CMAKE_SOURCE_DIR}/src/test/resources/Logging/balau-logging.hconf
	${CMAKE_BINARY_DIR}/bin/balau-logging.hconf
)

############################## BALAU TEST RUNNER ##############################

add_custom_target(
	RunBalauTests
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
	COMMAND BalauTests all
)

add_dependencies(RunBalauTests BalauTests)

################################# EXAMPLE APPS ################################

add_executable(ExampleWebServer src/examples/WebServer/WebServer.cpp)
target_link_libraries(ExampleWebServer Balau ${ALL_LIBS})

add_executable(ExampleSkeletonApp src/examples/SkeletonApp/SkeletonApp.cpp)
target_link_libraries(ExampleSkeletonApp Balau ${ALL_LIBS})
