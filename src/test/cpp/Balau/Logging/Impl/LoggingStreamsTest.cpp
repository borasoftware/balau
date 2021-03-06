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

#include <TestResources.hpp>
#include <Balau/Logging/Impl/LoggingStreams.hpp>

namespace Balau {

using Testing::is;

namespace LoggingSystem {

struct LoggingStreamsTest : public Testing::TestGroup<LoggingStreamsTest> {
	LoggingStreamsTest() {
		// Work in progress.
		//RegisterTestCase(fileRotation);
	}

	class TestClock : public System::Clock {
		public: Date::year_month_day todayValue;

		public: explicit TestClock(Date::year_month_day todayValue_) : todayValue(todayValue_) {}

		public: std::chrono::system_clock::time_point now() const override {
			AssertFail("now() called");
			return std::chrono::system_clock::now(); // avoid compiler complaint
		}

		public: Date::year_month_day today() const override {
			return todayValue;
		}

		public: std::chrono::nanoseconds nanotime() const override {
			AssertFail("now() called");
			return std::chrono::nanoseconds(0); // avoid compiler complaint
		}

		public: std::chrono::microseconds microtime() const override {
			AssertFail("now() called");
			return std::chrono::microseconds(0); // avoid compiler complaint
		}

		public: std::chrono::milliseconds millitime() const override {
			AssertFail("now() called");
			return std::chrono::milliseconds(0); // avoid compiler complaint
		}

		public: std::chrono::centiseconds centitime() const override {
			AssertFail("now() called");
			return std::chrono::centiseconds(0); // avoid compiler complaint
		}

		public: std::chrono::deciseconds decitime() const override {
			AssertFail("now() called");
			return std::chrono::deciseconds(0); // avoid compiler complaint
		}
	};

	void fileRotation() {
		const Resource::File uriBase = TestResources::TestResultsFolder / "LoggingStreamsTest";

		const Date::year_month_day today = Date::year{2017}/02/20;
		const Date::year_month_day tomorrow = Date::year{2017}/02/21;

		std::shared_ptr<System::Clock> clock = std::shared_ptr<System::Clock>(new TestClock(today));
		TestClock & testClock = * (TestClock *) clock.get();

		// Create a file logging stream without a date component in the URI.
		FileLoggingStream loggingStream(clock, (uriBase / "notUsed.log").toUriString());

		// Then test the new stream method manually, by first manipulating the
		// pathComponents internal state and then calling the method directly.

		loggingStream.pathComponents = Util::Vectors::toStringVector(
			Util::Strings::split((uriBase / "test-${date}.log").toUriString(), FileLoggingStream::dateRegEx, false)
		);

		// First stream creation.
		loggingStream.createNewStream();
		loggingStream.write("1 Should be in first log file 1");

		// Should not create a new stream.
		loggingStream.createNewStream();
		loggingStream.write("2 Should be in first log file 2");

		testClock.todayValue = tomorrow;

		// Second stream creation.
		loggingStream.createNewStream();
		loggingStream.write("3 Should be in second log file 3");

		// Should not create a new stream.
		loggingStream.createNewStream();
		loggingStream.write("4 Should be in second log file 4");
	}
};

} // namespace LoggingSystem

} // namespace Balau
