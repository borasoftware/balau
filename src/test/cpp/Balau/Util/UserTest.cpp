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
#include <Balau/Util/User.hpp>

namespace Balau {

using Testing::is;

namespace Util {

// This test is currently only implemented for Unix type OSes and relies on the HOME environment variable.
struct UserTest : public Testing::TestGroup<UserTest> {
	UserTest() {
		registerTest(&UserTest::homeDirectory, "homeDirectory");
	}

	void homeDirectory() {
		const char * home = std::getenv("HOME");

		if (home == nullptr || std::string(home).empty()) {
			AssertFail("Cannot get home directory for test purposes.");
		}

		AssertThat(User::getHomeDirectory(), is(Resource::File(home)));
	}
};

} // namespace Util

} // namespace Balau
