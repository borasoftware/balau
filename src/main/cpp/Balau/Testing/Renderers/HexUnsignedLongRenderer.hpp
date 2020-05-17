// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

///
/// @file HexUnsignedLongRenderer.hpp
///
/// Overload for rendering unsigned long containers as hex values.
///

#ifndef COM_BORA_SOFTWARE__BALAU_TESTING_TEST_RENDERERS__HEX_UNSIGNED_LONG_RENDERER
#define COM_BORA_SOFTWARE__BALAU_TESTING_TEST_RENDERERS__HEX_UNSIGNED_LONG_RENDERER

#include <Balau/Testing/Impl/HexContainerRendererImpl.hpp>

namespace Balau::Testing::TestRenderers {

///
/// Overload for rendering unsigned long containers as hex values.
///
template <template <typename> class ContainerA, template <typename> class ContainerE>
std::string render(const ContainerA<unsigned long long> & actual, const ContainerE<unsigned long long> & expected) {
	return Impl::renderAsHex(actual, expected);
}

} // namespace Balau::Testing::TestRenderers

#endif // COM_BORA_SOFTWARE__BALAU_TESTING_TEST_RENDERERS__HEX_UNSIGNED_LONG_RENDERER
