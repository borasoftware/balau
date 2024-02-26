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

///
/// @file HexUnsignedLongLongRenderer.hpp
///
/// Overload for rendering unsigned long long containers as hex values.
///

#ifndef COM_BORA_SOFTWARE_BALAU_TESTING_TEST_RENDERERS_HEX_UNSIGNED_LONG_LONG_RENDERER
#define COM_BORA_SOFTWARE_BALAU_TESTING_TEST_RENDERERS_HEX_UNSIGNED_LONG_LONG_RENDERER

#include <Balau/Testing/Impl/HexContainerRendererImpl.hpp>

namespace Balau::Testing::TestRenderers {

///
/// Overload for rendering unsigned long long containers as hex values.
///
template <template <typename> class ContainerA, template <typename> class ContainerE>
std::string render(const ContainerA<unsigned long long> & actual, const ContainerE<unsigned long long> & expected) {
	return Impl::renderAsHex(actual, expected);
}

} // namespace Balau::Testing::TestRenderers

#endif // COM_BORA_SOFTWARE_BALAU_TESTING_TEST_RENDERERS_HEX_UNSIGNED_LONG_LONG_RENDERER
