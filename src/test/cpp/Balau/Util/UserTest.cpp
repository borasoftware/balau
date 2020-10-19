// @formatter:off
//
// Balau core C++ library
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
#include <Balau/Util/User.hpp>

namespace Balau {

using Testing::is;

namespace Util {

// This test is currently only implemented for Unix type OSes and relies on the HOME environment variable.
struct UserTest : public Testing::TestGroup<UserTest> {
	UserTest() {
		RegisterTest(homeDirectory);
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
