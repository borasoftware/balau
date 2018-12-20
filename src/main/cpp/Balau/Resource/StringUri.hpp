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
/// @file StringUri.hpp
///
/// An immediate string pseudo-URI.
///

#ifndef COM_BORA_SOFTWARE__BALAU_RESOURCE__STRING_URI
#define COM_BORA_SOFTWARE__BALAU_RESOURCE__STRING_URI

#include <Balau/Exception/ResourceExceptions.hpp>
#include <Balau/Resource/StringUriByteReadResource.hpp>
#include <Balau/Resource/StringUriByteWriteResource.hpp>
#include <Balau/Resource/StringUriUtf8To32ReadResource.hpp>
#include <Balau/Resource/StringUriUtf32To8WriteResource.hpp>
#include <Balau/Resource/Uri.hpp>
#include <Balau/Util/Strings.hpp>

namespace Balau::Resource {

///
/// An immediate string pseudo-URI.
///
/// Unlike other resource URIs, the String URI is a pseudo-URI that specifies a
/// immediate data available from the read resource streams. As such, String URIs
/// do not designate a remote resource that will be read. Instead, string data
/// internal to the String URI will be implicitly read.
///
/// String URIs are a convenience mechanism that allows the abstraction of a byte,
/// UTF-8, or UTF-32 data source to be specified via a pointer to the abstract URI
/// class. They are thus not compliant with the Universal Resource Identifier
/// specification.
///
/// The URI of a String URI is always "string:". Note that String URI equality is
/// based on the internal data and not the URI contents (which is identical for
/// all String UIRs).
///
/// @todo Create String URI write resources
///
class StringUri : public Uri {
	///
	/// Create an empty String URI.
	///
	/// The internal string can be appended by calling the append() method.
	///
	public: StringUri() = default;

	///
	/// Create a String URI with the supplied string data.
	///
	public: explicit StringUri(std::string data_)
		: data(std::move(data_)) {}

	///
	/// Create an HTTP URI by copying the supplied instance.
	///
	public: StringUri(const StringUri & copy) = default;

	///
	/// Create an HTTP URI by moving the contents of the supplied instance.
	///
	public: StringUri(StringUri && rhs) noexcept = default;

	///
	/// Append the supplied text to the internal string data.
	///
	/// @param text a view onto the text to append
	/// @return the current object, allowing chaining calls to be made
	///
	public: StringUri & append(std::string_view text) {
		data += text;
		return *this;
	}

	///
	/// Append the supplied ascii character to the internal string data.
	///
	/// @param c the character to append
	/// @return the current object, allowing chaining calls to be made
	///
	public: StringUri & append(char c) {
		data += c;
		return *this;
	}

	///
	/// Append the supplied code point to the internal string data.
	///
	/// @param c the code point to append
	/// @return the current object, allowing chaining calls to be made
	///
	public: StringUri & append(char32_t c) {
		Util::Strings::append(data, c, 1);
		return *this;
	}

	///
	/// Get a const reference to the string data of the String URI.
	///
	/// @return a reference to the string data
	///
	public: const std::string & getString() const {
		return data;
	}

	public: bool isRegularDirectory() const override {
		return false;
	}

	public: bool isRegularFile() const override {
		return false;
	}

	public: std::string toUriString() const override {
		return "string:";
	}

	public: std::string toRawString() const override {
		return data;
	}

	public: size_t hashcode() const noexcept override {
		return std::hash<std::string>()(data);
	}

	public: std::unique_ptr<Uri> clone() const override {
		return std::unique_ptr<Uri>(new StringUri(*this));
	}

	public: std::unique_ptr<Uri> resolve(std::string_view path) const override {
		static const std::regex scheme { "[a-zA-Z][a-zA-Z0-9+-\\.]*:" };

		auto cleanPath = Util::Strings::trim(path);
		auto str = std::string(cleanPath);

		if (Util::Strings::startsWithRegex(str, scheme)) {
			std::unique_ptr<Uri> uri;
			fromString(uri, str);
			return uri;
		}

		ThrowBalauException(
			  Exception::IllegalArgumentException
			, "StringUri does not support calls to the resolve method without specifying a schema in the path."
		);
	}

	public: bool operator == (const Uri & rhs) const override {
		const auto * o = dynamic_cast<const StringUri *>(&rhs);
		return o != nullptr && data == o->data;
	}

	public: bool canReadFrom() const override {
		return true;
	}

	public: bool canWriteTo() const override {
		return false;
	}

	///
	/// Get a byte read resource for the HTTP source.
	///
	public: StringUriByteReadResource getByteReadResource() {
		return StringUriByteReadResource(*this);
	}

	///
	/// Get a UTF-8 to UTF-32 read resource for the HTTP source.
	///
	public: StringUriUtf8To32ReadResource getUtf8To32ReadResource() {
		return StringUriUtf8To32ReadResource(*this);
	}

	public: std::unique_ptr<ByteReadResource> byteReadResource() override {
		return std::unique_ptr<ByteReadResource>(new StringUriByteReadResource(*this));
	}

	public: std::unique_ptr<Utf8To32ReadResource> utf8To32ReadResource() override {
		return std::unique_ptr<Utf8To32ReadResource>(new StringUriUtf8To32ReadResource(*this));
	}

	public: std::unique_ptr<ByteWriteResource> byteWriteResource() override {
		return std::unique_ptr<ByteWriteResource>(new StringUriByteWriteResource(*this));
	}

	public: std::unique_ptr<Utf32To8WriteResource> utf32To8WriteResource() override {
		return std::unique_ptr<Utf32To8WriteResource>(new StringUriUtf32To8WriteResource(*this));
	}

	public: bool isRecursivelyIterable() const override {
		return false;
	}

	public: std::unique_ptr<RecursiveUriIterator> recursiveIterator() override {
		ThrowBalauException(Exception::NotImplementedException, "String URIs do not have a recursive iterator");
	}

	public: void dispatch(UriDispatcher & dispatcher) const override {
		dispatcher.dispatch(*this);
	}

	private: std::string data;
};

} // namespace Balau::Resource

#endif // COM_BORA_SOFTWARE__BALAU_RESOURCE__STRING_URI
