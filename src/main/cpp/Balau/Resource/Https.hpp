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
/// @file Https.hpp
///
/// An HTTPS URL.
///

#ifndef COM_BORA_SOFTWARE__BALAU_RESOURCE__HTTPS
#define COM_BORA_SOFTWARE__BALAU_RESOURCE__HTTPS

#include <Balau/Exception/BalauException.hpp>
#include <Balau/Resource/Url.hpp>
#include <Balau/Resource/HttpsByteReadResource.hpp>
#include <Balau/Resource/HttpsUtf8To32ReadResource.hpp>

namespace Balau::Resource {

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
			return std::unique_ptr<Uri>(new Https(url));
		} else {
			// Relative path.
			// TODO normalise path.
			std::string url = ::toString(components.scheme(), "://", components.host(), "/", components.path(), "/", str);
			return std::unique_ptr<Uri>(new Https(url));
		}
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
	public: HttpsByteReadResource getByteReadResource() {
		return HttpsByteReadResource(*this);
	}

	///
	/// Get a UTF-8 to UTF-32 read resource for the HTTPS source.
	///
	public: HttpsUtf8To32ReadResource getUtf8To32ReadResource() {
		return HttpsUtf8To32ReadResource(*this);
	}

	public: std::unique_ptr<ByteReadResource> byteReadResource() const override {
		return std::unique_ptr<ByteReadResource>(new HttpsByteReadResource(*this));
	}

	public: std::unique_ptr<Utf8To32ReadResource> utf8To32ReadResource() const override {
		return std::unique_ptr<Utf8To32ReadResource>(new HttpsUtf8To32ReadResource(*this));
	}

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

} // namespace Balau::Resource

#endif // COM_BORA_SOFTWARE__BALAU_RESOURCE__HTTPS
