// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include <TestResources.hpp>
#include <Balau/Util/App.hpp>

namespace Balau {

using Testing::is;

namespace Util {

// This test is currently only implemented for Unix type OSes and relies on the XDG or HOME environment variables.
struct AppTest : public Testing::TestGroup<AppTest> {
	AppTest() {
		registerTest(&AppTest::globalAppDataDirectory, "globalAppDataDirectory");
		registerTest(&AppTest::userAppDataDirectory,   "userAppDataDirectory");
		registerTest(&AppTest::userAppConfigDirectory, "userAppConfigDirectory");
	}

	void globalAppDataDirectory() {
		const boost::filesystem::path programLocation = boost::dll::program_location();

		const auto actual = App::getGlobalDataResourcesDirectory("ACompanyName", "AnApplication");
		const auto expected = Resource::File(programLocation.parent_path().parent_path()) / "share" / "ACompanyName" / "AnApplication";

		AssertThat(actual, is(expected));
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
};

} // namespace Util

} // namespace Balau
