// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#ifdef BALAU_LIBZIP_ENABLED

#include "ZipEntry.hpp"
#include "ZipEntryByteReadResource.hpp"

namespace Balau::Resource {

ZipEntryByteReadResource::ZipEntryByteReadResource(const ZipEntry & entry_)
	: entry(entry_)
	, stream(new boost::iostreams::stream<Impl::ZipEntrySource>(Impl::ZipEntrySource(entry.archive, entry.entryIndex))) {}

const Uri & ZipEntryByteReadResource::uri() const {
	return entry;
}

} // namespace Balau::Resource

#endif // BALAU_LIBZIP_ENABLED
