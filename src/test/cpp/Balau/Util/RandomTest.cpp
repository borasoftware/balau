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
#include <Balau/Util/Random.hpp>

namespace Balau {

using Testing::is;

namespace Util {

struct RandomTest : public Testing::TestGroup<RandomTest> {
	RandomTest() {
		RegisterTest(test);
	}

	void test() {
		UniformDouble gen(0.0, 1.0, 0);

		for (size_t m = 0; m < 1000; m++) {
			const double actual = gen();
			AssertThat(actual, isGreaterThanOrEqualTo(0.0));
			AssertThat(actual, isLessThanOrEqualTo(1.0));
		}
	}
};

} // namespace Util

} // namespace Balau
