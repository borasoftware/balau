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
#include <Balau/Type/UUID.hpp>

namespace Balau {

using Testing::is;

struct UUIDTest : public Testing::TestGroup<UUIDTest> {
	UUIDTest() {
		RegisterTest(test);
	}

	void test() {
		const std::array<unsigned char, 16> bytes = {
			0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xa, 0xb, 0xc, 0xd, 0xe, 0xf
		};

		UUID uuid(bytes.data());
		log("UUID = ", uuid);

		AssertThat(uuid.asArray(), is(bytes));
	}
};

} // namespace Balau
