// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include "ZipEntry.hpp"
#include "ZipEntryByteReadResource.hpp"

namespace Balau::Resource {

ZipEntryByteReadResource::ZipEntryByteReadResource(ZipEntry & entry_)
	: entry(entry_)
	, stream(Impl::ZipEntrySource(entry.archive, entry.entryIndex)) {}

const Uri & ZipEntryByteReadResource::uri() const {
	return entry;
}

} // namespace Balau::Resource
