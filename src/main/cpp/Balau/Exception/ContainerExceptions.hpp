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
/// @file ContainerExceptions.hpp
///
/// %Balau exceptions for containers.
///

#ifndef COM_BORA_SOFTWARE__BALAU_EXCEPTION__CONTAINER_EXCEPTIONS
#define COM_BORA_SOFTWARE__BALAU_EXCEPTION__CONTAINER_EXCEPTIONS

#include <Balau/Exception/BalauException.hpp>

namespace Balau::Exception {

///
/// Thrown when a specified index is not in the valid range.
///
class IndexOutOfRangeException : public BalauException {
	public: const size_t index;

	public: IndexOutOfRangeException(SourceCodeLocation location, const std::string & st, size_t index_)
		: BalauException(location, st, "IndexOutOfRange", ::toString(index_))
		, index(index_) {}

	public: IndexOutOfRangeException(const std::string & st, size_t index_)
		: BalauException(st, "IndexOutOfRange", ::toString(index_))
		, index(index_) {}

	public: IndexOutOfRangeException(SourceCodeLocation location, const std::string & st, const std::string & text, size_t index_)
		: BalauException(location, st, "IndexOutOfRange", text + " - " + ::toString(index_))
		, index(index_) {}

	public: IndexOutOfRangeException(const std::string & st, const std::string & text, size_t index_)
		: BalauException(st, "IndexOutOfRange", text + " - " + ::toString(index_))
		, index(index_) {}
};

inline bool operator == (const IndexOutOfRangeException & lhs, const IndexOutOfRangeException & rhs) {
	return lhs.message == rhs.message && lhs.index == rhs.index;
}

///
/// Thrown when a request is made for an element but no elements are available.
///
class EmptyException : public BalauException {
	public: EmptyException(SourceCodeLocation location, const std::string & st, const std::string & text)
		: BalauException(location, st, "Empty", text) {}

	public: EmptyException(const std::string & st, const std::string & text)
		: BalauException(st, "Empty", text) {}
};

///
/// Thrown when an invalid size is supplied or detected.
///
class SizeException : public BalauException {
	public: SizeException(SourceCodeLocation location, const std::string & st, const std::string & text)
		: BalauException(location, st, "Size", text) {}

	public: SizeException(const std::string & st, const std::string & text)
		: BalauException(st, "Size", text) {}
};

///
/// Thrown when an attempt is made to add an existing item to a container.
///
template <typename T> class ItemExistsException : public BalauException {
	public: const T item;

	public: ItemExistsException(SourceCodeLocation location, const std::string & st, T item_, const std::string & text)
		: BalauException(location, st, "ItemExists", text)
		, item(item_) {}

	public: ItemExistsException(const std::string & st, T item_, const std::string & text)
		: BalauException(st, "ItemExists", text)
		, item(item_) {}
};

///
/// Thrown when an attempt is made to remove a non-existing item in a container.
///
template <typename T> class ItemDoesNotExistException : public BalauException {
	public: const T item;

	public: ItemDoesNotExistException(SourceCodeLocation location, const std::string & st, T item_, const std::string & text)
		: BalauException(location, st, "ItemDoesNotExist", text)
		, item(item_) {}

	public: ItemDoesNotExistException(const std::string & st, T item_, const std::string & text)
		: BalauException(st, "ItemDoesNotExist", text)
		, item(item_) {}
};

///
/// Thrown when a non-existent relationship between two items is requested.
///
template <typename T, typename U> class RelationshipDoesNotExistException : public BalauException {
	public: const T item1;
	public: const U item2;

	public: RelationshipDoesNotExistException(SourceCodeLocation location, const std::string & st, T item1_, U item2_, const std::string & text)
		: BalauException(location, st, "RelationshipDoesNotExist", text)
		, item1(item1_)
		, item2(item2_) {}

	public: RelationshipDoesNotExistException(const std::string & st, T item1_, U item2_, const std::string & text)
		: BalauException(st, "RelationshipDoesNotExist", text)
		, item1(item1_)
		, item2(item2_) {}
};

} // namespace Balau::Exception

#endif // COM_BORA_SOFTWARE__BALAU_EXCEPTION__CONTAINER_EXCEPTIONS
