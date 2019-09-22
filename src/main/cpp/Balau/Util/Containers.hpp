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
	/// Appends the source containers to the destination container.
	///
	template <template <typename ...> class DstT, typename ... D, template <typename ...> class SrcT, typename ... S, template <typename ...> class ... SrcMT, typename ... SM>
	static void append(DstT<D ...> & dst, const SrcT<S ...> & src, const SrcMT<SM ...> & ... srcMore) {
		dst.insert(std::end(dst), std::begin(src), std::end(src));
		append(dst, srcMore ...);
	}

	///
	/// Appends the source container to the destination container.
	///
	template <template <typename ...> class DstT, typename ... D, template <typename ...> class SrcT, typename ... S>
	static void append(DstT<D ...> & dst, const SrcT<S ...> & src) {
		dst.insert(std::end(dst), std::begin(src), std::end(src));
	}

	///
	/// Concatenates the containers to form a new container. The type of the resulting container is the same as the first source container.
	///
	template <template <typename ...> class ContainerT, typename ... C, template <typename ...> class ... ContainerMT, typename ... M>
	static ContainerT<C ...> concatenate(const ContainerT<C ...> & c, const ContainerMT<M ...> & ... cMore) {
		ContainerT<C ...> dst;
		append(dst, c, cMore ...);
		return dst;
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

	///
	/// Does the container contain the supplied value?
	///
	template <template <typename ...> class C, typename ... T, typename U>
	static bool contains(C<T ...> & container, const U & value) {
		return std::find(container.begin(), container.end(), value) != container.end();
	}

	///
	/// Does the container contain one of the supplied values?
	///
	template <template <typename ...> class C, typename ... T, template <typename ...> class V, typename ... U>
	static bool contains(C<T ...> & container, const V<U ...> & values) {
		for (const auto & value : values) {
			if (std::find(container.begin(), container.end(), value) != container.end()) {
				return true;
			}
		}

		return false;
	}

	///////////////////////////////////////////////////////////////////////////

	Containers() = delete;
	Containers(const Containers &) = delete;
	Containers & operator = (const Containers &) = delete;
};

} // namespace Balau::Util

#endif // COM_BORA_SOFTWARE__BALAU_UTIL__CONTAINERS
