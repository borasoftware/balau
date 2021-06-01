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
/// @file StringUri.hpp
///
/// An immediate string pseudo-URI.
///

#ifndef COM_BORA_SOFTWARE__BALAU_RESOURCE__STRING_URI
#define COM_BORA_SOFTWARE__BALAU_RESOURCE__STRING_URI

#include <Balau/Resource/Uri.hpp>
#include <Balau/Util/Strings.hpp>

#include <boost/iostreams/code_converter.hpp>
#include <boost/iostreams/stream.hpp>

#include <codecvt>
#include <memory>
#include <sstream>

namespace Balau::Resource {

class StringUriByteReadResource;
class StringUriByteWriteResource;
class StringUriUtf8To32ReadResource;

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
		data.append(text.data(), text.length());
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

	public: std::unique_ptr<Uri> append(const std::string & pathComponent) const override {
		ThrowBalauException(Exception::UnsupportedOperationException, "StringUri does not support path appending.");
	}

	public: void visit(UriVisitor & visitor) const override {
		visitor.visit(*this);
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
	public: StringUriByteReadResource getByteReadResource() const;

	///
	/// Get a UTF-8 to UTF-32 read resource for the HTTP source.
	///
	public: StringUriUtf8To32ReadResource getUtf8To32ReadResource() const;

	public: std::unique_ptr<ByteReadResource> byteReadResource() const override;

	public: std::unique_ptr<Utf8To32ReadResource> utf8To32ReadResource() const override;

	public: std::unique_ptr<ByteWriteResource> byteWriteResource() override;

	public: std::unique_ptr<Utf32To8WriteResource> utf32To8WriteResource() override;

	public: bool isRecursivelyIterable() const override {
		return false;
	}

	public: bool isIterable() const override {
		return false;
	}

	public: std::unique_ptr<RecursiveUriIterator> recursiveIterator() const override {
		ThrowBalauException(Exception::NotImplementedException, "String URIs do not have recursive iterators.");
	}

	public: std::unique_ptr<UriIterator> iterator() const override {
		ThrowBalauException(Exception::NotImplementedException, "String URIs do not have iterators.");
	}

	public: void dispatch(UriDispatcher & dispatcher) const override {
		dispatcher.dispatch(*this);
	}

	private: std::string data;
};

///
/// A read-only String URI resource which is read as bytes.
///
class StringUriByteReadResource : public ByteReadResource {
	///
	/// Create a new string byte read resource from the supplied String URI.
	///
	public: explicit StringUriByteReadResource(const StringUri & stringUri_)
		: stringUri(new StringUri(stringUri_))
		, stream(std::string(stringUri_.getString())) {}

	public: StringUriByteReadResource(StringUriByteReadResource && rhs) noexcept
		: stringUri(std::move(rhs.stringUri))
		, stream(std::move(rhs.stream)) {}

	public: std::istream & readStream() override {
		return stream;
	}

	public: const Uri & uri() const override {
		return *stringUri;
	}

	public: void close() override {}

	////////////////////////// Private implementation /////////////////////////

	private: std::unique_ptr<Uri> stringUri;
	private: std::istringstream stream;
};

///
/// A write only String URI that is written as bytes.
///
class StringUriByteWriteResource : public ByteWriteResource {
	///
	/// Create a new string Uri byte write resource from the supplied String URI.
	///
	public: explicit StringUriByteWriteResource(StringUri & stringUri_)
		: stringUri(std::make_unique<StringUri>(stringUri_))
		, stream(stringUri->getString()) {}

	public: StringUriByteWriteResource(StringUriByteWriteResource && rhs) noexcept
		: stringUri(std::move(rhs.stringUri))
		, stream(std::move(rhs.stream)) {}

	public: ~StringUriByteWriteResource() override {
		close();
	}

	public: std::ostream & writeStream() override {
		return stream;
	}

	public: const Uri & uri() const override {
		return *stringUri;
	}

	public: void close() override {}

	////////////////////////// Private implementation /////////////////////////

	private: std::unique_ptr<StringUri> stringUri;
	private: std::ostringstream stream;
};

///
/// A read-only String UTF-8 resource which is read as UTF-32 characters.
///
/// @todo Convert to UTF-32 characters on the fly instead of using toString32.
///
class StringUriUtf8To32ReadResource : public Utf8To32ReadResource {
	private: using idevice_utf8_utf32 = boost::iostreams::code_converter<std::istream, std::codecvt_utf8<char32_t, 0x10ffff, std::consume_header>>;
	private: using istream_utf8_utf32 = boost::iostreams::stream<idevice_utf8_utf32>;

	///
	/// Create a new String URI UTF-8 to UTF-32 read resource from the supplied String URI.
	///
	public: explicit StringUriUtf8To32ReadResource(const StringUri & stringUri_)
		: stringUri(std::make_unique<StringUri>(stringUri_))
		, utf8Stream(new std::istringstream(stringUri->getString()))
		, ref(*utf8Stream)
		, utf32Stream(new istream_utf8_utf32(ref)) {}

	public: StringUriUtf8To32ReadResource(StringUriUtf8To32ReadResource && rhs) noexcept
		: stringUri(std::move(rhs.stringUri))
		, utf8Stream(std::move(rhs.utf8Stream))
		, ref(rhs.ref)
		, utf32Stream(std::move(rhs.utf32Stream)) {}

	public: std::u32istream & readStream() override {
		return *utf32Stream;
	}

	public: const Uri & uri() const override {
		return *stringUri;
	}

	public: void close() override {}

	////////////////////////// Private implementation /////////////////////////

	private: std::unique_ptr<StringUri> stringUri;
	private: std::unique_ptr<std::istringstream> utf8Stream;
	private: std::istream & ref;
	private: std::unique_ptr<istream_utf8_utf32> utf32Stream;
};

///
/// A write-only UTF-8 resource in a standard file on a file system, which is written with UTF-32 characters.
///
class StringUriUtf32To8WriteResource : public Utf32To8WriteResource {
	private: using odevice_utf32_utf8 = boost::iostreams::code_converter<std::ostream, std::codecvt_utf8<char32_t, 0x10ffff, std::consume_header>>;
	private: using ostream_utf32_utf8 = boost::iostreams::stream<odevice_utf32_utf8>;

	///
	/// Create a new file UTF-32 to UTF-8 write resource from the supplied file URI.
	///
	/// @throw NotFoundException if the file does not exist
	///
	public: explicit StringUriUtf32To8WriteResource(StringUri & stringUri_)
		: stringUri(stringUri_)
		, utf8Stream(new std::ostringstream())
		, ref(*utf8Stream)
		, utf32Stream(new ostream_utf32_utf8(ref)) {}

	public: StringUriUtf32To8WriteResource(StringUriUtf32To8WriteResource && rhs) noexcept
		: stringUri(rhs.stringUri)
		, utf8Stream(std::move(rhs.utf8Stream))
		, ref(rhs.ref)
		, utf32Stream(std::move(rhs.utf32Stream)) {}

	public: ~StringUriUtf32To8WriteResource() override {
		close();
	}

	public: std::u32ostream & writeStream() override {
		return *utf32Stream;
	}

	public: const Uri & uri() const override {
		return stringUri;
	}

	public: void close() override {
		// TODO Determine why the temporary padding is required.. is this a defect?
		*utf32Stream << U"XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
		utf32Stream->flush();
		auto s = utf8Stream->str();
		std::string_view sv = s;
		std::string_view sv2 = sv.substr(0, sv.length() - 100);
		stringUri.append(sv2);
	}

	////////////////////////// Private implementation /////////////////////////

	private: StringUri & stringUri;
	private: std::unique_ptr<std::ostringstream> utf8Stream;
	private: std::ostream & ref;
	private: std::unique_ptr<ostream_utf32_utf8> utf32Stream;
};

inline StringUriByteReadResource StringUri::getByteReadResource() const {
	return StringUriByteReadResource(*this);
}

inline StringUriUtf8To32ReadResource StringUri::getUtf8To32ReadResource() const {
	return StringUriUtf8To32ReadResource(*this);
}

inline std::unique_ptr<ByteReadResource> StringUri::byteReadResource() const {
	return std::unique_ptr<ByteReadResource>(new StringUriByteReadResource(*this));
}

inline std::unique_ptr<Utf8To32ReadResource> StringUri::utf8To32ReadResource() const {
	return std::unique_ptr<Utf8To32ReadResource>(new StringUriUtf8To32ReadResource(*this));
}

inline std::unique_ptr<ByteWriteResource> StringUri::byteWriteResource() {
	return std::unique_ptr<ByteWriteResource>(new StringUriByteWriteResource(*this));
}

inline std::unique_ptr<Utf32To8WriteResource> StringUri::utf32To8WriteResource() {
	return std::unique_ptr<Utf32To8WriteResource>(new StringUriUtf32To8WriteResource(*this));
}

} // namespace Balau::Resource

#endif // COM_BORA_SOFTWARE__BALAU_RESOURCE__STRING_URI
