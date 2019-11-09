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

///
/// These tests are currently only implemented for Unix type OSes and use the XDG, HOME, and TMPDIR environment variables.
///
struct AppTest : public Testing::TestGroup<AppTest> {
	AppTest() {
		registerTest(&AppTest::userAppDataDirectory,            "userAppDataDirectory");
		registerTest(&AppTest::globalAppDataDirectory,          "globalAppDataDirectory");
		registerTest(&AppTest::userAppConfigDirectory,          "userAppConfigDirectory");
		registerTest(&AppTest::globalAppConfigDirectory,        "globalAppConfigDirectory");
		registerTest(&AppTest::applicationRuntimeDataDirectory, "applicationRuntimeDataDirectory");
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
