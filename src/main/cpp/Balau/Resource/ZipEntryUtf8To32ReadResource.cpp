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

ZipEntryUtf8To32ReadResource::ZipEntryUtf8To32ReadResource(const ZipEntry & entry_)
	: entry(entry_)
	, utf8Stream(entry.archive, entry.entryIndex)
	, ref(utf8Stream)
	, utf32Stream(ref) {}

const Uri & ZipEntryUtf8To32ReadResource::uri() const {
	return entry;
}

} // namespace Balau::Resource
