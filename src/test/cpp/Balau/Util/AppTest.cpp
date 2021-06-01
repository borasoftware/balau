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
#include <Balau/Util/App.hpp>

namespace Balau {

using Testing::is;

namespace Util {

///
/// These tests are currently only implemented for Unix type OSes and use the XDG, HOME, and TMPDIR environment variables.
///
struct AppTest : public Testing::TestGroup<AppTest> {
	AppTest() {
		RegisterTestCase(userAppDataDirectory);
		RegisterTestCase(globalAppDataDirectory);
		RegisterTestCase(userAppConfigDirectory);
		RegisterTestCase(globalAppConfigDirectory);
		RegisterTestCase(applicationRuntimeDataDirectory);
	}

	void userAppDataDirectory() {
		const auto actual = App::getUserApplicationDataDirectory("ACompanyName", "AnApplication");

		const char * xdg = std::getenv("XDG_DATA_HOME");
		Resource::File expected;

		if (xdg != nullptr && !std::string(xdg).empty()) {
			expected = Resource::File(xdg) / "ACompanyName" / "AnApplication";
		} else {
			const char * home = std::getenv("HOME");

			if (home == nullptr || std::string(home).empty()) {
				AssertFail("Cannot get home directory for test purposes.");
			}

			expected = Resource::File(home) / ".local" / "share" / "ACompanyName" / "AnApplication";
		}

		AssertThat(actual, is(expected));
	}

	void globalAppDataDirectory() {
		const boost::filesystem::path programLocation = boost::dll::program_location();

		const auto actual = App::getGlobalApplicationDataDirectory("ACompanyName", "AnApplication");
		const auto expected = Resource::File(programLocation.parent_path().parent_path()) / "share" / "ACompanyName" / "AnApplication" / "data";

		AssertThat(actual, is(expected));
	}

	void userAppConfigDirectory() {
		const auto actual = App::getUserApplicationConfigDirectory("ACompanyName", "AnApplication");

		const char * xdg = std::getenv("XDG_CONFIG_HOME");
		Resource::File expected;

		if (xdg != nullptr && !std::string(xdg).empty()) {
			expected = Resource::File(xdg) / "ACompanyName" / "AnApplication";
		} else {
			const char * home = std::getenv("HOME");

			if (home == nullptr || std::string(home).empty()) {
				AssertFail("Cannot get home directory for test purposes.");
			}

			expected = Resource::File(home) / ".config" / "ACompanyName" / "AnApplication";
		}

		AssertThat(actual, is(expected));
	}

	void globalAppConfigDirectory() {
		const boost::filesystem::path programLocation = boost::dll::program_location();

		const auto actual = App::getGlobalApplicationConfigDirectory("ACompanyName", "AnApplication");
		const auto expected = Resource::File(programLocation.parent_path().parent_path()) / "share" / "ACompanyName" / "AnApplication" / "config";

		AssertThat(actual, is(expected));
	}

	void applicationRuntimeDataDirectory() {
		const auto actual = App::getApplicationRuntimeDataDirectory("ACompanyName", "AnApplication");

		const char * xdg = std::getenv("XDG_RUNTIME_DIR");
		const char * tmpdir = std::getenv("TMPDIR");
		const auto username = User::getUserName();
		Resource::File expected;

		if (xdg != nullptr && !Strings::trim(xdg).empty()) {
			expected = Resource::File(xdg) / "ACompanyName" / "AnApplication";
		} else if (tmpdir != nullptr && !Strings::trim(tmpdir).empty()) {
			expected = Resource::File(tmpdir) / "ACompanyName" / "AnApplication";
		} else {
			expected = Resource::File("/tmp") / username / "ACompanyName" / "AnApplication";
		}

		AssertThat(actual, is(expected));
	}
};

} // namespace Util

} // namespace Balau
