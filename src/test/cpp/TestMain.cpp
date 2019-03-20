// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include "Balau/Testing/TestRunner.hpp"

#include <boost/process.hpp>

using namespace Balau;

//
// Balau specific logic that runs all execution models.
//
int runAllExecutionModels(int argc, char * argv[]) {
	std::cout << "Running tests with all execution models in turn.\n";

	std::string testList;

	// Join the test list up into a single comma delimited argument to send to the system call.
	if (argc > 2) {
		testList = argv[2];

		for (int i = 3; i < argc; ++i) {
			testList += " ";
			testList += argv[i];
		}

		auto v = Util::Strings::split(testList, std::regex(" *, *| +"));
		auto s = std::set<std::string_view>(v.begin(), v.end());
		testList = Util::Strings::joinContainer(",", s);
	}

	// Runs the test for a single execution model.
	struct RunTests {
		int runTests(const char * executable, Testing::ExecutionModel model, std::string testList) {
			const int exitStatus = boost::process::system(executable, toString(model), testList);
			std::ostream & stream = exitStatus == 0 ? std::cout : std::cerr;
			stream << "\n\n***** Test run for execution model " << toString(model)
			       << " returned exit status " << exitStatus << " *****\n" << std::endl;
			return exitStatus;
		}
	};

	int thisExitStatus;
	int exitStatus = 0;

	thisExitStatus = RunTests().runTests(argv[0], Testing::SingleThreaded, testList);
	exitStatus = thisExitStatus != 0 ? thisExitStatus : exitStatus;
	thisExitStatus = RunTests().runTests(argv[0], Testing::WorkerThreads, testList);
	exitStatus = thisExitStatus != 0 ? thisExitStatus : exitStatus;
	thisExitStatus = RunTests().runTests(argv[0], Testing::WorkerProcesses, testList);
	exitStatus = thisExitStatus != 0 ? thisExitStatus : exitStatus;
	thisExitStatus = RunTests().runTests(argv[0], Testing::ProcessPerTest, testList);
	exitStatus = thisExitStatus != 0 ? thisExitStatus : exitStatus;

	std::cout << "Finished running tests with all execution models in turn.\n";

	return exitStatus;
}

//
// Runs the Balau tests:
//
//  - as a single run with the single threaded execution model if no arguments
//    are supplied or if argv[1] is not "all" or an execution model;
//
//  - with all four execution models in turn if argv[1] == all;
//
//  - as a single run with the specified execution model otherwise.
//
int main(int argc, char * argv[]) {
	// Special Balau test run mode that runs all four execution models.
	if (argc > 1 && Util::Strings::toLower(argv[1]) == "all") {
		return runAllExecutionModels(argc, argv);
	} else {
		return Testing::TestRunner::run(argc, argv);
	}
}
