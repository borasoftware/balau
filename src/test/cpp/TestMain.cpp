// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include "Balau/System/ThreadName.hpp"

#include "Balau/Application/Impl/EnvironmentConfigurationBuilderTest.hpp"
#include "Balau/Application/CommandLineTest.hpp"
#include "Balau/Application/EnvironmentConfigurationTest.hpp"
#include "Balau/Application/InjectorTest.hpp"
#include "Balau/Application/ToStringTest.hpp"
#include "Balau/Container/ArrayBlockingQueueTest.hpp"
#include "Balau/Container/DependencyGraphTest.hpp"
#include "Balau/Container/ObjectTrieTest.hpp"
#include "Balau/Documentation/ConfigurationWebAppTest.hpp"
#include "Balau/Interprocess/SharedMemoryQueueTest.hpp"
#include "Balau/Lang/Common/ScannedTokensTest.hpp"
#include "Balau/Lang/Property/Parser/PropertyParserTest.hpp"
#include "Balau/Logging/Impl/LoggingStreamsTest.hpp"
#include "Balau/Logging/LoggerTest.hpp"
#include "Balau/Network/Http/Client/HttpClientTest.hpp"
#include "Balau/Network/Http/Client/HttpsClientTest.hpp"
#include "Balau/Network/Http/Server/HttpServerTest.hpp"
#include "Balau/Network/Http/Server/HttpWebApps/EmailSendingHttpWebAppTest.hpp"
#include "Balau/Network/Http/Server/HttpWebApps/FileServingHttpWebAppTest.hpp"
#include "Balau/Network/Http/Server/HttpWebApps/RedirectingHttpWebAppTest.hpp"
#include "Balau/Network/Http/Server/HttpWebApps/RoutingHttpWebAppTest.hpp"
#include "Balau/Resource/FileByteReadResourceTest.hpp"
#include "Balau/Resource/FileByteWriteResourceTest.hpp"
#include "Balau/Resource/FileTest.hpp"
#include "Balau/Resource/FileUtf32To8WriteResourceTest.hpp"
#include "Balau/Resource/FileUtf8To32ReadResourceTest.hpp"
#include "Balau/Resource/HttpByteReadResourceTest.hpp"
#include "Balau/Resource/HttpsByteReadResourceTest.hpp"
#include "Balau/Resource/HttpsUtf8To32ReadResourceTest.hpp"
#include "Balau/Resource/HttpUtf8To32ReadResourceTest.hpp"
#include "Balau/Resource/UriComponentsTest.hpp"
#include "Balau/Resource/ZipEntryByteReadResourceTest.hpp"
#include "Balau/Resource/ZipEntryUtf8To32ReadResourceTest.hpp"
#include "Balau/Resource/ZipFileTest.hpp"
#include "Balau/System/SystemClockTest.hpp"
#include "Balau/Testing/AssertionsTest.hpp"
#include "Balau/ThirdParty/HashLibrary/HashLibraryTest.hpp"
#include "Balau/Type/CharacterTest.hpp"
#include "Balau/Util/CompressionTest.hpp"
#include "Balau/Util/DateTimeTest.hpp"
#include "Balau/Util/PrettyPrintTest.hpp"
#include "Balau/Util/RandomTest.hpp"
#include "Balau/Util/StreamsTest.hpp"
#include "Balau/Util/StringsTest.hpp"
#include "TestResources.hpp"

#include <boost/process.hpp>

using namespace Balau;

//
// Set SINGLE_RUN to 1 in order to perform a single test run of the chosen model.
// This will allow breakpoints to be hit if one of the in-process models is chosen.
//
// If SINGLE_RUN is set to 0, four sequential test runs will be performed, one for
// each model, in a separate process for each run.
//
#define SINGLE_RUN 1

// Runs the Balau tests with all four execution models in turn.
int main(int argc, char * argv[]) {
	System::ThreadName::setName("TestMain");
	Testing::ExecutionModel model;

	if (!SINGLE_RUN && argc == 1) {
		std::cout << "Running tests with all execution models in turn.\n";

		struct RunTests {
			// Runs the test for a single execution model.
			void runTests(const char * executable, Testing::ExecutionModel model) {
				const int exitStatus = boost::process::system(executable, toString(model));
				std::ostream & stream = exitStatus == 0 ? std::cout : std::cerr;
				stream << "\n\n***** Test run for execution model " << toString(model)
				       << " returned exit status " << exitStatus << " *****\n" << std::endl;
			}
		};

		RunTests().runTests(argv[0], Testing::SingleThreaded);
		RunTests().runTests(argv[0], Testing::WorkerThreads);
		RunTests().runTests(argv[0], Testing::WorkerProcesses);
		RunTests().runTests(argv[0], Testing::ProcessPerTest);
		std::cout << "Finished running tests with all execution models in turn.\n";
		return 0;
	} else if (argc > 1) {
		model = Testing::executionModelFromString(argv[1]);

		std::cout << "\n\n***** Running tests for command line specified execution model "
		          << toString(model) << " *****\n\n" << std::flush;
	} else {
		model =
			Testing::SingleThreaded;
			//Testing::WorkerThreads;
			//Testing::WorkerProcesses;
			//Testing::ProcessPerTest;

		std::cout << "\n\n***** Running tests for predefined execution model "
		          << toString(model) << " *****\n\n" << std::flush;
	}

	Testing::TestRunner runner(model);

	return runner
		.registerGroup<Impl::EnvironmentConfigurationBuilderTest>()
		.registerGroup<CommandLineTest>()
		.registerGroup<EnvironmentConfigurationTest>()
		.registerGroup<InjectorTest>()
		.registerGroup<ToStringTest>()
		.registerGroup<Container::ArrayBlockingQueueTest>()
		.registerGroup<Container::DependencyGraphTest>()
		.registerGroup<Container::ObjectTrieTest>()
		.registerGroup<Documentation::ConfigurationWebAppTest>()
		.registerGroup<Interprocess::SharedMemoryQueueTest>()
		.registerGroup<Lang::ScannedTokensTest>()
		.registerGroup<Lang::Property::PropertyParserTest>()
		.registerGroup<LoggingSystem::LoggingStreamsTest>()
		.registerGroup<LoggerTest>()
		.registerGroup<Network::Http::HttpClientTest>()
		.registerGroup<Network::Http::HttpsClientTest>()
		.registerGroup<Network::Http::HttpWebApps::EmailSendingHttpWebAppTest>()
		.registerGroup<Network::Http::HttpWebApps::FileServingHttpWebAppTest>()
		.registerGroup<Network::Http::HttpWebApps::RedirectingHttpWebAppTest>()
		.registerGroup<Network::Http::HttpWebApps::RoutingHttpWebAppTest>()
		.registerGroup<Network::Http::HttpServerTest>()
		.registerGroup<Resource::FileByteReadResourceTest>()
		.registerGroup<Resource::FileByteWriteResourceTest>()
		.registerGroup<Resource::FileTest>()
		.registerGroup<Resource::FileUtf32To8WriteResourceTest>()
		.registerGroup<Resource::FileUtf8To32ReadResourceTest>()
		.registerGroup<Resource::HttpByteReadResourceTest>()
		.registerGroup<Resource::HttpsByteReadResourceTest>()
		.registerGroup<Resource::HttpsUtf8To32ReadResourceTest>()
		.registerGroup<Resource::HttpUtf8To32ReadResourceTest>()
		.registerGroup<Resource::UriComponentsTest>()
		.registerGroup<Resource::ZipEntryByteReadResourceTest>()
		.registerGroup<Resource::ZipEntryUtf8To32ReadResourceTest>()
		.registerGroup<Resource::ZipFileTest>()
		.registerGroup<System::SystemClockTest>()
		.registerGroup<AssertionsTest>()
		.registerGroup<HashLibraryTest>()
		.registerGroup<CharacterTest>()
		.registerGroup<Util::CompressionTest>()
		.registerGroup<Util::DateTimeTest>()
		.registerGroup<Util::PrettyPrintTest>()
		.registerGroup<Util::RandomTest>()
		.registerGroup<Util::StreamsTest>()
		.registerGroup<Util::StringsTest>()
		.printNamespaces(false)
		.run();
}
