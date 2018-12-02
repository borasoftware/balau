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
/// @file HttpsUtf8To32ReadResource.hpp
///
/// A read-only HTTPS UTF-8 resource which is read as UTF-32 characters.
///

#ifndef COM_BORA_SOFTWARE__BALAU_RESOURCE__HTTPS_UTF8_TO_32_READ_RESOURCE
#define COM_BORA_SOFTWARE__BALAU_RESOURCE__HTTPS_UTF8_TO_32_READ_RESOURCE

#include <Balau/Resource/Utf8To32ReadResource.hpp>
#include <Balau/Resource/Impl/HttpsSource.hpp>

#include <boost/iostreams/code_converter.hpp>
#include <boost/iostreams/stream.hpp>

#include <codecvt>

namespace Balau::Resource {

class Https;

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
	public: explicit HttpsUtf8To32ReadResource(const Https & http_);

	public: ~HttpsUtf8To32ReadResource() {
		close();
	}

	public: std::u32istream & readStream() override {
		return utf32Stream;
	}

	public: const Uri & uri() const override;

	public: void close() override {
		utf32Stream.close();
	}

	////////////////////////// Private implementation /////////////////////////

	private: std::unique_ptr<Https> url;
	private: boost::iostreams::stream<Impl::HttpsSource> utf8Stream;
	private: std::istream & ref;
	private: istream_utf8_utf32 utf32Stream;
};

} // namespace Balau::Resource

#endif // COM_BORA_SOFTWARE__BALAU_RESOURCE__HTTPS_UTF8_TO_32_READ_RESOURCE
