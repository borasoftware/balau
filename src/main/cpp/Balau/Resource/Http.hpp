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
/// @file Http.hpp
///
/// An HTTP URL.
///

#ifndef COM_BORA_SOFTWARE__BALAU_RESOURCE__HTTP
#define COM_BORA_SOFTWARE__BALAU_RESOURCE__HTTP

#include <Balau/Resource/ByteReadResource.hpp>
#include <Balau/Resource/UriComponents.hpp>
#include <Balau/Resource/Url.hpp>
#include <Balau/Resource/Utf8To32ReadResource.hpp>
#include <Balau/ThirdParty/CppHttpLib/HttpLib.hpp>

#include <boost/iostreams/code_converter.hpp>
#include <boost/iostreams/concepts.hpp>
#include <boost/iostreams/stream.hpp>

#include <codecvt>

namespace Balau::Resource {

class HttpByteReadResource;
class HttpsByteReadResource;
class HttpsUtf8To32ReadResource;
class HttpUtf8To32ReadResource;

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

	public: std::unique_ptr<Uri> append(const std::string & pathComponent) const override {
		return std::unique_ptr<Uri>(new Http(appendPathComponent(pathComponent)));
	}

	public: void visit(UriVisitor & visitor) const override {
		visitor.visit(*this);
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
	public: HttpByteReadResource getByteReadResource() const;

	///
	/// Get a UTF-8 to UTF-32 read resource for the HTTP source.
	///
	public: HttpUtf8To32ReadResource getUtf8To32ReadResource() const;

	public: std::unique_ptr<ByteReadResource> byteReadResource() const override;

	public: std::unique_ptr<Utf8To32ReadResource> utf8To32ReadResource() const override;

	public: std::unique_ptr<ByteWriteResource> byteWriteResource() override {
		ThrowBalauException(Exception::NotImplementedException, "HTTP URIs do not have a byte write resource.");
	}

	public: std::unique_ptr<Utf32To8WriteResource> utf32To8WriteResource() override {
		ThrowBalauException(Exception::NotImplementedException, "HTTP URIs do not have a Unicode write resource.");
	}

	public: bool isRecursivelyIterable() const override {
		return false;
	}

	public: bool isIterable() const override {
		return false;
	}

	public: std::unique_ptr<RecursiveUriIterator> recursiveIterator() const override {
		ThrowBalauException(Exception::NotImplementedException, "HTTP URIs do not have recursive iterators.");
	}

	public: std::unique_ptr<UriIterator> iterator() const override {
		ThrowBalauException(Exception::NotImplementedException, "HTTP URIs do not have iterators.");
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

///
/// An HTTPS URL.
///
class Https : public Url {
	///
	/// Create a null HTTPS URI.
	///
	public: Https() = default;

	///
	/// Create an HTTPS URI from the supplied URI string.
	///
	public: explicit Https(std::string uri_) : Url(std::move(uri_)) {}

	///
	/// Create an HTTPS URI from the supplied URI string.
	///
	public: explicit Https(std::string_view uri_) : Url(uri_) {}

	///
	/// Create an HTTPS URI from the supplied URI string.
	///
	public: explicit Https(const char * uri_) : Url(uri_) {}

	///
	/// Create an HTTPS URI by copying the supplied instance.
	///
	public: Https(const Https & copy) = default;

	///
	/// Create an HTTPS URI by moving the contents of the supplied instance.
	///
	public: Https(Https && rhs) noexcept : Url(std::move(rhs)) {}

	public: std::unique_ptr<Uri> clone() const override {
		return std::unique_ptr<Uri>(new Https(*this));
	}

	public: std::unique_ptr<Uri> append(const std::string & pathComponent) const override {
		return std::unique_ptr<Uri>(new Https(appendPathComponent(pathComponent)));
	}

	public: void visit(UriVisitor & visitor) const override {
		visitor.visit(*this);
	}

	public: bool operator == (const Uri & rhs) const override {
		const auto * o = dynamic_cast<const Https *>(&rhs);
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
	/// Get a byte read resource for the HTTPS source.
	///
	public: HttpsByteReadResource getByteReadResource();

	///
	/// Get a UTF-8 to UTF-32 read resource for the HTTPS source.
	///
	public: HttpsUtf8To32ReadResource getUtf8To32ReadResource();

	public: std::unique_ptr<ByteReadResource> byteReadResource() const override;

	public: std::unique_ptr<Utf8To32ReadResource> utf8To32ReadResource() const override;

	public: std::unique_ptr<ByteWriteResource> byteWriteResource() override {
		ThrowBalauException(Exception::NotImplementedException, "HTTPS URIs do not have a byte write resource.");
	}

	public: std::unique_ptr<Utf32To8WriteResource> utf32To8WriteResource() override {
		ThrowBalauException(Exception::NotImplementedException, "HTTPS URIs do not have a Unicode write resource.");
	}

	public: bool isRecursivelyIterable() const override {
		return false;
	}

	public: bool isIterable() const override {
		return false;
	}

	public: std::unique_ptr<RecursiveUriIterator> recursiveIterator() const override {
		ThrowBalauException(Exception::NotImplementedException, "HTTPS URIs do not have recursive iterator.");
	}

	public: std::unique_ptr<UriIterator> iterator() const override {
		ThrowBalauException(Exception::NotImplementedException, "HTTPS URIs do not have iterators.");
	}

	public: void dispatch(UriDispatcher & dispatcher) const override {
		dispatcher.dispatch(*this);
	}
};

///
/// Print the HTTPs URL as a UTF-8 string.
///
/// @return a UTF-8 string containing the HTTPs URL
///
template <typename AllocatorT>
inline Balau::U8String<AllocatorT> toString(const Https & https) {
	return https.toRawString<AllocatorT>();
}

namespace Impl {

///
/// Boost IO streams HTTP source, used in the HTTP input stream.
///
class HttpSource : public boost::iostreams::source {
	public: HttpSource(const Http & url_)
		: http(std::make_unique<Http>(url_))
		, url(url_)
		, client(createClient())
		, responseBody(getBody())
		, currentPosition(0) {}

	public: HttpSource(const HttpSource & copy)
		: http(copy.http)
		, url(copy.url)
		, client(copy.client->clone())
		, responseBody(copy.responseBody)
		, currentPosition(copy.currentPosition) {}

	public: ~HttpSource() = default;

	public: std::streamsize read(char * s, std::streamsize n) {
		if (currentPosition >= (std::streamsize) responseBody.size()) {
			return 0;
		}

		const auto bytesAvailable = (std::streamsize) responseBody.size() - currentPosition;
		const auto bytesToProvide = bytesAvailable < n ? bytesAvailable : n;

		std::memcpy(s, responseBody.data() + currentPosition, (size_t) bytesToProvide);
		currentPosition += bytesToProvide;
		return bytesToProvide;
	}

	public: const Http & getUrl() const {
		return *http;
	}

	///////////////////////// Private implementation //////////////////////////

	private: std::unique_ptr<HttpLib::Client> createClient() {
		if (!url.hasHost()) {
			ThrowBalauException(
				Exception::InvalidUriException, "A URL with host information is required: " + toString(url)
			);
		}

		if (!url.hasPort()) {
			// Default to port 80.
			return std::make_unique<HttpLib::Client>(std::string(url.host()), 80);
		} else {
			return std::make_unique<HttpLib::Client>(std::string(url.host()), url.port());
		}
	}

	private: std::vector<char> getBody() {
		auto p = url.pathQueryFragment();
		auto response = client->Get(std::string(p).c_str());
		return Util::Vectors::toCharVector(response->body);
	}

	private: std::shared_ptr<Http> http;
	private: UriComponents url;
	private: std::unique_ptr<HttpLib::Client> client;
	private: std::vector<char> responseBody;
	private: std::streamsize currentPosition;
};

///
/// Boost IO streams HTTPS source, used in the HTTPS input stream.
///
class HttpsSource : public boost::iostreams::source {
	public: HttpsSource(const Https & url_)
		: https(std::make_unique<Https>(url_))
		, url(url_)
		, client(createClient())
		, responseBody(getBody())
		, currentPosition(0) {}

	public: HttpsSource(const HttpsSource & copy)
		: https(copy.https)
		, url(copy.url)
		, client(copy.client->clone())
		, responseBody(copy.responseBody)
		, currentPosition(copy.currentPosition) {}

	public: ~HttpsSource() = default;

	public: std::streamsize read(char * s, std::streamsize n) {
		if (currentPosition >= (std::streamsize) responseBody.size()) {
			return 0;
		}

		const auto bytesAvailable = (std::streamsize) responseBody.size() - currentPosition;
		const auto bytesToProvide = bytesAvailable < n ? bytesAvailable : n;

		std::memcpy(s, responseBody.data() + currentPosition, (size_t) bytesToProvide);
		currentPosition += bytesToProvide;
		return bytesToProvide;
	}

	public: const Https & getUrl() const {
		return *https;
	}

	///////////////////////// Private implementation //////////////////////////

	private: std::unique_ptr<HttpLib::Client> createClient() {
		if (!url.hasHost()) {
			ThrowBalauException(
				Exception::InvalidUriException, "A URL with host information is required: " + toString(url)
			);
		}

		const std::string clientCertPath; // TODO
		const std::string clientKeyPath; // TODO

		if (!url.hasPort()) {
			// Default to port 443.
			return std::make_unique<HttpLib::Client>(std::string(url.host()), 443, true, clientCertPath, clientKeyPath);
		} else {
			return std::make_unique<HttpLib::Client>(std::string(url.host()), url.port(), true, clientCertPath, clientKeyPath);
		}
	}

	private: std::vector<char> getBody() {
		auto response = client->Get(std::string(url.pathQueryFragment()).c_str());
		return Util::Vectors::toCharVector(response->body);
	}

	private: std::shared_ptr<Https> https;
	private: UriComponents url;
	private: std::unique_ptr<HttpLib::Client> client;
	private: std::vector<char> responseBody;
	private: std::streamsize currentPosition;
};

} // namespace Impl

///
/// A read-only HTTP resource which is read as bytes.
///
class HttpByteReadResource : public ByteReadResource {
	///
	/// Create a new HTTP byte read resource from the supplied HTTP URI.
	///
	/// @throw InvalidUriException if no host information was provided
	///
	public: explicit HttpByteReadResource(const Http & url_)
		: url(new Http(url_))
		, stream(new boost::iostreams::stream<Impl::HttpSource>(Impl::HttpSource(url_))) {}

	public: HttpByteReadResource(HttpByteReadResource && rhs) noexcept
		: url(std::move(rhs.url))
		, stream(std::move(rhs.stream)) {}

	public: ~HttpByteReadResource() override {
		close();
	}

	public: std::istream & readStream() override {
		return *stream;
	}

	public: const Uri & uri() const override {
		return *url;
	}

	public: void close() override {
		stream->close();
	}

	////////////////////////// Private implementation /////////////////////////

	private: std::unique_ptr<Http> url;
	private: std::unique_ptr<boost::iostreams::stream<Impl::HttpSource>> stream;
};

///
/// A read-only HTTP resource which is read as bytes.
///
class HttpsByteReadResource : public ByteReadResource {
	///
	/// Create a new HTTPS byte read resource from the supplied HTTPS URI.
	///
	/// @throw InvalidUriException if no host information was provided
	///
	public: explicit HttpsByteReadResource(const Https & url_)
		: url(new Https(url_))
		, stream(new boost::iostreams::stream<Impl::HttpsSource>(Impl::HttpsSource(url_))) {}

	public: HttpsByteReadResource(HttpsByteReadResource && rhs) noexcept
		: url(std::move(rhs.url))
		, stream(std::move(rhs.stream)) {}

	public: ~HttpsByteReadResource() override {
		close();
	}

	public: std::istream & readStream() override {
		return *stream;
	}

	public: const Uri & uri() const override {
		return *url;
	}

	public: void close() override {
		stream->close();
	}

	////////////////////////// Private implementation /////////////////////////

	private: std::unique_ptr<Https> url;
	private: std::unique_ptr<boost::iostreams::stream<Impl::HttpsSource>> stream;
};

///
/// A read-only HTTP UTF-8 resource which is read as UTF-32 characters.
///
class HttpsUtf8To32ReadResource : public Utf8To32ReadResource {
	private: using idevice_utf8_utf32 = boost::iostreams::code_converter<std::istream, std::codecvt_utf8<char32_t, 0x10ffff, std::consume_header>>;
	private: using istream_utf8_utf32 = boost::iostreams::stream<idevice_utf8_utf32>;

	///
	/// Create a new HTTPS UTF-8 to UTF-32 read resource from the supplied HTTPS URI.
	///
	/// @throw InvalidUriException if no host information was provided
	///
	public: explicit HttpsUtf8To32ReadResource(const Https & http_)
		: url(new Https(http_))
		, utf8Stream(new boost::iostreams::stream<Impl::HttpsSource>(http_))
		, ref(*utf8Stream)
		, utf32Stream(new istream_utf8_utf32(ref)) {}

	public: HttpsUtf8To32ReadResource(HttpsUtf8To32ReadResource && rhs) noexcept
		: url(std::move(rhs.url))
		, utf8Stream(std::move(rhs.utf8Stream))
		, ref(rhs.ref)
		, utf32Stream(std::move(rhs.utf32Stream)) {}

	public: ~HttpsUtf8To32ReadResource() override {
		close();
	}

	public: std::u32istream & readStream() override {
		return *utf32Stream;
	}

	public: const Uri & uri() const override {
		return *url;
	}

	public: void close() override {
		utf32Stream->close();
	}

	////////////////////////// Private implementation /////////////////////////

	private: std::unique_ptr<Https> url;
	private: std::unique_ptr<boost::iostreams::stream<Impl::HttpsSource>> utf8Stream;
	private: std::istream & ref;
	private: std::unique_ptr<istream_utf8_utf32> utf32Stream;
};

///
/// A read-only HTTP UTF-8 resource which is read as UTF-32 characters.
///
class HttpUtf8To32ReadResource : public Utf8To32ReadResource {
	private: using idevice_utf8_utf32 = boost::iostreams::code_converter<std::istream, std::codecvt_utf8<char32_t, 0x10ffff, std::consume_header>>;
	private: using istream_utf8_utf32 = boost::iostreams::stream<idevice_utf8_utf32>;

	///
	/// Create a new HTTP UTF-8 to UTF-32 read resource from the supplied HTTP URI.
	///
	/// @throw InvalidUriException if no host information was provided
	///
	public: explicit HttpUtf8To32ReadResource(const Http & url_)
		: url(new Http(url_))
		, utf8Stream(new boost::iostreams::stream<Impl::HttpSource>(url_))
		, ref(*utf8Stream)
		, utf32Stream(new istream_utf8_utf32(ref)) {}

	public: HttpUtf8To32ReadResource(HttpUtf8To32ReadResource && rhs) noexcept
		: url(std::move(rhs.url))
		, utf8Stream(std::move(rhs.utf8Stream))
		, ref(rhs.ref)
		, utf32Stream(std::move(rhs.utf32Stream)) {}

	public: ~HttpUtf8To32ReadResource() override {
		close();
	}

	public: std::u32istream & readStream() override {
		return *utf32Stream;
	}

	public: const Uri & uri() const override {
		return *url;
	}

	public: void close() override {
		utf32Stream->close();
	}

	////////////////////////// Private implementation /////////////////////////

	private: std::unique_ptr<Http> url;
	private: std::unique_ptr<boost::iostreams::stream<Impl::HttpSource>> utf8Stream;
	private: std::istream & ref;
	private: std::unique_ptr<istream_utf8_utf32> utf32Stream;
};

inline HttpByteReadResource Http::getByteReadResource() const {
	return HttpByteReadResource(*this);
}

inline HttpUtf8To32ReadResource Http::getUtf8To32ReadResource() const {
	return HttpUtf8To32ReadResource(*this);
}

inline std::unique_ptr<ByteReadResource> Http::byteReadResource() const {
	return std::unique_ptr<ByteReadResource>(new HttpByteReadResource(*this));
}

inline std::unique_ptr<Utf8To32ReadResource> Http::utf8To32ReadResource() const {
	return std::unique_ptr<Utf8To32ReadResource>(new HttpUtf8To32ReadResource(*this));
}

inline HttpsByteReadResource Https::getByteReadResource() {
	return HttpsByteReadResource(*this);
}

inline HttpsUtf8To32ReadResource Https::getUtf8To32ReadResource() {
	return HttpsUtf8To32ReadResource(*this);
}

inline std::unique_ptr<ByteReadResource> Https::byteReadResource() const {
	return std::unique_ptr<ByteReadResource>(new HttpsByteReadResource(*this));
}

inline std::unique_ptr<Utf8To32ReadResource> Https::utf8To32ReadResource() const {
	return std::unique_ptr<Utf8To32ReadResource>(new HttpsUtf8To32ReadResource(*this));
}

} // namespace Balau::Resource

#endif // COM_BORA_SOFTWARE__BALAU_RESOURCE__HTTP
