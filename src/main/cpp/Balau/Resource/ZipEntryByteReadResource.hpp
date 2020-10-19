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
/// @file ZipEntryByteReadResource.hpp
///
/// A read-only entry in a zip file which is read as bytes.
///

#ifndef COM_BORA_SOFTWARE__BALAU_RESOURCE__ZIP_ENTRY_BYTE_READ_RESOURCE
#define COM_BORA_SOFTWARE__BALAU_RESOURCE__ZIP_ENTRY_BYTE_READ_RESOURCE

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

#endif // COM_BORA_SOFTWARE__BALAU_RESOURCE__ZIP_ENTRY_BYTE_READ_RESOURCE
