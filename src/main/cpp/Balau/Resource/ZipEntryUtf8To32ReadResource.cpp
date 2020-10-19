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
#include "ZipEntry.hpp"
#include "ZipEntryByteReadResource.hpp"

namespace Balau::Resource {

ZipEntryUtf8To32ReadResource::ZipEntryUtf8To32ReadResource(const ZipEntry & entry_)
	: entry(entry_)
	, utf8Stream(new boost::iostreams::stream<Impl::ZipEntrySource>(entry.archive, entry.entryIndex))
	, ref(*utf8Stream)
	, utf32Stream(new istream_utf8_utf32(ref)) {}

const Uri & ZipEntryUtf8To32ReadResource::uri() const {
	return entry;
}

} // namespace Balau::Resource
