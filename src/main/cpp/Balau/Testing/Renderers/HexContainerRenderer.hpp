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
/// @file HexContainerRenderer.hpp
///
/// Overload for rendering numeric containers as hex values.
///

#ifndef COM_BORA_SOFTWARE__BALAU_TESTING_TEST_RENDERERS__HEX_CONTAINER_RENDERER
#define COM_BORA_SOFTWARE__BALAU_TESTING_TEST_RENDERERS__HEX_CONTAINER_RENDERER

#include <Balau/Testing/Impl/HexContainerRendererImpl.hpp>

namespace Balau::Testing::TestRenderers {

///
/// Overload for rendering numeric containers as hex values.
///
template <typename A, typename E, template <typename> class ContainerA, template <typename> class ContainerE>
std::string render(const ContainerA<A> & actual, const ContainerE<E> & expected) {
	return Impl::renderAsHex(actual, expected);
}

} // namespace Balau::Testing::TestRenderers

#endif // COM_BORA_SOFTWARE__BALAU_TESTING_TEST_RENDERERS__HEX_CONTAINER_RENDERER
