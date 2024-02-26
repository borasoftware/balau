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

#ifndef COM_BORA_SOFTWARE_BALAU_TESTING_IMPL_HEX_CONTAINER_RENDERER
#define COM_BORA_SOFTWARE_BALAU_TESTING_IMPL_HEX_CONTAINER_RENDERER

#include <Balau/Type/ToString.hpp>

#include <iomanip>

namespace Balau::Testing::Impl {

// Implementation of numeric container renders which render their numeric values as hex.
template <typename A, typename E, template <typename> class ContainerA, template <typename> class ContainerE>
std::string renderAsHex(const ContainerA<A> & actual, const ContainerE<E> & expected) {
	const size_t actualSize = actual.size();
	const size_t expectedSize = expected.size();
	const size_t size = actualSize > expectedSize ? actualSize : expectedSize;

	std::stringstream builder;

	size_t firstColumnWidth = 0;

	for (size_t m = 0; m < size; m++) {
		size_t len;

		len = ::toString(m).length();
		if (firstColumnWidth < len) {
			firstColumnWidth = len;
		}
	}

	builder << std::right << std::setw((int) firstColumnWidth) << "#" << "  ACTUAL      EXPECTED\n";

	for (size_t m = 0; m < size; m++) {
		builder << std::right << std::setw((int) firstColumnWidth) << m << ":  ";

		if (m < actualSize) {
			builder << std::right << "   " << std::setw(2) << std::hex << std::setfill('0') << (int) actual[m];
		} else {
			builder << "  ";
		}

		if (m < actualSize && m < expectedSize && actual[m] == expected[m]) {
			builder << "  ==  ";
		} else {
			builder << "  !=  ";
		}

		if (m < expectedSize) {
			builder << std::right << std::setw(2) << std::hex << std::setfill('0') << (int) expected[m];
		}

		builder << "\n";
	}

	builder << "\n";

	return builder.str();
}

} // namespace Balau::Testing::Impl

#endif // COM_BORA_SOFTWARE_BALAU_TESTING_IMPL_HEX_CONTAINER_RENDERER
