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
/// @file Uri.hpp
///
/// The abstract URI base class.
///

#ifndef COM_BORA_SOFTWARE__BALAU_RESOURCE__URI
#define COM_BORA_SOFTWARE__BALAU_RESOURCE__URI

#include <Balau/Dev/Assert.hpp>
#include <Balau/Resource/UriDispatcher.hpp>
#include <Balau/Type/StdTypes.hpp>

#include <memory>

namespace Balau::Resource {

class Uri;
class ByteReadResource;
class ByteWriteResource;
class Utf8To32ReadResource;
class Utf32To8WriteResource;

///
/// An abstract read iterator that iterates recursively.
///
class RecursiveUriIterator {
	///
	/// Returns true if there is another item available in the iterator.
	///
	/// @return true if there is another item available in the iterator
	/// @deprecated
	///
	public: virtual bool hasNext() const = 0;

	///
	/// Get the next item.
	///
	/// @return the next item
	///
	public: virtual std::unique_ptr<Uri> next() = 0;

	///
	/// Destroy the iterator instance.
	///
	public: virtual ~RecursiveUriIterator() = default;

	///
	/// Is this iterator equal to the supplied iterator.
	///
	/// @return true if this iterator is equal to the supplied iterator
	///
//	public: virtual bool operator == (const RecursiveUriIterator & rhs) const = 0;

	///
	/// Is this iterator equal to the supplied iterator.
	///
	/// @return true if this iterator is equal to the supplied iterator
	///
//	public: virtual RecursiveUriIterator operator ++(const RecursiveUriIterator & rhs) const = 0;

	///
	/// Is this iterator equal to the supplied iterator.
	///
	/// @return true if this iterator is equal to the supplied iterator
	///
//	public: virtual bool operator == (const RecursiveUriIterator & rhs) const = 0;

};

///
/// An abstract read iterator.
///
class UriIterator {
	///
	/// Returns true if there is another item available in the iterator.
	///
	/// @return true if there is another item available in the iterator
	/// @deprecated
	///
	public: virtual bool hasNext() const = 0;

	///
	/// Get the next item.
	///
	/// @return the next item
	///
	public: virtual std::unique_ptr<Uri> next() = 0;

	///
	/// Destroy the iterator instance.
	///
	public: virtual ~UriIterator() = default;

	///
	/// Is this iterator equal to the supplied iterator.
	///
	/// @return true if this iterator is equal to the supplied iterator
	///
//	public: virtual bool operator == (const UriIterator & rhs) const = 0;

	///
	/// Is this iterator equal to the supplied iterator.
	///
	/// @return true if this iterator is equal to the supplied iterator
	///
//	public: virtual RecursiveUriIterator operator ++(const UriIterator & rhs) const = 0;

	///
	/// Is this iterator equal to the supplied iterator.
	///
	/// @return true if this iterator is equal to the supplied iterator
	///
//	public: virtual bool operator == (const UriIterator & rhs) const = 0;

};

///
/// An abstract universal resource identifier.
///
class Uri {
	///
	/// Clone the concrete Uri.
	///
	/// @return a clone of the URI
	///
	public: virtual std::unique_ptr<Uri> clone() const = 0;

	///
	/// Clone the concrete Uri to a shared pointer.
	///
	/// @return a clone of the URI in a shared pointer
	///
	public: std::shared_ptr<Uri> cloneToShared() const {
		return clone();
	}

	///
	/// Appends the path component to the supplied URI, returning a new URI.
	///
	/// If the URI type does not permit appending of path components, UnsupportedOperationException is thrown.
	///
	public: virtual std::unique_ptr<Uri> append(const std::string & pathComponent) const = 0;

	///
	/// Resolve the relative or absolute path, in reference to the current URI.
	///
	/// If the supplied path contains a scheme prefix, the URI fromString function will be
	/// called to create a new unrelated URI instance.
	///
	/// This method will determine the base path to use by applying the following rules.
	///
	///  - If the URI path ends with a forward slash (or is a directory for file URIs), the
	///    base path is the URI path.
	///
	///  - Otherwise, the rightmost path element is stripped from the URI path to form
	///    the base path.
	///
	/// @param path a relative or absolute path, or a full URI
	/// @return a new URI
	///
	public: virtual std::unique_ptr<Uri> resolve(std::string_view path) const = 0;

	///
	/// Returns true if the URI points to a file directory.
	///
	/// @return true if the URI points to a file directory
	///
	public: virtual bool isRegularDirectory() const = 0;

	///
	/// Returns true if the URI is a regular file.
	///
	/// @return true if the URI is a regular file
	///
	public: virtual bool isRegularFile() const = 0;

	///
	/// Get a string representing the URI, complete with scheme.
	///
	/// @return a string representing the URI, complete with scheme
	///
	public: virtual std::string toUriString() const = 0;

	///
	/// Get a string representing the raw URI. This may or may not be the same
	/// as the string returned by toUriString().
	///
	/// @return a string representing the raw URI
	///
	public: virtual std::string toRawString() const = 0;

	///
	/// Get the URI's hash code.
	///
	/// @return the hash code
	///
	public: virtual size_t hashcode() const noexcept = 0;

	///
	/// Can data be read from the URI via a read resource.
	///
	public: virtual bool canReadFrom() const = 0;

	///
	/// Can data be written to the URI via a write resource.
	///
	public: virtual bool canWriteTo() const = 0;

	///
	/// Get a byte read resource for the URI.
	///
	/// @throw NotImplementedException if the URI does not support reading
	/// @throw ResourceException if the resource could not be created
	///
	public: virtual std::unique_ptr<ByteReadResource> byteReadResource() const = 0;

	///
	/// Get a UTF-8 to UTF-32 read resource for the URI.
	///
	/// @throw NotImplementedException if the URI does not support reading
	/// @throw ResourceException if the resource could not be created
	///
	public: virtual std::unique_ptr<Utf8To32ReadResource> utf8To32ReadResource() const = 0;

	///
	/// Get a byte write resource for the URI.
	///
	/// @throw NotImplementedException if the URI does not support writing
	/// @throw ResourceException if the resource could not be created
	///
	public: virtual std::unique_ptr<ByteWriteResource> byteWriteResource() = 0;

	///
	/// Get a UTF-32 to UTF-8 write resource for the URI.
	///
	/// @throw NotImplementedException if the URI does not support writing
	/// @throw ResourceException if the resource could not be created
	///
	public: virtual std::unique_ptr<Utf32To8WriteResource> utf32To8WriteResource() = 0;

	///
	/// Does the URI have a recursive iterator (examples: file and zip archive URIs).
	///
	public: virtual bool isRecursivelyIterable() const = 0;

	///
	/// Does the URI have a non-recursive iterator (examples: file and zip archive URIs).
	///
	public: virtual bool isIterable() const = 0;

	///
	/// Get a recursive iterator.
	///
	/// This will throw a NotImplementedException if the URI does not have a recursive iterator.
	///
	public: virtual std::unique_ptr<RecursiveUriIterator> recursiveIterator() const = 0;

	///
	/// Get a (non-recursive) iterator.
	///
	/// This will throw a NotImplementedException if the URI does not have an iterator.
	///
	public: virtual std::unique_ptr<UriIterator> iterator() const = 0;

	///
	/// Compare the supplied URI to the current URI.
	///
	/// @return true if the supplied URI is equal to the current URI.
	///
	public: virtual bool operator == (const Uri & rhs) const = 0;

	///
	/// Destroy the URI instance.
	///
	public: virtual ~Uri() = default;

	///
	/// Visitor pattern dispatching.
	///
	/// @param visitor the visitor to visit
	///
	public: virtual void dispatch(UriDispatcher & visitor) const = 0;
};

///
/// Appends the path component to the supplied URI, returning a new URI.
///
/// If the URI type does not permit appending of path components, UnsupportedOperationException is thrown.
///
inline std::unique_ptr<Uri> operator / (const std::unique_ptr<Uri> & uri, const std::string & pathComponent) {
	return uri->append(pathComponent);
}

///
/// Print the URI as a UTF-8 string.
///
/// This function does not normalise the URI.
///
/// @return a UTF-8 string representing the URI
///
inline std::string toString(const Uri & uri) {
	return uri.toRawString();
}

///
/// Create a unique pointer URI from the supplied string.
///
/// @param uri the unique pointer container to place the new URI instance into
/// @param value the input string
///
void fromString(std::unique_ptr<Uri> & uri, std::string_view value);

///
/// Create a shared pointer URI from the supplied string.
///
/// @param uri the shared pointer container to place the new URI instance into
/// @param value the input string
///
inline void fromString(std::shared_ptr<Uri> & uri, std::string_view value) {
	std::unique_ptr<Uri> ptr;
	fromString(ptr, value);
	uri = std::shared_ptr<Uri>(std::move(ptr));
}

///
/// Create a shared const pointer URI from the supplied string.
///
/// @param uri the shared pointer container to place the new URI instance into
/// @param value the input string
///
inline void fromString(std::shared_ptr<const Uri> & uri, std::string_view value) {
	std::unique_ptr<Uri> ptr;
	fromString(ptr, value);
	uri = std::shared_ptr<const Uri>(std::move(ptr));
}

} // namespace Balau::Resource

#endif // COM_BORA_SOFTWARE__BALAU_RESOURCE__URI
