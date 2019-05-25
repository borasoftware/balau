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

#endif // BALAU_LIBZIP_ENABLED
