// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#include "SurefireTestReportGenerator.hpp"
#include "../Impl/TestGroupBase.hpp"
#include "../Impl/TestResult.hpp"

#include <iostream>
#include <numeric>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

namespace Balau::Testing::Reporters {

static void writeProperty(boost::property_tree::ptree & properties, const std::string & name, const std::string & value) {
	boost::property_tree::ptree & property = properties.add("property", "");
	property.add("<xmlattr>.name", name);
	property.add("<xmlattr>.value", value);
}

// A few basic properties.. may add more later.
static void writeProperties(boost::property_tree::ptree & library) {
	boost::property_tree::ptree & properties = library.add("properties", "");

	writeProperty(properties, "line.separator", "\n");
	writeProperty(properties, "file.encoding", "UTF-8");
	writeProperty(properties, "file.separator", "/");
}

static size_t getResultCount(const std::vector<const Impl::TestResult *> & groupTestResults, Impl::TestResult::Result resultType) {
	return std::accumulate(
		  groupTestResults.begin()
		, groupTestResults.end()
		, 0U
		, [resultType] (size_t count, const Impl::TestResult * testResult) {
			return count + (testResult->result == resultType ? 1 : 0);
		}
	);
}

static std::string toSeconds(unsigned long long ns) {
	std::string d = std::to_string((double) ns / 1.0e9);
	return d;
}

void SurefireTestReportGenerator::generate(const Impl::TestGroupBase & group,
                                           const std::chrono::nanoseconds & groupDuration,
                                           const std::vector<const Impl::TestResult *> & groupTestResults) {
	if (outputFolder.toRawString().empty()) {
		return; // Nowhere to write to.
	}

	boost::property_tree::ptree report;

	boost::property_tree::ptree & testsuite = report.add("testsuite", "");
	testsuite.add("<xmlattr>.tests", groupTestResults.size());
	testsuite.add("<xmlattr>.name", group.getGroupName());
	testsuite.add("<xmlattr>.time", toSeconds(groupDuration.count()));
	testsuite.add("<xmlattr>.failures", getResultCount(groupTestResults, Impl::TestResult::Result::Failure));
	testsuite.add("<xmlattr>.errors", getResultCount(groupTestResults, Impl::TestResult::Result::Error));
	testsuite.add("<xmlattr>.skipped", getResultCount(groupTestResults, Impl::TestResult::Result::Ignored));

	writeProperties(testsuite);

	for (const auto * testResult : groupTestResults) {
		boost::property_tree::ptree & testcase = testsuite.add("testcase", "");

		testcase.add("<xmlattr>.classname", group.getGroupName());
		testcase.add("<xmlattr>.name", testResult->testName);
		testcase.add("<xmlattr>.time", toSeconds(testResult->duration));

		switch (testResult->result) {
			case Impl::TestResult::Result::Success: {
				break;
			}

			case Impl::TestResult::Result::Failure: {
				boost::property_tree::ptree & failure = testcase.add("failure", "");
				failure.add("<xmlattr>.message", testResult->resultText);
				failure.add("<xmlattr>.type", "AssertionException");
				break;
			}

			case Impl::TestResult::Result::Error: {
				boost::property_tree::ptree & error = testcase.add("error", "");
				error.add("<xmlattr>.message", testResult->resultText);
				error.add("<xmlattr>.type", "");
				break;
			}

			case Impl::TestResult::Result::Ignored: {
				boost::property_tree::ptree & skipped = testcase.add("skipped", "");
				skipped.add("<xmlattr>.message", testResult->resultText);
				break;
			}
		}

		// todo
		// <xs:element name="system-out" nillable="true" minOccurs="0" maxOccurs="1"/>
		// <xs:element name="system-err" nillable="true" minOccurs="0" maxOccurs="1"/>
	}

	std::string outputFile = "TEST-" + group.getGroupName() + ".xml";
	auto path = outputFolder / outputFile;

	boost::property_tree::write_xml(
		path.toRawString(), report, std::locale(), boost::property_tree::xml_writer_settings<std::string>(' ', 4)
	);
}

} // namespace Balau::Testing::Reporters
