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
project(Balau VERSION 2019.7.1)

include(${CMAKE_SOURCE_DIR}/src/main/cmake/CMakeListsDefaultPaths.cmake)
include(${CMAKE_SOURCE_DIR}/src/main/cmake/CMakeListsDefaultConfiguration.cmake)

################################## DEFINITIONS ################################

if (BALAU_ENABLE_THREAD_LOCAL_LOGGING_ALLOCATOR)
	message(STATUS "Thread local logging allocator enabled.")
else ()
	message(STATUS "Thread local logging allocator not enabled.")
endif ()

#
# Balau test app variables
#

set(BALAU_VERSION ${PROJECT_VERSION})
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

if (DEFINED BALAU_LOGGING_THREAD_LOCAL_ALLOCATOR_BUFFER_SIZE_KB)
	add_definitions("-DBALAU_LOGGING_THREAD_LOCAL_ALLOCATOR_BUFFER_SIZE_KB=${BALAU_LOGGING_THREAD_LOCAL_ALLOCATOR_BUFFER_SIZE_KB}")
endif ()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/testResults)

include_directories(src/main/cpp)
include_directories(${CMAKE_BINARY_DIR})

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

############################### GENERATED FILES ###############################

configure_file(src/main/cmake/BalauConfig.hpp.in Balau/BalauConfig.hpp @ONLY)
configure_file(src/doc/api/Doxyfile.in Doxyfile)
configure_file(src/test/cpp/TestResources.cpp.in TestResources.cpp @ONLY)
configure_file(src/test/cpp/TestResources.hpp.in TestResources.hpp @ONLY)

################################# BALAU LIBRARY ###############################

set(THIRD_PARTY_SOURCE_FILES
	src/main/cpp/Balau/ThirdParty/Date/date.hpp
	src/main/cpp/Balau/ThirdParty/Date/tz.cpp
	src/main/cpp/Balau/ThirdParty/Date/tz.hpp
	src/main/cpp/Balau/ThirdParty/Date/tz_private.hpp

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

	${CMAKE_BINARY_DIR}/Balau/BalauConfig.hpp

	src/main/cpp/Balau/Application/BindingBuilder.hpp
	src/main/cpp/Balau/Application/CommandLine.hpp
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
	src/main/cpp/Balau/Application/Impl/InjectorLogger.cpp
	src/main/cpp/Balau/Application/Impl/InjectorLogger.hpp
	src/main/cpp/Balau/Application/Impl/InjectorBodyMacros.hpp
	src/main/cpp/Balau/Application/Impl/InjectorHeaderMacros.hpp
	src/main/cpp/Balau/Application/Impl/InjectorMacros.hpp
	src/main/cpp/Balau/Application/Impl/PropertyBindingBuilderFactory.cpp
	src/main/cpp/Balau/Application/Impl/PropertyBindingBuilderFactory.hpp
	src/main/cpp/Balau/Application/Impl/PropertyString.hpp
	src/main/cpp/Balau/Application/Impl/PropertyTypeSpecificationVisitor.cpp
	src/main/cpp/Balau/Application/Impl/PropertyTypeSpecificationVisitor.hpp

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
	src/main/cpp/Balau/Resource/Utf32To8WriteResource.hpp
	src/main/cpp/Balau/Resource/Utf8To32ReadResource.hpp

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
	src/main/cpp/Balau/Testing/Writers/TestWriter.hpp
	src/main/cpp/Balau/Testing/Writers/StdWriters.hpp
	src/main/cpp/Balau/Testing/Writers/LogWriter.cpp
	src/main/cpp/Balau/Testing/Writers/LogWriter.hpp

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
	src/main/cpp/Balau/Util/Impl/MacrosImpl.hpp
	src/main/cpp/Balau/Util/Impl/RandomImpl.hpp
	src/main/cpp/Balau/Util/Impl/StringsImpl.hpp

	src/main/cmake/BalauConfig.cmake
	src/main/cmake/BalauConfigVersion.cmake

	src/doc
)

set_source_files_properties(${CMAKE_BINARY_DIR}/Balau/BalauConfig.hpp PROPERTIES GENERATED TRUE)

if (BALAU_ENABLE_HTTP)
	set(BALAU_HTTP_SOURCE_FILES
		src/main/cpp/Balau/ThirdParty/Boost/Beast/Http/BasicFileBody.hpp
		src/main/cpp/Balau/Documentation/ConfigurationWebApp.cpp
		src/main/cpp/Balau/Documentation/ConfigurationWebApp.hpp
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
		src/main/cpp/Balau/Resource/Url.hpp
		src/main/cpp/Balau/Resource/Impl/HttpSource.cpp
		src/main/cpp/Balau/Resource/Impl/HttpSource.hpp
		src/main/cpp/Balau/Resource/Impl/HttpsSource.cpp
		src/main/cpp/Balau/Resource/Impl/HttpsSource.hpp
		src/main/cpp/Balau/Testing/Util/NetworkTesting.cpp
		src/main/cpp/Balau/Testing/Util/NetworkTesting.hpp
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

if (BALAU_ENABLE_ZLIB)
	set(BALAU_ZLIB_SOURCE_FILES
		src/main/cpp/Balau/Util/Compression.hpp
	)
else ()
	set(BALAU_ZLIB_SOURCE_FILES)
endif ()

if (BALAU_ENABLE_ZIP)
	set(BALAU_ZIP_SOURCE_FILES
		src/main/cpp/Balau/Resource/ZipEntry.cpp
		src/main/cpp/Balau/Resource/ZipEntry.hpp
		src/main/cpp/Balau/Resource/ZipEntryByteReadResource.cpp
		src/main/cpp/Balau/Resource/ZipEntryByteReadResource.hpp
		src/main/cpp/Balau/Resource/ZipEntryUtf8To32ReadResource.cpp
		src/main/cpp/Balau/Resource/ZipEntryUtf8To32ReadResource.hpp
		src/main/cpp/Balau/Resource/ZipFile.hpp
		src/main/cpp/Balau/Resource/Impl/ZipEntrySource.cpp
		src/main/cpp/Balau/Resource/Impl/ZipEntrySource.hpp
		src/main/cpp/Balau/Util/Zip.cpp
		src/main/cpp/Balau/Util/Zip.hpp
	)
else ()
	set(BALAU_ZIP_SOURCE_FILES)
endif ()

add_library(
	Balau
	STATIC
	${THIRD_PARTY_SOURCE_FILES}
	${BALAU_SOURCE_FILES}
	${BALAU_HTTP_SOURCE_FILES}
	${BALAU_CURL_SOURCE_FILES}
	${BALAU_ZLIB_SOURCE_FILES}
	${BALAU_ZIP_SOURCE_FILES}
)

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
install(FILES src/main/cmake/CMakeListsDefaultPaths.cmake DESTINATION "include/Balau-${PROJECT_VERSION}/Balau")
install(FILES src/main/cmake/CMakeListsDefaultConfiguration.cmake DESTINATION "include/Balau-${PROJECT_VERSION}/Balau")
install(FILES ${CMAKE_BINARY_DIR}/Balau/BalauConfig.hpp DESTINATION "include/Balau-${PROJECT_VERSION}/Balau")
install(DIRECTORY src/main/resources/BalauConfig/ DESTINATION "include/Balau-${PROJECT_VERSION}/BalauConfig" FILES_MATCHING PATTERN "*.thconf")

################################# BALAU TESTS #################################

set(BALAU_TESTS_SOURCE_FILES
	src/test/resources

	${CMAKE_BINARY_DIR}/TestResources.cpp
	${CMAKE_BINARY_DIR}/TestResources.hpp
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
	src/test/cpp/Balau/Resource/StringUtf8To32ReadResourceTest.cpp
	src/test/cpp/Balau/Resource/UriComponentsTest.cpp
	src/test/cpp/Balau/System/SystemClockTest.cpp
	src/test/cpp/Balau/Testing/AssertionsTest.cpp
	src/test/cpp/Balau/Testing/AssertionsTestData.hpp
	src/test/cpp/Balau/ThirdParty/HashLibrary/HashLibraryTest.cpp
	src/test/cpp/Balau/Type/CharacterTest.cpp
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
		src/test/cpp/Balau/Documentation/ConfigurationWebAppTest.cpp
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

add_executable(
	BalauTests
	${BALAU_TESTS_SOURCE_FILES}
	${BALAU_TESTS_HTTP_SOURCE_FILES}
	${BALAU_TESTS_CURL_SOURCE_FILES}
	${BALAU_TESTS_ZIP_SOURCE_FILES}
)

target_link_libraries(BalauTests Balau ${ALL_LIBS})
target_include_directories(BalauTests PUBLIC ${CMAKE_BINARY_DIR})
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

############################## BALAU TEST RUNNER ##############################

add_custom_target(
	RunBalauTests
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
	COMMAND BalauTests all
)

add_dependencies(RunBalauTests BalauTests)

############################## CODE QUALITY TOOLS #############################

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

################################# EXAMPLE APPS ################################

if (BALAU_ENABLE_HTTP)
	add_executable(ExampleWebServer src/examples/WebServer/WebServer.cpp)
	target_link_libraries(ExampleWebServer Balau ${ALL_LIBS})
endif ()

add_executable(ExampleSkeletonApp src/examples/SkeletonApp/SkeletonApp.cpp)
target_link_libraries(ExampleSkeletonApp Balau ${ALL_LIBS})
