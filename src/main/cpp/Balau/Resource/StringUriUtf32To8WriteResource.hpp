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
/// @file StringUriUtf32To8WriteResource.hpp
///
/// A write-only String based UTF-8 resource which is written as UTF-32 characters.
///

#ifndef COM_BORA_SOFTWARE__BALAU_RESOURCE__STRING_URI_UTF32_TO_8_WRITE_RESOURCE
#define COM_BORA_SOFTWARE__BALAU_RESOURCE__STRING_URI_UTF32_TO_8_WRITE_RESOURCE

#include <Balau/Resource/Utf32To8WriteResource.hpp>

#include <boost/iostreams/code_converter.hpp>
#include <boost/iostreams/stream.hpp>

#include <codecvt>

namespace Balau::Resource {

class StringUri;
class Uri;

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
	public: explicit StringUriUtf32To8WriteResource(StringUri & stringUri_);

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

	public: const Uri & uri() const override;

	public: void close() override;

	////////////////////////// Private implementation /////////////////////////

	private: StringUri & stringUri;
	private: std::unique_ptr<std::ostringstream> utf8Stream;
	private: std::ostream & ref;
	private: std::unique_ptr<ostream_utf32_utf8> utf32Stream;
};

} // namespace Balau::Resource

#endif // COM_BORA_SOFTWARE__BALAU_RESOURCE__STRING_URI_UTF32_TO_8_WRITE_RESOURCE
