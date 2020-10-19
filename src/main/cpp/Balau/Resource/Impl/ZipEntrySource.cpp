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

#define _Nonnull
#define _Nullable
#include <zip.h>

#include "ZipEntrySource.hpp"

#include "../../Exception/ResourceExceptions.hpp"

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
