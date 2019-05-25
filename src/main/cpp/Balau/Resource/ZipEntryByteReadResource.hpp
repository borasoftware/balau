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
/// @file ZipEntryByteReadResource.hpp
///
/// A read-only entry in a zip file which is read as bytes.
///

#ifndef COM_BORA_SOFTWARE__BALAU_RESOURCE__ZIP_ENTRY_BYTE_READ_RESOURCE
#define COM_BORA_SOFTWARE__BALAU_RESOURCE__ZIP_ENTRY_BYTE_READ_RESOURCE

#ifdef BALAU_LIBZIP_ENABLED

#include <Balau/Resource/ByteReadResource.hpp>
#include <Balau/Resource/Impl/ZipEntrySource.hpp>

#include <boost/iostreams/stream.hpp>

namespace Balau::Resource {

class ZipEntry;

///
/// A read-only entry in a zip file which is read as bytes.
///
/// Zip entry byte read resources are created by calling ZipEntry::getByteReadResource.
///
class ZipEntryByteReadResource : public ByteReadResource {
	public: ZipEntryByteReadResource(ZipEntryByteReadResource && rhs) noexcept
		: entry(rhs.entry)
		, stream(std::move(rhs.stream)) {}

	public: ~ZipEntryByteReadResource() override = default;

	public: void close() override {
		// NOP
	}

	public: const Uri & uri() const override;

	public: std::istream & readStream() override {
		return *stream;
	}

	////////////////////////// Private implementation /////////////////////////

	friend class ZipEntry;

	private: explicit ZipEntryByteReadResource(const ZipEntry & entry_);

	private: const ZipEntry & entry;
	private: std::unique_ptr<boost::iostreams::stream<Impl::ZipEntrySource>> stream;
};

} // namespace Balau::Resource

#endif // BALAU_LIBZIP_ENABLED

#endif // COM_BORA_SOFTWARE__BALAU_RESOURCE__ZIP_ENTRY_BYTE_READ_RESOURCE
