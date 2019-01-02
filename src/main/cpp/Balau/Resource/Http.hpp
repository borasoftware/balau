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
/// @file Http.hpp
///
/// An HTTP URL.
///

#ifndef COM_BORA_SOFTWARE__BALAU_RESOURCE__HTTP
#define COM_BORA_SOFTWARE__BALAU_RESOURCE__HTTP

#include <Balau/Exception/BalauException.hpp>
#include <Balau/Resource/HttpByteReadResource.hpp>
#include <Balau/Resource/HttpUtf8To32ReadResource.hpp>
#include <Balau/Resource/Url.hpp>
#include <Balau/Resource/UriComponents.hpp>

namespace Balau::Resource {

///
/// An HTTP URL.
///
class Http : public Url {
	///
	/// Create a null HTTP URI.
	///
	public: Http() = default;

	///
	/// Create an HTTP URI from the supplied URI string.
	///
	public: explicit Http(std::string && uri_) : Url(std::move(uri_)) {}

	///
	/// Create an HTTP URI from the supplied URI string.
	///
	public: explicit Http(std::string_view uri_) : Url(uri_) {}

	///
	/// Create an HTTP URI from the supplied URI string.
	///
	public: explicit Http(const char * uri_) : Url(uri_) {}

	///
	/// Create an HTTP URI by copying the supplied instance.
	///
	public: Http(const Http & copy) = default;

	///
	/// Create an HTTP URI by moving the contents of the supplied instance.
	///
	public: Http(Http && rhs) noexcept : Url(std::move(rhs)) {}

	public: std::unique_ptr<Uri> clone() const override {
		return std::unique_ptr<Uri>(new Http(*this));
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

		const UriComponents components(uri);

		if (Util::Strings::startsWith(str, "/")) {
			// Absolute path.
			std::string url = ::toString(components.scheme(), "://", components.host(), str);
			return std::unique_ptr<Uri>(new Http(url));
		} else {
			// Relative path.
			// TODO normalise path.
			std::string url = ::toString(components.scheme(), "://", components.host(), "/", components.path(), "/", str);
			return std::unique_ptr<Uri>(new Http(url));
		}
	}

	public: bool operator == (const Uri & rhs) const override {
		const auto * o = dynamic_cast<const Http *>(&rhs);
		return o != nullptr && uri == o->uri;
	}

	public: bool canReadFrom() const override {
		return true;
	}

	public: bool canWriteTo() const override {
		return false;
	}

	public: template <typename AllocatorT> Balau::U8String<AllocatorT> toRawString() const {
		return ::toString<AllocatorT>(uri);
	}

	///
	/// Get a byte read resource for the HTTP source.
	///
	public: HttpByteReadResource getByteReadResource() const {
		return HttpByteReadResource(*this);
	}

	///
	/// Get a UTF-8 to UTF-32 read resource for the HTTP source.
	///
	public: HttpUtf8To32ReadResource getUtf8To32ReadResource() const {
		return HttpUtf8To32ReadResource(*this);
	}

	public: std::unique_ptr<ByteReadResource> byteReadResource() const override {
		return std::unique_ptr<ByteReadResource>(new HttpByteReadResource(*this));
	}

	public: std::unique_ptr<Utf8To32ReadResource> utf8To32ReadResource() const override {
		return std::unique_ptr<Utf8To32ReadResource>(new HttpUtf8To32ReadResource(*this));
	}

	public: std::unique_ptr<ByteWriteResource> byteWriteResource() override {
		ThrowBalauException(Exception::NotImplementedException, "HTTP URIs do not have a byte write resource.");
	}

	public: std::unique_ptr<Utf32To8WriteResource> utf32To8WriteResource() override {
		ThrowBalauException(Exception::NotImplementedException, "HTTP URIs do not have a Unicode write resource.");
	}

	public: bool isRecursivelyIterable() const override {
		return false;
	}

	public: std::unique_ptr<RecursiveUriIterator> recursiveIterator() const override {
		ThrowBalauException(Exception::NotImplementedException, "HTTP URIs do not have a recursive iterator");
	}

	public: void dispatch(UriDispatcher & dispatcher) const override {
		dispatcher.dispatch(*this);
	}
};

///
/// Print the HTTP URL as a UTF-8 string.
///
/// @return a UTF-8 string containing the HTTP URL
///
template <typename AllocatorT>
inline Balau::U8String<AllocatorT> toString(const Http & http) {
	return http.toRawString<AllocatorT>();
}

} // namespace Balau::Resource

#endif // COM_BORA_SOFTWARE__BALAU_RESOURCE__HTTP
