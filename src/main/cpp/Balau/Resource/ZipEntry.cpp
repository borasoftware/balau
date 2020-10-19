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
#include "ZipFile.hpp"

namespace Balau::Resource {

std::string ZipEntry::toUriString() const {
	Util::ZipEntryInfo info {};
	archive.getEntryInfo(entryIndex, info);
	return archive.getPath().toUriString() + "#" + ::toString(info.name);
}

size_t ZipEntry::hashcode() const noexcept {
	Util::ZipEntryInfo info {};
	archive.getEntryInfo(entryIndex, info);
	return archive.getPath().hashcode() ^ std::hash<std::string>()(::toString(info.name));
}

bool ZipEntry::isEntryDirectory() const {
	Util::ZipEntryInfo info {};
	archive.getEntryInfo(entryIndex, info);
	return Util::Strings::endsWith(info.name, "/");
}

bool ZipEntry::isEntryFile() const {
	return !isEntryDirectory();
}

size_t ZipEntry::size() const {
	Util::ZipEntryInfo info {};
	archive.getEntryInfo(entryIndex, info);
	return info.uncompressedSize;
}

bool ZipEntry::operator == (const Uri & rhs) const {
	const auto * o = dynamic_cast<const ZipEntry *>(&rhs);
	return o == nullptr ? false : (archive.getPath() == o->archive.getPath() && entryIndex == o->entryIndex);
}

} // namespace Balau::Resource
