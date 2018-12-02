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
/// @file StringUriUtf8To32ReadResource.hpp
///
/// A read-only String UTF-8 resource which is read as UTF-32 characters.
///

#ifndef COM_BORA_SOFTWARE__BALAU_RESOURCE__STRING_URI_UTF8_TO_32_READ_RESOURCE
#define COM_BORA_SOFTWARE__BALAU_RESOURCE__STRING_URI_UTF8_TO_32_READ_RESOURCE

#include <Balau/Resource/Utf8To32ReadResource.hpp>

#include <boost/iostreams/code_converter.hpp>
#include <boost/iostreams/stream.hpp>

#include <codecvt>

namespace Balau::Resource {

class StringUri;
class Uri;

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
	public: explicit StringUriUtf8To32ReadResource(const StringUri & stringUri_);

	public: std::u32istream & readStream() override {
		return utf32Stream;
	}

	public: const Uri & uri() const override;

	public: void close() override {}

	////////////////////////// Private implementation /////////////////////////

	private: std::unique_ptr<StringUri> stringUri;
	private: std::istringstream utf8Stream;
	private: std::istream & ref;
	private: istream_utf8_utf32 utf32Stream;
};

} // namespace Balau::Resource

#endif // COM_BORA_SOFTWARE__BALAU_RESOURCE__STRING_URI_UTF8_TO_32_READ_RESOURCE
