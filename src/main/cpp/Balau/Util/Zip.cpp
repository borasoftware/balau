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
#include "Zip.hpp"
#include "Balau/Type/OnScopeExit.hpp"

#define _Nonnull
#define _Nullable
#include <zip.h>

namespace Balau::Util {

std::string getLibZipErrorAsString(int error) {
	switch (error) {
		case ZIP_ER_OK:              return "No error";
		case ZIP_ER_MULTIDISK:       return "Multi-disk zip archives not supported";
		case ZIP_ER_RENAME:          return "Renaming temporary file failed";
		case ZIP_ER_CLOSE:           return "Closing zip archive failed";
		case ZIP_ER_SEEK:            return "Seek error";
		case ZIP_ER_READ:            return "Read error";
		case ZIP_ER_WRITE:           return "Write error";
		case ZIP_ER_CRC:             return "CRC error";
		case ZIP_ER_ZIPCLOSED:       return "Containing zip archive was closed";
		case ZIP_ER_NOENT:           return "No such file";
		case ZIP_ER_EXISTS:          return "File already exists";
		case ZIP_ER_OPEN:            return "Can't open file";
		case ZIP_ER_TMPOPEN:         return "Failure to create temporary file";
		case ZIP_ER_ZLIB:            return "Zlib error";
		case ZIP_ER_MEMORY:          return "Malloc failure";
		case ZIP_ER_CHANGED:         return "Entry has been changed";
		case ZIP_ER_COMPNOTSUPP:     return "Compression method not supported";
		case ZIP_ER_EOF:             return "Premature end of file";
		case ZIP_ER_INVAL:           return "Invalid argument";
		case ZIP_ER_NOZIP:           return "Not a zip archive";
		case ZIP_ER_INTERNAL:        return "Internal error";
		case ZIP_ER_INCONS:          return "Zip archive inconsistent";
		case ZIP_ER_REMOVE:          return "Can't remove file";
		case ZIP_ER_DELETED:         return "Entry has been deleted";
		case ZIP_ER_ENCRNOTSUPP:     return "Encryption method not supported";
		case ZIP_ER_RDONLY:          return "Read-only archive";
		case ZIP_ER_NOPASSWD:        return "No password provided";
		case ZIP_ER_WRONGPASSWD:     return "Wrong password provided";
		case ZIP_ER_OPNOTSUPP:       return "Operation not supported";
		case ZIP_ER_INUSE:           return "Resource still in use";
		case ZIP_ER_TELL:            return "Tell error";
		default:                     return "No error";
	}
}

inline zip_t * cast(void * archive) {
	return (zip_t *) archive;
}

inline void checkOpen(const Unzipper & instance) {
	if (!instance.isOpen()) {
		ThrowBalauException(
			  Exception::ZipException
			, "Attempt to use an unzipper instance without opening an archive"
			, Resource::File()
		);
	}
}

inline void checkOpen(const Zipper & instance) {
	if (!instance.isOpen()) {
		ThrowBalauException(
			  Exception::ZipException
			, "Attempt to use a zipper instance without opening an archive"
			, Resource::File()
		);
	}
}

///////////////////////////////////////////////////////////////////////////////

void Unzipper::open(const Resource::File & path_, bool verify) {
	open(path_, verify, "");
}

void Unzipper::open(const Resource::File & path_, bool verify, const std::string & pw) {
	openImpl(path_, verify, pw, ZIP_RDONLY);
}

std::string Unzipper::readArchiveComment() const {
	checkOpen(*this);
	const char * comment = zip_get_archive_comment(cast(archive), nullptr, 0);

	if (comment) {
		return std::string(comment);
	} else {
		return std::string();
	}
}

long long Unzipper::entryCount() const {
	checkOpen(*this);
	const long long count = zip_get_num_entries(cast(archive), 0);

	if (count < 0) {
		// Should never happen, as checkOpen is called first.
		ThrowBalauException(Exception::BugException, "zip_get_num_entries returned -1.");
	}

	return static_cast<long long>(count);
}

std::vector<std::string> Unzipper::entryNames() const {
	checkOpen(*this);
	std::vector<std::string> names;
	struct zip_stat sb {};
	zip_stat_init(&sb);
	const long long count = entryCount();

	for (long long m = 0; m < count; m++) {
		if (zip_stat_index(cast(archive), m, 0, &sb) != 0) {
			throwZipException("Could not read information for entry number " + ::toString(m));
		}

		names.emplace_back(sb.name);
	}

	return names;
}

bool Unzipper::hasEntry(const std::string & name) const {
	return getEntryIndex(name) >= 0;
}

long long Unzipper::getEntryIndex(const std::string & name) const {
	const std::string cleanName = cleanUpName(name, EntryType::Either);
	return getEntryIndexClean(cleanName);
}

std::string Unzipper::getEntryName(long long index) const {
	ZipEntryInfo info;

	try {
		getEntryInfo(index, info);
	} catch (const Exception::ZipException & e) {
		throwZipException("Could not get entry info for entry with index " + ::toString(index));
	}

	return info.name;
}

void Unzipper::getEntryInfo(const std::string & name, ZipEntryInfo & info) const {
	const std::string cleanName = cleanUpName(name, EntryType::Either);
	const long long index = getEntryIndex(cleanName);

	if (index < 0) {
		throwZipException("No entry exists with name " + cleanName);
	}

	try {
		return getEntryInfo(static_cast<long long>(index), info);
	} catch (const Exception::ZipException & e) {
		throwZipException("Could not get entry info for entry with name " + ::toString(name));
	}
}

void Unzipper::getEntryInfo(long long index, ZipEntryInfo & info) const {
	zip_stat_t sb {};

	if (zip_stat_index(cast(archive), index, 0, &sb) < 0) {
		throwZipException("Could not get entry info for entry with index " + ::toString(index));
	}

	info.name = std::string(sb.name);
	info.index = sb.index;
	info.uncompressedSize = sb.size;
	info.compressedSize = sb.comp_size;
	info.modificationTime = std::chrono::system_clock::from_time_t(sb.mtime);
	info.crc = sb.crc;
	info.compressionMethod = sb.comp_method;
	info.encryptionMethod = sb.encryption_method;
}

std::string Unzipper::readEntryComment(const std::string & name) const {
	const std::string cleanName = cleanUpName(name, EntryType::Either);
	const long long index = getEntryIndex(cleanName);

	if (index < 0) {
		throwZipException("No entry exists with name " + cleanName);
	}

	return readEntryComment(static_cast<long long>(index));
}

std::string Unzipper::readEntryComment(long long index) const {
	const char * comment = zip_file_get_comment(
		  cast(archive)
		, index
		, nullptr
		, ZIP_FL_ENC_GUESS
	);

	if (comment) {
		return std::string(comment);
	} else {
		return std::string();
	}
}

std::vector<char> Unzipper::readEntryAsBytes(const std::string & name) const {
	const std::string cleanName = cleanUpName(name, EntryType::File);
	ZipEntryInfo info {};
	getEntryInfo(cleanName, info);
	return readEntryAsBytesImpl(info, cleanName);
}

std::vector<char> Unzipper::readEntryAsBytes(long long index) const {
	ZipEntryInfo info {};
	getEntryInfo(index, info);
	return readEntryAsBytesImpl(info, "");
}

std::vector<char> Unzipper::readEntryAsBytesImpl(const ZipEntryInfo & info, const std::string & cleanName) const {
	zip_file_t * zipFile = nullptr;

	zipFile = zip_fopen_index(cast(archive), info.index, 0);

	if (!zipFile) {
		if (cleanName.empty()) {
			ThrowBalauException(
				  Exception::ZipException
				, "LibZip could not open the entry with index " + ::toString(info.index)
				, path
			);
		} else {
			ThrowBalauException(
				  Exception::ZipException
				, "LibZip could not open the entry with name " + cleanName
				, path
			);
		}
	}

	OnScopeExit fileCloser([zipFile] () { zip_fclose(zipFile); });
	std::vector<char> bytes(info.uncompressedSize);
	long long bytesRemaining = info.uncompressedSize;
	char * data = bytes.data();

	while (bytesRemaining > 0) {
		long long bytesRead = zip_fread(zipFile, data, bytesRemaining);

		if (bytesRead < 0) {
			// TODO check that this is the right thing to do.
			if (cleanName.empty()) {
				throwZipException("Could not read data for entry with index " + ::toString(info.index));
			} else {
				throwZipException("Could not read data for entry with name " + cleanName);
			}
		}

		data += bytesRead;
		bytesRemaining -= bytesRead;
	}

	return bytes;
}

std::string Unzipper::readEntryAsString(const std::string & name) const {
	const std::string cleanName = cleanUpName(name, EntryType::File);
	ZipEntryInfo info {};
	getEntryInfo(cleanName, info);
	return readEntryAsStringImpl(info, cleanName);
}

std::string Unzipper::readEntryAsString(long long index) const {
	ZipEntryInfo info {};
	getEntryInfo(index, info);
	return readEntryAsStringImpl(info, "");
}

std::string Unzipper::readEntryAsStringImpl(const ZipEntryInfo & info, const std::string & cleanName) const {
	zip_file_t * zipFile = nullptr;

	zipFile = zip_fopen_index(cast(archive), info.index, 0);

	if (!zipFile) {
		if (cleanName.empty()) {
			ThrowBalauException(
				  Exception::ZipException
				, "LibZip could not open the entry with index " + ::toString(info.index)
				, path
			);
		} else {
			ThrowBalauException(
				  Exception::ZipException
				, "LibZip could not open the entry with name " + cleanName
				, path
			);
		}
	}

	OnScopeExit fileCloser([zipFile] () { zip_fclose(zipFile); });
	std::string str;
	long long bytesRemaining = info.uncompressedSize;
	std::array<char, 1024 * 2> buffer {};

	while (bytesRemaining > 0) {
		long long bytesRead = zip_fread(zipFile, buffer.data(), bytesRemaining);

		if (bytesRead < 0) {
			// TODO check that this is the right thing to do.
			if (cleanName.empty()) {
				throwZipException("Could not read data for entry with index " + ::toString(info.index));
			} else {
				throwZipException("Could not read data for entry with name " + cleanName);
			}

		}

		str += std::string(buffer.data(), static_cast<long long>(bytesRead));
		bytesRemaining -= bytesRead;
	}

	return str;
}

void Unzipper::close() {
	if (!isOpen()) {
		return;
	}

	zip_discard(cast(archive));
	archive = nullptr;
	path = Resource::File();
}

Unzipper::~Unzipper() {
	close();
}

inline bool isDirectoryName(const std::string & name) {
	return Strings::endsWith(name, "/");
}

void Unzipper::openImpl(const Resource::File & path_, bool verify, const std::string & pw, int mode) {
	int flags = mode;
	const std::string modeStr = mode == ZIP_CREATE ? "writing" : "reading";

	if (verify) {
		flags |= ZIP_CHECKCONS;
	}

	int error = 0;
	archive = zip_open(path_.toRawString().c_str(), flags, &error);

	if (error) {
		const std::string errorStr = getLibZipErrorAsString(error);

		ThrowBalauException(
			Exception::ZipException, "Could not open zip archive for " + modeStr + ": " + errorStr, path_
		);
	}

	if (!pw.empty()) {
		if (zip_set_default_password(cast(archive), pw.c_str()) < 0) {
			const char * errorString = zip_error_strerror(zip_get_error(cast(archive)));
			close();

			ThrowBalauException(
				  Exception::ZipException
				, "Could not set password for archive: " + std::string(errorString) + "."
				, path_
			);
		}
	}

	path = path_;
}

std::string Unzipper::cleanUpName(const std::string & name, EntryType entryType) const {
	std::string cleanedName = std::string(Strings::trim(name));

	switch (entryType) {
		case EntryType::Directory: {
			if (!isDirectoryName(name)) {
				ThrowBalauException(
					  Exception::ZipException
					, "The supplied entry directory name does not end with a forward slash: " + name
					, path
				);
			}

			break;
		}

		case EntryType::File: {
			if (isDirectoryName(name)) {
				ThrowBalauException(
					  Exception::ZipException
					, "The supplied entry file name ends with a forward slash: " + name
					, path
				);
			}

			break;
		}

		case EntryType::Either: {
			break;
		}
	}

	static std::regex removeMultipleSeparators("//+");
	static std::regex removeFrontSeparator("^/");

	cleanedName = Strings::replaceAll(cleanedName, removeMultipleSeparators, "/");
	return Strings::replaceAll(cleanedName, removeFrontSeparator, "");
}

long long Unzipper::getEntryIndexClean(const std::string & cleanName) const {
	checkOpen(*this);
	zip_flags_t flags = ZIP_FL_ENC_GUESS;
	return zip_name_locate(cast(archive), cleanName.c_str(), flags);
}

void Unzipper::throwZipException(const std::string & errorMessage) const {
	const char * errorString = zip_error_strerror(zip_get_error(cast(archive)));
	ThrowBalauException(Exception::ZipException, errorMessage + ": " + std::string(errorString) + ".", path);
}

///////////////////////////////////////////////////////////////////////////////

void Zipper::open(const Resource::File & path_, bool verify) {
	open(path_, verify, "");
}

void Zipper::open(const Resource::File & path_, bool verify, const std::string & pw) {
	openImpl(path_, verify, pw, ZIP_CREATE);
}

void Zipper::putArchiveComment(const std::string & text) {
	checkOpen(*this);
	const auto length = static_cast<zip_uint16_t>(text.length() <= USHRT_MAX ? text.length() : USHRT_MAX);

	if (zip_set_archive_comment(cast(archive), text.c_str(), length) != 0) {
		throwZipException("Could not set archive comment");
	}
}

void Zipper::deleteComment() {
	putArchiveComment("");
}

void Zipper::putDirectory(const std::string & name) {
	checkOpen(*this);

	if (name.empty() || name == "/") {
		return;
	}

	const std::string cleanName = cleanUpName(name, EntryType::Directory);

	if (hasEntry(cleanName)) {
		return;
	}

	const std::string cleanNameNoEndSlash = cleanName.substr(0, cleanName.length() - 1);
	const std::vector<std::string_view> components = Strings::split(cleanNameNoEndSlash, "/");
	std::string directoryComponents;
	size_t m = 0;

	while (m < components.size()) {
		directoryComponents += std::string(components[m]) + "/";

		if (!hasEntry(directoryComponents)) {
			if (zip_dir_add(cast(archive), directoryComponents.c_str(), ZIP_FL_ENC_GUESS) == -1) {
				throwZipException("Could not create directory " + directoryComponents);
			}
		}

		m++;
	}
}

void Zipper::putEntry(const std::string & name, const Resource::File & bytes) {
	putEntry(name, [&bytes, this] () { return zip_source_file(cast(archive), bytes.toRawString().c_str(), 0, 0); });
}

void Zipper::putEntry(const std::string & name, const std::vector<char> & bytes) {
	putEntry(name, [&bytes, this] () { return zip_source_buffer(cast(archive), bytes.data(), bytes.size(), 0); });
}

void Zipper::putEntry(const std::string & name, const std::string & bytes) {
	putEntry(name, [&bytes, this] () { return zip_source_buffer(cast(archive), bytes.c_str(), bytes.length(), 0); });
}

void Zipper::renameEntry(const std::string & name, const std::string & newName) {
	checkOpen(*this);
	const std::string cleanName = cleanUpName(name, EntryType::Either);

	if (isDirectoryName(cleanName)) {
		const std::string newCleanName = cleanUpName(newName, EntryType::Directory);
		renameDirectory(cleanName, newCleanName);
	} else {
		const std::string newCleanName = cleanUpName(newName, EntryType::File);
		renameFile(cleanName, newCleanName);
	}
}

void Zipper::deleteEntry(const std::string & name) {
	checkOpen(*this);
	const std::string cleanName = cleanUpName(name, EntryType::Either);

	if (isDirectoryName(cleanName)) {
		deleteDirectory(cleanName);
	} else {
		deleteFile(cleanName);
	}
}

void Zipper::close() {
	if (!isOpen()) {
		return;
	}

	zip_discard(cast(archive));
	archive = nullptr;
	path = Resource::File();
}

void Zipper::commit() {
	if (!isOpen()) {
		return;
	}

	if (zip_close(cast(archive)) != 0) {
		throwZipException("Failed to commit changes to zip archive");
	}

	archive = nullptr;
	path = Resource::File();
}

template <typename SourceFunctionT> void Zipper::putEntry(const std::string & name, SourceFunctionT getSource) {
	checkOpen(*this);
	const std::string cleanName = cleanUpName(name, EntryType::File);
	const size_t lastSlash = Strings::lastIndexOf(cleanName, "/");

	if (lastSlash != std::string::npos) {
		putDirectory(cleanName.substr(0, lastSlash + 1));
	}

	zip_source * const zipSource = getSource();

	if (zipSource != nullptr) {
		if (zip_file_add(cast(archive), cleanName.c_str(), zipSource, ZIP_FL_OVERWRITE | ZIP_FL_ENC_UTF_8) >= 0) {
			// LibZip has deleted the source object automatically.
			return;
		} else {
			zip_source_free(zipSource);
			throwZipException("Attempt to add data to entry " + name + " failed with error");
		}
	}

	throwZipException("Could not create a zip source object to write with");
}

void Zipper::renameDirectory(const std::string & cleanName, const std::string & newCleanName) {
	std::vector<std::string> names = entryNames();

	for (auto & entryName : names) {
		if (Strings::startsWith(entryName, cleanName)) {
			std::string newEntryName = newCleanName + entryName.substr(cleanName.length());
			renameFile(cleanName, newEntryName);
		}
	}
}

void Zipper::renameFile(const std::string & cleanName, const std::string & newCleanName) {
	const long long index = getEntryIndex(cleanName);

	if (zip_file_rename(cast(archive), static_cast<long long>(index), newCleanName.c_str(), 0) != 0) {
		ThrowBalauException(Exception::ZipException, "Unable to rename entry: " + cleanName, path);
	}
}

void Zipper::deleteDirectory(const std::string & cleanName) {
	std::vector<std::string> names = entryNames();

	for (auto & entryName : names) {
		if (Strings::startsWith(entryName, cleanName)) {
			deleteFile(cleanName);
		}
	}
}

void Zipper::deleteFile(const std::string & cleanName) {
	const long long index = getEntryIndex(cleanName);

	if (zip_delete(cast(archive), static_cast<long long>(index)) != 0) {
		ThrowBalauException(Exception::ZipException, "Unable to delete entry: " + cleanName, path);
	}
}

} // namespace Balau::Util
