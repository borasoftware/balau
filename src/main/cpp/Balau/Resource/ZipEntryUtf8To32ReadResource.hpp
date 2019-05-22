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
/// @file ZipEntryUtf8To32ReadResource.hpp
///
/// A read-only UTF-8 entry in a zip file which is read as UTF-32 characters.
///

#ifndef COM_BORA_SOFTWARE__BALAU_RESOURCE__ZIP_ENTRY_UTF8_TO_32_READ_RESOURCE
#define COM_BORA_SOFTWARE__BALAU_RESOURCE__ZIP_ENTRY_UTF8_TO_32_READ_RESOURCE

#ifdef BALAU_LIBZIP_ENABLED

#include <Balau/Resource/Utf8To32ReadResource.hpp>
#include <Balau/Resource/ByteReadResource.hpp>
#include <Balau/Resource/Impl/ZipEntrySource.hpp>

#include <boost/iostreams/code_converter.hpp>
#include <boost/iostreams/stream.hpp>

#include <codecvt>

namespace Balau::Resource {

class ZipEntry;

///
/// A read-only entry in a zip file which is read as UTF-8 characters and implicitly converted to UTF-32 characters.
///
/// Zip entry UTF-8 to UTF-32 read resources are created by calling ZipEntry::ZipEntryUtf8To32ReadResource.
///
class ZipEntryUtf8To32ReadResource : public Utf8To32ReadResource {
	private: using idevice_utf8_utf32 = boost::iostreams::code_converter<std::istream, std::codecvt_utf8<char32_t, 0x10ffff, std::consume_header>>;
	private: using istream_utf8_utf32 = boost::iostreams::stream<idevice_utf8_utf32>;

	public: ~ZipEntryUtf8To32ReadResource() override = default;

	public: ZipEntryUtf8To32ReadResource(ZipEntryUtf8To32ReadResource && rhs) noexcept
		: entry(rhs.entry)
		, utf8Stream(std::move(rhs.utf8Stream))
		, ref(rhs.ref)
		, utf32Stream(std::move(rhs.utf32Stream)) {}

	public: void close() override {
		// NOP
	}

	public: const Uri & uri() const override;

	public: std::u32istream & readStream() override {
		return *utf32Stream;
	}

	///
	/// Create a zip entry UTF-8 to UTF-32 read resource for the supplied zip entry URI.
	///
	public: explicit ZipEntryUtf8To32ReadResource(const ZipEntry & entry_);

	////////////////////////// Private implementation /////////////////////////

	friend class ZipEntry;

	private: const ZipEntry & entry;
	private: std::unique_ptr<boost::iostreams::stream<Impl::ZipEntrySource>> utf8Stream;
	private: std::istream & ref;
	private: std::unique_ptr<istream_utf8_utf32> utf32Stream;
};

} // namespace Balau::Resource

#endif // BALAU_LIBZIP_ENABLED

#endif // COM_BORA_SOFTWARE__BALAU_RESOURCE__ZIP_ENTRY_UTF8_TO_32_READ_RESOURCE
