// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include "ZipEntrySource.hpp"
#include "../../Exception/ResourceExceptions.hpp"

#include <zip.h>

namespace Balau::Resource::Impl {

inline zip_file_t * cast(std::shared_ptr<void> & zipFileHandle) {
	return (zip_file_t *) zipFileHandle.get();
}

ZipEntrySource::ZipEntrySource(Util::Unzipper & archive_, unsigned long long index_)
	: archive(archive_)
	, index(index_)
	, zipFile(createZipFilePointer(archive)) {}

std::streamsize ZipEntrySource::read(char * s, std::streamsize n) {
	if (!archive.isOpen()) {
		ThrowBalauException(Exception::ZipException, "Zip archive is not open.", archive.path);
	}

	long long result = zip_fread(cast(zipFile), s, (unsigned long long) n);

	// TODO check if 0 or -1 is the normal EOF indicator.
	if (result >= 0) {
		return result;
	} else {
		ThrowBalauException(Exception::ZipException, "Unexpected error whilst reading zip file.", archive.path);
	}
}

std::shared_ptr<void> ZipEntrySource::createZipFilePointer(Util::Unzipper & archive) {
	zip_file_t * file = zip_fopen_index((zip_t *) archive.archive, index, 0);

	if (file == nullptr) {
		ThrowBalauException(Exception::ZipException, "Could not open zip file.", archive.path);
	}

	return std::shared_ptr<void>(
		  file
		, [] (auto ptr) {
			if (ptr != nullptr) {
				zip_fclose((zip_file_t *) ptr);
			}
		}
	);
}

///////////////////////////////////////////////////////////////////////////////

//ZipEntrySink::ZipEntrySink(Util::Zipper & archive_, unsigned long long index_)
//	: archive(archive_)
//	, index(index_)
//	, zipFile(zip_fopen_index((zip_t *) archive.archive, index, 0)) {
//
//	if (zipFile == nullptr) {
//		ThrowBalauException(Exception::ZipException, "Could not open zip file.", archive.path);
//	}
//}
//
//ZipEntrySink::~ZipEntrySink() {
//	if (zipFile != nullptr) {
//		zip_fclose(cast(zipFile));
//	}
//}
//
//std::streamsize ZipEntrySink::write(char * s, std::streamsize n) {
//	// TODO
//}

} // namespace Balau::Resource::Impl
