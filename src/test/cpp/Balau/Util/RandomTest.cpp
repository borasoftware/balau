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
#include <Balau/Util/Random.hpp>

namespace Balau {

using Testing::is;

namespace Util {

struct RandomTest : public Testing::TestGroup<RandomTest> {
	RandomTest() {
		RegisterTestCase(test);
	}

	void test() {
		UniformDouble gen(0.0, 1.0, 0);

		const std::array<double, 10> expectedValues = {
			  0.59284461651668274
			, 0.8442657442565985
			, 0.8579456199898301
			, 0.84725173738433146
			, 0.62356369649610843
			, 0.38438170837375674
			, 0.29753460535723425
			, 0.056712975933163677
			, 0.27265629474158937
			, 0.47766511174464643
		};

		for (double expected : expectedValues) {
			const double actual = gen();
			AssertThat(actual, is(expected));
		}
	}
};

} // namespace Util

} // namespace Balau
