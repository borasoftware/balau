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

	public: IndexOutOfRangeException(const char * file, int line, const std::string & st, size_t index_)
		: BalauException(file, line, st, "IndexOutOfRange", ::toString(index_))
		, index(index_) {}

	public: IndexOutOfRangeException(const char * file, int line, const std::string & st, const std::string & text, size_t index_)
		: BalauException(file, line, st, "IndexOutOfRange", text + " - " + ::toString(index_))
		, index(index_) {}
};

inline bool operator == (const IndexOutOfRangeException & lhs, const IndexOutOfRangeException & rhs) {
	return lhs.message == rhs.message && lhs.index == rhs.index;
}

///
/// Thrown when a request is made for an element but no elements are available.
///
class EmptyException : public BalauException {
	public: EmptyException(const char * file, int line, const std::string & st, const std::string & text)
		: BalauException(file, line, st, "Empty", text) {}
};

///
/// Thrown when an invalid size is supplied or detected.
///
class SizeException : public BalauException {
	public: SizeException(const char * file, int line, const std::string & st, const std::string & text)
		: BalauException(file, line, st, "Size", text) {}
};

///
/// Thrown when an attempt is made to add an existing item to a container.
///
template <typename T> class ItemExistsException : public BalauException {
	public: const T item;

	public: ItemExistsException(const char * file, int line, const std::string & st, T item_, const std::string & text)
		: BalauException(file, line, st, "ItemExists", text)
		, item(item_) {}
};

///
/// Thrown when an attempt is made to remove a non-existing item in a container.
///
template <typename T> class ItemDoesNotExistException : public BalauException {
	public: const T item;

	public: ItemDoesNotExistException(const char * file, int line, const std::string & st, T item_, const std::string & text)
		: BalauException(file, line, st, "ItemDoesNotExist", text)
		, item(item_) {}
};

///
/// Thrown when a non-existent relationship between two items is requested.
///
template <typename T, typename U> class RelationshipDoesNotExistException : public BalauException {
	public: const T item1;
	public: const U item2;

	public: RelationshipDoesNotExistException(const char * file, int line, const std::string & st, T item1_, U item2_, const std::string & text)
		: BalauException(file, line, st, "RelationshipDoesNotExist", text)
		, item1(item1_)
		, item2(item2_) {}
};

} // namespace Balau::Exception

#endif // COM_BORA_SOFTWARE__BALAU_EXCEPTION__CONTAINER_EXCEPTIONS
