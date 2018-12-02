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
/// @file Containers.hpp
///
/// Utilities for containers.
///

#ifndef COM_BORA_SOFTWARE__BALAU_UTIL__CONTAINERS
#define COM_BORA_SOFTWARE__BALAU_UTIL__CONTAINERS

#include <Balau/Type/ToString.hpp>

#include <cstring>
#include <memory>
#include <vector>

namespace Balau::Util {

///
/// Utilities for containers.
///
struct Containers final {
	///////////////////// Miscellaneous utility functions /////////////////////

	///
	/// Appends the source container to the destination container.
	///
	template <template <typename ...> class DstT, typename ... D, template <typename ...> class SrcT, typename ... S>
	static void append(DstT<D ...> & dst, const SrcT<S ...> & src) {
		dst.insert(std::end(dst), std::begin(src), std::end(src));
	}

	///
	/// Perform a map operator (transform the input elements to a container of different elements).
	///
	template <typename D, typename S, template <typename ...> class ContainerT, typename ... DC, typename ... SC>
	static ContainerT<D, DC ...> map(const ContainerT<S, SC ...> & input, std::function<D (const S &)> f) {
		ContainerT<D, DC ...> ret;
		std::transform(input.begin(), input.end(), std::back_inserter(ret), f);
		return ret;
	}

	///////////////////////////////////////////////////////////////////////////

	Containers() = delete;
	Containers(const Containers &) = delete;
	Containers & operator = (const Containers &) = delete;
};

} // namespace Balau::Util

#endif // COM_BORA_SOFTWARE__BALAU_UTIL__CONTAINERS
