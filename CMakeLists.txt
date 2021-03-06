##
## Balau core C++ library
##
## Copyright (C) 2008 Bora Software (contact@borasoftware.com)
##
## Licensed under the Apache License, Version 2.0 (the "License");
## you may not use this file except in compliance with the License.
## You may obtain a copy of the License at
##
##     http://www.apache.org/licenses/LICENSE-2.0
##
## Unless required by applicable law or agreed to in writing, software
## distributed under the License is distributed on an "AS IS" BASIS,
## WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
## See the License for the specific language governing permissions and
## limitations under the License.
##

##
## See src/doc/manual/NonCode/Building.bdml for information
## on building the Balau library and its dependencies.
##

cmake_minimum_required(VERSION 3.10.2)

################################ Balau version ################################

set(BALAU_VERSION_YEAR 2021)
set(BALAU_VERSION_MONTH 6)
set(BALAU_VERSION_PATCH 1)
set(BALAU_VERSION "${BALAU_VERSION_YEAR}.${BALAU_VERSION_MONTH}.${BALAU_VERSION_PATCH}")

project(Balau VERSION ${BALAU_VERSION})

message(STATUS "PROJECT_VERSION = ${PROJECT_VERSION}")

######################## Standard Balau CMake includes ########################

include(${CMAKE_SOURCE_DIR}/src/main/cmake/CMakeListsDefaultPaths.cmake)
include(${CMAKE_SOURCE_DIR}/src/main/cmake/CMakeListsDefaultConfiguration.cmake)

################### Generate files with version information ###################

configure_file(src/main/cmake/BalauConfigVersion.cmake.in src/main/cmake/BalauConfigVersion.cmake @ONLY)
configure_file(src/main/cmake/BalauConfig.cmake.in src/main/cmake/BalauConfig.cmake @ONLY)
configure_file(src/main/cmake/BalauFakeConfigVersion.cmake.in src/main/cmake/BalauFakeConfigVersion.cmake @ONLY)
configure_file(src/main/cmake/BalauFakeConfig.cmake.in src/main/cmake/BalauFakeConfig.cmake @ONLY)
configure_file(src/doc/single-page/pdf/title-page.bdml.in src/doc/single-page/pdf/title-page.bdml @ONLY)
configure_file(src/doc/api/Doxyfile.in src/doc/api/Doxyfile)

################################## Definitions ################################

if (BALAU_ENABLE_THREAD_LOCAL_LOGGING_ALLOCATOR)
	message(STATUS "Thread local logging allocator enabled.")
else ()
	message(STATUS "Thread local logging allocator not enabled.")
endif ()

#
# Balau test app variables
#

set(BALAU_BASE_FOLDER ${CMAKE_SOURCE_DIR})
set(BALAU_SOURCE_FOLDER ${CMAKE_SOURCE_DIR}/src)
set(BALAU_SOURCE_MAIN_FOLDER ${CMAKE_SOURCE_DIR}/src/main)
set(BALAU_SOURCE_TEST_FOLDER ${CMAKE_SOURCE_DIR}/src/test)
set(BALAU_SOURCE_TEST_RESOURCES_FOLDER ${CMAKE_SOURCE_DIR}/src/test/resources)
set(BALAU_TARGET_FOLDER ${CMAKE_BINARY_DIR})
set(BALAU_TEST_RESULTS_FOLDER ${CMAKE_BINARY_DIR}/testResults)

set(BALAU_TEST_EMAIL_HOST $ENV{BALAU_TEST_EMAIL_HOST})
set(BALAU_TEST_EMAIL_PORT $ENV{BALAU_TEST_EMAIL_PORT})
set(BALAU_TEST_EMAIL_SENDER_USER $ENV{BALAU_TEST_EMAIL_SENDER_USER})
set(BALAU_TEST_EMAIL_SENDER_PASSWORD $ENV{BALAU_TEST_EMAIL_SENDER_PASSWORD})
set(BALAU_TEST_EMAIL_TO $ENV{BALAU_TEST_EMAIL_TO})

if (DEFINED BALAU_TEST_EMAIL_HOST)
	message(STATUS "BALAU_TEST_EMAIL_HOST defined as ${BALAU_TEST_EMAIL_HOST}")
endif ()

if (DEFINED BALAU_LOGGING_THREAD_LOCAL_ALLOCATOR_BUFFER_SIZE_KB)
	add_definitions("-DBALAU_LOGGING_THREAD_LOCAL_ALLOCATOR_BUFFER_SIZE_KB=${BALAU_LOGGING_THREAD_LOCAL_ALLOCATOR_BUFFER_SIZE_KB}")
endif ()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/testResults)

include_directories(${CMAKE_SOURCE_DIR}/src/main/cpp)
include_directories(${CMAKE_BINARY_DIR}/src/main/cpp)

############################### Status printout ###############################

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

############################### Generated files ###############################

configure_file(src/main/cpp/Balau/BalauConfig.hpp.in src/main/cpp/Balau/BalauConfig.hpp @ONLY)
configure_file(src/main/cpp/Balau/Type/BalauVersion.hpp.in src/main/cpp/Balau/Type/BalauVersion.hpp)
configure_file(src/test/cpp/TestResources.cpp.in src/test/cpp/TestResources.cpp @ONLY)
configure_file(src/test/cpp/TestResources.hpp.in src/test/cpp/TestResources.hpp @ONLY)

############################ Balau libraries sources ##########################

set(THIRD_PARTY_SOURCE_FILES
	src/main/cpp/Balau/ThirdParty/Date/date.hpp

	src/main/cpp/Balau/ThirdParty/CppHttpLib/HttpLib.hpp

	src/main/cpp/Balau/ThirdParty/HashLibrary/crc32.h
	src/main/cpp/Balau/ThirdParty/HashLibrary/hash.h
	src/main/cpp/Balau/ThirdParty/HashLibrary/hmac.h
	src/main/cpp/Balau/ThirdParty/HashLibrary/keccak.h
	src/main/cpp/Balau/ThirdParty/HashLibrary/md5.h
	src/main/cpp/Balau/ThirdParty/HashLibrary/sha1.h
	src/main/cpp/Balau/ThirdParty/HashLibrary/sha256.h
	src/main/cpp/Balau/ThirdParty/HashLibrary/sha3.h
)

if (BALAU_ENABLE_ZLIB)
	set(BALAU_CORE_ZLIB_HEADER_FILES
		src/main/cpp/Balau/Util/Compression.hpp
	)
else ()
	set(BALAU_CORE_ZLIB_HEADER_FILES)
endif ()

if (BALAU_ENABLE_ZIP)
	set(BALAU_CORE_ZIP_HEADER_FILES
		src/main/cpp/Balau/Resource/ZipEntry.hpp
		src/main/cpp/Balau/Resource/ZipFile.hpp
		src/main/cpp/Balau/Util/Zip.hpp
	)
else ()
	set(BALAU_CORE_ZIP_HEADER_FILES)
endif ()

set(BALAU_CORE_HEADER_FILES

	${CMAKE_BINARY_DIR}/src/main/cmake/BalauConfig.cmake
	${CMAKE_BINARY_DIR}/src/main/cmake/BalauConfigVersion.cmake

	${CMAKE_BINARY_DIR}/src/main/cpp/Balau/BalauConfig.hpp
	${CMAKE_BINARY_DIR}/src/main/cpp/Balau/Type/BalauVersion.hpp

	${THIRD_PARTY_SOURCE_FILES}
	${BALAU_CORE_ZLIB_HEADER_FILES}
	${BALAU_CORE_ZIP_HEADER_FILES}

	src/main/cpp/Balau/Application/BindingBuilder.hpp
	src/main/cpp/Balau/Application/CommandLine.hpp
	src/main/cpp/Balau/Application/CommandLineHolder.hpp
	src/main/cpp/Balau/Application/Inject.hpp
	src/main/cpp/Balau/Application/Injectable.hpp
	src/main/cpp/Balau/Application/InjectBody.hpp
	src/main/cpp/Balau/Application/InjectHeader.hpp
	src/main/cpp/Balau/Application/InjectableBody.hpp
	src/main/cpp/Balau/Application/InjectableHeader.hpp
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
	src/main/cpp/Balau/Application/Impl/InjectorBodyMacros.hpp
	src/main/cpp/Balau/Application/Impl/InjectorHeaderMacros.hpp
	src/main/cpp/Balau/Application/Impl/InjectorMacros.hpp
	src/main/cpp/Balau/Application/Impl/PropertyBindingBuilderFactory.hpp
	src/main/cpp/Balau/Application/Impl/PropertyString.hpp
	src/main/cpp/Balau/Application/Impl/PropertyTypeSpecificationVisitor.hpp
	src/main/cpp/Balau/Application/Impl/StaticSingletonBuilder.hpp

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
	src/main/cpp/Balau/Interprocess/SharedMemoryUtils.hpp
	src/main/cpp/Balau/Interprocess/Impl/SharedMemoryQueueImpl.hpp

	src/main/cpp/Balau/Lang/Common/AbstractScanner.hpp
	src/main/cpp/Balau/Lang/Common/CodeSpan.hpp
	src/main/cpp/Balau/Lang/Common/ScannedTokens.hpp
	src/main/cpp/Balau/Lang/Property/PropertyParserService.hpp
	src/main/cpp/Balau/Lang/Property/Lang/PropertyAst.hpp
	src/main/cpp/Balau/Lang/Property/Lang/PropertyToken.hpp
	src/main/cpp/Balau/Lang/Property/Parser/PropertyParser.hpp
	src/main/cpp/Balau/Lang/Property/Parser/PropertyScanner.hpp
	src/main/cpp/Balau/Lang/Property/Util/PropertyAstToString.hpp
	src/main/cpp/Balau/Lang/Property/Util/PropertyVisitor.hpp

	src/main/cpp/Balau/Resource/ByteReadResource.hpp
	src/main/cpp/Balau/Resource/ByteWriteResource.hpp
	src/main/cpp/Balau/Resource/File.hpp
	src/main/cpp/Balau/Resource/Http.hpp
	src/main/cpp/Balau/Resource/Resource.hpp
	src/main/cpp/Balau/Resource/StringUri.hpp
	src/main/cpp/Balau/Resource/Uri.hpp
	src/main/cpp/Balau/Resource/Url.hpp
	src/main/cpp/Balau/Resource/UriComponents.hpp
	src/main/cpp/Balau/Resource/UriDispatcher.hpp
	src/main/cpp/Balau/Resource/UriFromString.hpp
	src/main/cpp/Balau/Resource/UriResolve.hpp
	src/main/cpp/Balau/Resource/Utf32To8WriteResource.hpp
	src/main/cpp/Balau/Resource/Utf8To32ReadResource.hpp

	src/main/cpp/Balau/Serialization/SerializationMacros.hpp

	src/main/cpp/Balau/System/Clock.hpp
	src/main/cpp/Balau/System/SystemClock.hpp
	src/main/cpp/Balau/System/Sleep.hpp
	src/main/cpp/Balau/System/ThreadName.hpp

	src/main/cpp/Balau/Type/Character.hpp
	src/main/cpp/Balau/Type/FromString.hpp
	src/main/cpp/Balau/Type/MovableOnScopeExit.hpp
	src/main/cpp/Balau/Type/OnScopeExit.hpp
	src/main/cpp/Balau/Type/StdTypes.hpp
	src/main/cpp/Balau/Type/ToString.hpp
	src/main/cpp/Balau/Type/ToStringA.hpp
	src/main/cpp/Balau/Type/ToStringS.hpp
	src/main/cpp/Balau/Type/UUID.hpp
	src/main/cpp/Balau/Type/Impl/FromStringImpl.hpp
	src/main/cpp/Balau/Type/Impl/ToStringImpl.hpp

	src/main/cpp/Balau/Util/App.hpp
	src/main/cpp/Balau/Util/Containers.hpp
	src/main/cpp/Balau/Util/DateTime.hpp
	src/main/cpp/Balau/Util/Enums.hpp
	src/main/cpp/Balau/Util/Files.hpp
	src/main/cpp/Balau/Util/Hashing.hpp
	src/main/cpp/Balau/Application/Macros.hpp
	src/main/cpp/Balau/Util/Memory.hpp
	src/main/cpp/Balau/Util/PrettyPrint.hpp
	src/main/cpp/Balau/Util/Random.hpp
	src/main/cpp/Balau/Util/Streams.hpp
	src/main/cpp/Balau/Util/Strings.hpp
	src/main/cpp/Balau/Util/User.hpp
	src/main/cpp/Balau/Util/Vectors.hpp
	src/main/cpp/Balau/Util/Impl/AppImpl.hpp
	src/main/cpp/Balau/Application/Impl/MacrosImpl.hpp
	src/main/cpp/Balau/Util/Impl/RandomImpl.hpp
	src/main/cpp/Balau/Util/Impl/StringsImpl.hpp

	src/doc
)

set(BALAU_LOGGING_SOURCE_FILES

	src/main/cpp/Balau/Logging/Logger.cpp
	src/main/cpp/Balau/Logging/Logger.hpp
	src/main/cpp/Balau/Logging/LoggerMacros.hpp
	src/main/cpp/Balau/Logging/LoggingLevel.hpp
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
)

set(BALAU_TESTING_SOURCE_FILES

	src/main/cpp/Balau/Testing/Assertions.hpp
	src/main/cpp/Balau/Testing/Matchers.hpp
	src/main/cpp/Balau/Testing/TestRunner.hpp
	src/main/cpp/Balau/Testing/ExecutionModel.hpp
	src/main/cpp/Balau/Testing/Impl/CompositeWriter.hpp
	src/main/cpp/Balau/Testing/Impl/FlattenedTestCase.hpp
	src/main/cpp/Balau/Testing/Impl/HexContainerRendererImpl.hpp
	src/main/cpp/Balau/Testing/Impl/MatcherClasses.hpp
	src/main/cpp/Balau/Testing/Impl/MultiProcessTestRunnerExecutor.hpp
	src/main/cpp/Balau/Testing/Impl/ProcessPerTestTestRunnerExecutor.hpp
	src/main/cpp/Balau/Testing/Impl/SingleThreadedTestRunnerExecutor.hpp
	src/main/cpp/Balau/Testing/Impl/StandardRenderers.hpp
	src/main/cpp/Balau/Testing/Impl/TestCase.hpp
	src/main/cpp/Balau/Testing/Impl/TestGroupBase.hpp
	src/main/cpp/Balau/Testing/Impl/TestMethodBase.hpp
	src/main/cpp/Balau/Testing/Impl/TestResult.hpp
	src/main/cpp/Balau/Testing/Impl/TestResultQueue.hpp
	src/main/cpp/Balau/Testing/Impl/TestReportGenerator.hpp
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
	src/main/cpp/Balau/Testing/Reporters/SurefireTestReportGenerator.cpp
	src/main/cpp/Balau/Testing/Reporters/SurefireTestReportGenerator.hpp
	src/main/cpp/Balau/Testing/Util/NetworkTesting.hpp
	src/main/cpp/Balau/Testing/Writers/TestWriter.hpp
	src/main/cpp/Balau/Testing/Writers/StdWriters.hpp
	src/main/cpp/Balau/Testing/Writers/LogWriter.hpp
)

set_source_files_properties(${CMAKE_BINARY_DIR}/src/main/cpp/Balau/BalauConfig.hpp PROPERTIES GENERATED TRUE)

if (BALAU_ENABLE_HTTP)
	set(BALAU_HTTP_SOURCE_FILES
		src/main/cpp/Balau/ThirdParty/Boost/Beast/Http/BasicFileBody.hpp
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
		src/main/cpp/Balau/Network/Utilities/BalauLogger.cpp
		src/main/cpp/Balau/Network/Utilities/BalauLogger.hpp
		src/main/cpp/Balau/Network/Utilities/MimeTypes.cpp
		src/main/cpp/Balau/Network/Utilities/MimeTypes.hpp
			src/main/cpp/Balau/ThirdParty/Boost/Beast/Http/root_certificates.hpp
		src/main/cpp/Balau/Network/Utilities/UrlDecode.hpp
	)
else ()
	set(BALAU_HTTP_SOURCE_FILES)
endif ()

if (BALAU_ENABLE_HTTP AND BALAU_ENABLE_CURL)
	set(BALAU_CURL_SOURCE_FILES
		src/main/cpp/Balau/Network/Http/Server/HttpWebApps/EmailSendingHttpWebApp.cpp
		src/main/cpp/Balau/Network/Http/Server/HttpWebApps/EmailSendingHttpWebApp.hpp
		src/main/cpp/Balau/Network/Http/Server/HttpWebApps/Impl/CurlEmailSender.cpp
		src/main/cpp/Balau/Network/Http/Server/HttpWebApps/Impl/CurlEmailSender.hpp
		src/main/cpp/Balau/Network/Http/Server/HttpWebApps/Impl/CurlInitializer.cpp
		src/main/cpp/Balau/Network/Http/Server/HttpWebApps/Impl/CurlInitializer.hpp
	)
else ()
	set(BALAU_CURL_SOURCE_FILES)
endif ()

################################ Balau libraries ##############################

add_library(BalauLogging_objects OBJECT ${BALAU_LOGGING_SOURCE_FILES} ${BALAU_CORE_HEADER_FILES})
set_property(TARGET BalauLogging_objects PROPERTY POSITION_INDEPENDENT_CODE 1)
add_library(BalauLogging STATIC $<TARGET_OBJECTS:BalauLogging_objects>)
add_library(BalauLoggingShared SHARED $<TARGET_OBJECTS:BalauLogging_objects>)
target_link_libraries(BalauLogging ${ALL_LIBS})
target_link_libraries(BalauLoggingShared ${ALL_LIBS})

if (BALAU_ENABLE_HTTP)
	add_library(BalauNetwork_objects OBJECT ${BALAU_HTTP_SOURCE_FILES} ${BALAU_CURL_SOURCE_FILES} ${BALAU_CORE_HEADER_FILES})
	set_property(TARGET BalauNetwork_objects PROPERTY POSITION_INDEPENDENT_CODE 1)
	add_library(BalauNetwork STATIC $<TARGET_OBJECTS:BalauNetwork_objects>)
	add_library(BalauNetworkShared SHARED $<TARGET_OBJECTS:BalauNetwork_objects>)
	target_link_libraries(BalauNetwork BalauLogging ${ALL_LIBS})
	target_link_libraries(BalauNetworkShared BalauLoggingShared ${ALL_LIBS})
endif ()

add_library(BalauTesting_objects OBJECT ${BALAU_TESTING_SOURCE_FILES} ${BALAU_CORE_HEADER_FILES})
set_property(TARGET BalauTesting_objects PROPERTY POSITION_INDEPENDENT_CODE 1)
add_library(BalauTesting STATIC $<TARGET_OBJECTS:BalauTesting_objects>)
add_library(BalauTestingShared SHARED $<TARGET_OBJECTS:BalauTesting_objects>)
target_link_libraries(BalauTesting BalauLogging ${ALL_LIBS})
target_link_libraries(BalauTestingShared BalauLoggingShared ${ALL_LIBS})

############################# BALAU DOCUMENTATION #############################

#
# This target requires doxygen and graphviz.
#
add_custom_target(
	BalauApiDoc
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
	COMMAND doxygen ${CMAKE_BINARY_DIR}/src/doc/api/Doxyfile
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

#
# This target requires bdml.
#
add_custom_target(
		BalauManualPdf
		WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
		COMMAND sh ../src/doc/scripts/makePdf.sh ${PROJECT_VERSION}
)

############################ Installable file lists ###########################

install(DIRECTORY src/main/cpp/Balau/ DESTINATION "include/Balau-${PROJECT_VERSION}/Balau" FILES_MATCHING PATTERN "*.hpp")
install(DIRECTORY src/main/resources/BalauConfig/ DESTINATION "include/Balau-${PROJECT_VERSION}/BalauConfig" FILES_MATCHING PATTERN "*.thconf")
install(FILES ${CMAKE_BINARY_DIR}/src/main/cmake/BalauConfig.cmake DESTINATION "lib/Balau-${PROJECT_VERSION}")
install(FILES ${CMAKE_BINARY_DIR}/src/main/cmake/BalauConfigVersion.cmake DESTINATION "lib/Balau-${PROJECT_VERSION}")
install(FILES src/main/cmake/CMakeListsDefaultPaths.cmake DESTINATION "include/Balau-${PROJECT_VERSION}/Balau")
install(FILES src/main/cmake/CMakeListsDefaultConfiguration.cmake DESTINATION "include/Balau-${PROJECT_VERSION}/Balau")
install(FILES ${CMAKE_BINARY_DIR}/src/main/cpp/Balau/BalauConfig.hpp DESTINATION "include/Balau-${PROJECT_VERSION}/Balau")

install(DIRECTORY src/main/cpp/BalauFake/ DESTINATION "include/BalauFake-${PROJECT_VERSION}/Balau" FILES_MATCHING PATTERN "*.hpp")
install(FILES ${CMAKE_BINARY_DIR}/src/main/cmake/BalauFakeConfig.cmake DESTINATION "lib/BalauFake-${PROJECT_VERSION}")
install(FILES ${CMAKE_BINARY_DIR}/src/main/cmake/BalauFakeConfigVersion.cmake DESTINATION "lib/BalauFake-${PROJECT_VERSION}")

install(TARGETS BalauLogging DESTINATION "lib/Balau-${PROJECT_VERSION}")
install(TARGETS BalauLoggingShared DESTINATION "lib/Balau-${PROJECT_VERSION}")

if (BALAU_ENABLE_HTTP)
	install(TARGETS BalauNetwork DESTINATION "lib/Balau-${PROJECT_VERSION}")
	install(TARGETS BalauNetworkShared DESTINATION "lib/Balau-${PROJECT_VERSION}")
endif ()

install(TARGETS BalauTesting DESTINATION "lib/Balau-${PROJECT_VERSION}")
install(TARGETS BalauTestingShared DESTINATION "lib/Balau-${PROJECT_VERSION}")

################################# Balau tests #################################

set(BALAU_TESTS_SOURCE_FILES
	src/test/resources

	${CMAKE_BINARY_DIR}/src/test/cpp/TestResources.cpp
	${CMAKE_BINARY_DIR}/src/test/cpp/TestResources.hpp

	src/test/cpp/TestMain.cpp

	src/test/cpp/Balau/Application/CommandLineTest.cpp
	src/test/cpp/Balau/Application/InjectorTest.cpp
	src/test/cpp/Balau/Application/InjectorHeaderBody.cpp
	src/test/cpp/Balau/Application/InjectorHeaderBody.hpp
	src/test/cpp/Balau/Application/ToStringTest.cpp
	src/test/cpp/Balau/Container/ArrayBlockingQueueTest.cpp
	src/test/cpp/Balau/Container/DependencyGraphTest.cpp
	src/test/cpp/Balau/Container/ObjectTrieTest.cpp
	src/test/cpp/Balau/Interprocess/SharedMemoryQueueTest.cpp
	src/test/cpp/Balau/Lang/Common/ScannedTokensTest.cpp
	src/test/cpp/Balau/Lang/Property/Parser/PropertyParserTest.cpp
	src/test/cpp/Balau/Logging/LoggerTest.cpp
	src/test/cpp/Balau/Logging/Impl/LoggingStreamsTest.cpp
	src/test/cpp/Balau/Resource/FileByteReadResourceTest.cpp
	src/test/cpp/Balau/Resource/FileByteWriteResourceTest.cpp
	src/test/cpp/Balau/Resource/FileTest.cpp
	src/test/cpp/Balau/Resource/FileUtf32To8WriteResourceTest.cpp
	src/test/cpp/Balau/Resource/FileUtf8To32ReadResourceTest.cpp
	src/test/cpp/Balau/Resource/StringByteReadResourceTest.cpp
	src/test/cpp/Balau/Resource/StringUriTest.cpp
	src/test/cpp/Balau/Resource/StringUtf8To32ReadResourceTest.cpp
	src/test/cpp/Balau/Resource/UriComponentsTest.cpp
	src/test/cpp/Balau/System/SystemClockTest.cpp
	src/test/cpp/Balau/Testing/AssertionsTest.cpp
	src/test/cpp/Balau/Testing/AssertionsTestData.hpp
	src/test/cpp/Balau/ThirdParty/HashLibrary/HashLibraryTest.cpp
	src/test/cpp/Balau/Type/CharacterTest.cpp
	src/test/cpp/Balau/Util/AppTest.cpp
	src/test/cpp/Balau/Util/DateTimeTest.cpp
	src/test/cpp/Balau/Util/PrettyPrintTest.cpp
	src/test/cpp/Balau/Util/RandomTest.cpp
	src/test/cpp/Balau/Util/StreamsTest.cpp
	src/test/cpp/Balau/Util/StringsTest.cpp
	src/test/cpp/Balau/Util/UserTest.cpp
)

if (BALAU_ENABLE_HTTP)
	set(BALAU_TESTS_HTTP_SOURCE_FILES
		src/test/cpp/Balau/Application/EnvironmentConfigurationTest.cpp
		src/test/cpp/Balau/Application/Impl/EnvironmentConfigurationBuilderTest.cpp
		src/test/cpp/Balau/Network/Http/Client/HttpClientTest.cpp
		src/test/cpp/Balau/Network/Http/Client/HttpsClientTest.cpp
		src/test/cpp/Balau/Network/Http/Server/HttpServerTest.cpp
		src/test/cpp/Balau/Network/Http/Server/HttpWebApps/FileServingHttpWebAppTest.cpp
		src/test/cpp/Balau/Network/Http/Server/HttpWebApps/EmailSendingHttpWebAppTest.cpp
		src/test/cpp/Balau/Network/Http/Server/HttpWebApps/RedirectingHttpWebAppTest.cpp
		src/test/cpp/Balau/Network/Http/Server/HttpWebApps/RoutingHttpWebAppTest.cpp
		src/test/cpp/Balau/Network/Http/Server/WsWebApps/ChatWsWebAppTest.cpp
		src/test/cpp/Balau/Network/Http/Server/WsWebApps/EchoingWsWebAppTest.cpp
		src/test/cpp/Balau/Network/Utilities/UrlDecodeTest.cpp
		src/test/cpp/Balau/Resource/HttpByteReadResourceTest.cpp
		src/test/cpp/Balau/Resource/HttpsByteReadResourceTest.cpp
		src/test/cpp/Balau/Resource/HttpsUtf8To32ReadResourceTest.cpp
		src/test/cpp/Balau/Resource/HttpsTest.cpp
		src/test/cpp/Balau/Resource/HttpTest.cpp
		src/test/cpp/Balau/Resource/HttpUtf8To32ReadResourceTest.cpp
	)
else ()
	set(BALAU_TESTS_HTTP_SOURCE_FILES)
endif ()

if (BALAU_ENABLE_CURL)
	set(BALAU_TESTS_CURL_SOURCE_FILES
		src/test/cpp/Balau/Network/Http/Server/HttpWebApps/EmailSendingHttpWebAppTest.cpp
	)
else ()
	set(BALAU_TESTS_CURL_SOURCE_FILES)
endif ()

if (BALAU_ENABLE_ZIP)
	set(BALAU_TESTS_ZIP_SOURCE_FILES
		src/test/cpp/Balau/Resource/ZipEntryByteReadResourceTest.cpp
		src/test/cpp/Balau/Resource/ZipEntryUtf8To32ReadResourceTest.cpp
		src/test/cpp/Balau/Resource/ZipFileTest.cpp
		src/test/cpp/Balau/Util/ZipTest.cpp
	)
else ()
	set(BALAU_TESTS_ZIP_SOURCE_FILES)
endif ()

set(ALL_TEST_LIBS BalauLogging BalauTesting)

if (BALAU_ENABLE_HTTP)
	set(ALL_TEST_LIBS ${ALL_TEST_LIBS} BalauNetwork)
endif ()

set(ALL_TEST_LIBS ${ALL_TEST_LIBS} ${ALL_LIBS})

add_executable(
	BalauTests
	${BALAU_TESTS_SOURCE_FILES}
	${BALAU_TESTS_HTTP_SOURCE_FILES}
	${BALAU_TESTS_CURL_SOURCE_FILES}
	${BALAU_TESTS_ZIP_SOURCE_FILES}
)

target_link_libraries(BalauTests ${ALL_TEST_LIBS})
target_include_directories(BalauTests PUBLIC ${CMAKE_BINARY_DIR}/src/test/cpp)
target_compile_options(BalauTests PUBLIC "-Wno-string-plus-int")

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

############################## Balau test runner ##############################

add_custom_target(
	RunBalauTests
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
	COMMAND BalauTests all
)

add_dependencies(RunBalauTests BalauTests)

############################## Code quality tools #############################

#
# A targt to run Memcheck.
#
# This target requires CppCheck.
#
# This target requires Valgrind.
#
add_custom_target(
	RunBalauTestsMemcheck
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
	COMMAND valgrind --tool=memcheck --leak-check=full --leak-resolution=med --track-origins=yes --vgdb=no --trace-children=yes --read-inline-info=yes --fair-sched=yes --read-var-info=yes ./BalauTests SingleThreaded
)

add_dependencies(RunBalauTestsMemcheck BalauTests)

#
# A targt to run Memcheck with the WorkerThreads execution model..
#
# This target requires Valgrind.
#
add_custom_target(
	RunBalauTestsMemcheckWorkerThreads
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
	COMMAND valgrind --tool=memcheck --leak-check=full --leak-resolution=med --track-origins=yes --vgdb=no --trace-children=yes --read-inline-info=yes --fair-sched=yes --read-var-info=yes ./BalauTests WorkerThreads
)

add_dependencies(RunBalauTestsMemcheck BalauTests)

#
# A targt to run Helgrind.
#
# This target requires Valgrind.
#
add_custom_target(
	RunBalauTestsHelgrind
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
	COMMAND valgrind --tool=helgrind --vgdb=no --trace-children=yes --read-inline-info=yes --fair-sched=yes --suppressions=../../src/test/data/balau-helgrind.supp ./BalauTests SingleThreaded
)

add_dependencies(RunBalauTestsHelgrind BalauTests)

#
# A targt to run CppCheck on the compile_commands.json file.
#
# This target requires CppCheck.
#
add_custom_target(
	RunCppCheck
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
	COMMAND cppcheck --enable=warning,performance,information --project=compile_commands.json -j 4
)

add_dependencies(RunCppCheck BalauTests)

#
# A targt to run CppCheck on the compile_commands.json file and create an XML report file.
#
# This target requires CppCheck.
#
add_custom_target(
	RunCppCheckXml
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
	COMMAND cppcheck --enable=warning,performance,information --project=compile_commands.json --xml balau_cppcheck.xml -j 4
)

add_dependencies(RunCppCheck BalauTests)

################################# Example apps ################################

add_executable(ExampleSkeletonApp src/examples/SkeletonApp/SkeletonApp.cpp)
target_link_libraries(ExampleSkeletonApp BalauLogging ${ALL_TEST_LIBS})

if (BALAU_ENABLE_HTTP)
	add_executable(ExampleWebServer src/examples/WebServer/WebServer.cpp)
	target_link_libraries(ExampleWebServer BalauLogging BalauNetwork ${ALL_TEST_LIBS})
endif ()
