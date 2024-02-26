// @formatter:off
//
// Balau core C++ library
//
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
/// @file Zip.hpp
///
/// Zipper and Unzipper classes.
///

#ifndef COM_BORA_SOFTWARE_BALAU_UTIL_ZIP
#define COM_BORA_SOFTWARE_BALAU_UTIL_ZIP

#include <Balau/Exception/ResourceExceptions.hpp>
#include <Balau/Resource/File.hpp>
#include "Balau/Type/OnScopeExit.hpp"
#include <Balau/Util/DateTime.hpp>
#include <Balau/Util/Files.hpp>

#include <boost/iostreams/categories.hpp>
#include <boost/iostreams/concepts.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/device/file_descriptor.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filtering_streambuf.hpp>

#include <algorithm>
#include <chrono>
#include <iosfwd>
#include <sstream>

#define _Nonnull
#define _Nullable
#include <zip.h>

namespace Balau {

namespace Exception {

///
/// Thrown when a zip resource has an error.
///
class ZipException : public Exception::ResourceException {
	public: const Resource::File file;

	public: ZipException(SourceCodeLocation location, const std::string & st, const Resource::File & file_)
		: ResourceException(location, st, "Zip", file_.clone())
		, file(file_) {}

	public: ZipException(const std::string & st, const Resource::File & file_)
		: ResourceException(st, "Zip", file_.clone())
		, file(file_) {}

	public: ZipException(SourceCodeLocation location,
	                     const std::string & st,
	                     const std::string & message_,
	                     const Resource::File & file_)
		: ResourceException(location, st, "Zip", message_, file_.clone())
		, file(file_) {}

	public: ZipException(const std::string & st,
	                     const std::string & message_,
	                     const Resource::File & file_)
		: ResourceException(st, "Zip", message_, file_.clone())
		, file(file_) {}
};

inline bool operator == (const ZipException & lhs, const ZipException & rhs) {
	return lhs.message == rhs.message && lhs.file == rhs.file;
}

} // namespace Exception

namespace Resource::Impl {

class ZipEntrySource;
class ZipEntrySink;

} // namespace Resource::Impl

namespace Util {

class Unzipper;
class Zipper;

namespace Impl {

template <typename Unused = int>
class ZipUtilities {
	static std::string getLibZipErrorAsString(int error);
	static zip_t * cast(void * archive);
	static void checkOpen(const Unzipper & instance);
	static void checkOpen(const Zipper & instance);
	friend class ::Balau::Util::Unzipper;
	friend class ::Balau::Util::Zipper;
};

} // namespace Impl

///
/// Information about a zip archive entry (file or directory).
///
/// This structure is equivalent to LibZip's zip_stat structure.
///
struct ZipEntryInfo {
	///
	/// The name of the entry.
	///
	std::string name;

	///
	/// The entry index within the archive.
	///
	long long index;

	///
	/// The uncompressed size of the entry.
	///
	long long uncompressedSize;

	///
	/// The compressed size of the entry.
	///
	long long compressedSize;

	///
	/// The last modification time.
	///
	std::chrono::system_clock::time_point modificationTime;

	///
	/// The CRC of the entry's data.
	///
	unsigned int crc;

	///
	/// The compression method used.
	///
	unsigned short compressionMethod;

	///
	/// The encryption method used.
	///
	unsigned short encryptionMethod;
};

inline bool operator == (const ZipEntryInfo & lhs, const ZipEntryInfo & rhs) {
	return lhs.name == rhs.name
		&& lhs.index == rhs.index
		&& lhs.uncompressedSize == rhs.uncompressedSize
		&& lhs.compressedSize == rhs.compressedSize
		&& lhs.modificationTime == rhs.modificationTime
		&& lhs.crc == rhs.crc
		&& lhs.compressionMethod == rhs.compressionMethod
		&& lhs.encryptionMethod == rhs.encryptionMethod;
}

///
/// Print the zip entry info as a UTF-8 string.
///
/// @return a UTF-8 string representing the zip entry info
///
template <typename AllocatorT>
inline U8String<AllocatorT> toString(const ZipEntryInfo & info) {
	return "{ " + Strings::join<AllocatorT>(
		  ", "
		, info.name
		, info.index
		, info.uncompressedSize
		, info.compressedSize
		// TODO
//		, info.modificationTime
		, info.crc
		, info.compressionMethod
		, info.encryptionMethod
	) + " }";
}

///
/// Print the zip entry info as a UTF-8 string.
///
/// @return a UTF-8 string representing the zip entry info
///
inline std::string toString(const ZipEntryInfo & info) {
	return "{ " + Strings::join(
		  ", "
		, info.name
		, info.index
		, info.uncompressedSize
		, info.compressedSize
		// TODO
//		, info.modificationTime
		, info.crc
		, info.compressionMethod
		, info.encryptionMethod
	) + " }";
}

///
/// Random access to the entries in a zip archive.
///
/// Zip archive entries can be accessed via their names or their indices.
///
/// Unzipper instances are not thread safe and thus must be used in a single thread.
///
class Unzipper {
	///
	/// Open the specified archive for reading.
	///
	/// @param path_ the path to the archive
	/// @param verify if true, the implementation will first verify the archive
	/// @throw InvalidArchiveException if verify is true and the archive is deemed to be invalid
	///
	public: virtual void open(const Resource::File & path_, bool verify) {
		open(path_, verify, "");
	}

	///
	/// Open the specified archive for reading.
	///
	/// @param path_ the path to the archive
	/// @param verify if true, the implementation will first verify the archive
	/// @param pw the archive's password if encryption was used when writing
	/// @throw InvalidArchiveException if verify is true and the archive is deemed to be invalid
	///
	public: virtual void open(const Resource::File & path_, bool verify, const std::string & pw) {
		openImpl(path_, verify, pw, ZIP_RDONLY);
	}

	///
	/// Is an archive currently open?
	///
	/// @return true if the archive is currently open
	///
	public: bool isOpen() const {
		return archive != nullptr;
	}

	///
	/// Get the file URI that this unzipper accesses.
	///
	/// @return the file URI that this unzipper accesses
	///
	public: Resource::File getPath() const {
		return path;
	}

	///
	/// Get the comment on the archive if one exists.
	///
	/// If the archive is being written to (via Zipper) and is new, this will
	/// return an empty string.
	///
	/// @return the comment on the archive if one exists or empty string
	/// @throw ZipException if the archive is not open
	///
	public: std::string readArchiveComment() const {
		Impl::ZipUtilities<>::checkOpen(*this);
		const char * comment = zip_get_archive_comment(Impl::ZipUtilities<>::cast(archive), nullptr, 0);

		if (comment) {
			return std::string(comment);
		} else {
			return std::string();
		}
	}

	///
	/// Get the number of entries in the original archive if one exists.
	///
	/// If the archive is being written to (via Zipper) and is new, this will
	/// return zero.
	///
	/// @return the number of entries in the original archive if one exists or zero
	/// @throw ZipException if the archive is not open
	///
	public: long long entryCount() const {
		Impl::ZipUtilities<>::checkOpen(*this);
		const long long count = zip_get_num_entries(Impl::ZipUtilities<>::cast(archive), 0);

		if (count < 0) {
			// Should never happen, as checkOpen is called first.
			ThrowBalauException(Exception::BugException, "zip_get_num_entries returned -1.");
		}

		return static_cast<long long>(count);
	}

	///
	/// Get the entry names in the original archive if one exists.
	///
	/// If the archive is being written to (via Zipper) and is new, this will
	/// return an empty vector.
	///
	/// @return the entry names in the original archive if one exists or an empty vector
	/// @throw ZipException if the archive is not open
	///
	public: std::vector<std::string> entryNames() const {
		Impl::ZipUtilities<>::checkOpen(*this);
		std::vector<std::string> names;
		struct zip_stat sb {};
		zip_stat_init(&sb);
		const long long count = entryCount();

		for (long long m = 0; m < count; m++) {
			if (zip_stat_index(Impl::ZipUtilities<>::cast(archive), m, 0, &sb) != 0) {
				throwZipException("Could not read information for entry number " + ::toString(m));
			}

			names.emplace_back(sb.name);
		}

		return names;
	}

	///
	/// Does the original archive have an entry with the specified name?
	///
	/// If the archive being written to is new, this will always return false.
	///
	/// @param name the name of the entry
	/// @return true if the original archive has an entry with the specified name
	/// @throw ZipException if the archive is not open
	///
	public: bool hasEntry(const std::string & name) const {
		return getEntryIndex(name) >= 0;
	}

	///
	/// Get the index of the entry that corresponds to the supplied name.
	///
	/// @param name the name of the entry
	/// @return the index of the entry that corresponds to the supplied name
	/// @throw ZipException if there is no such entry
	/// @throw ZipException if the archive is not open
	///
	public: long long getEntryIndex(const std::string & name) const {
		const std::string cleanName = cleanUpName(name, EntryType::Either);
		return getEntryIndexClean(cleanName);
	}

	///
	/// Get the name of the entry that corresponds to the supplied index.
	///
	/// @param index the index of the entry
	/// @return the name of the entry that corresponds to the supplied index
	/// @throw ZipException if the entry name could not be obtained
	/// @throw ZipException if the archive is not open
	///
	public: std::string getEntryName(long long index) const {
		ZipEntryInfo info;

		try {
			getEntryInfo(index, info);
		} catch (const Exception::ZipException & e) {
			throwZipException("Could not get entry info for entry with index " + ::toString(index));
		}

		return info.name;
	}

	///
	/// Get information on the entry with the specified name.
	///
	/// A valid entry name must first be obtained by calling getEntryPaths().
	///
	/// @param name the name of the entry
	/// @param info the zip entry info structure that will be filled with the info
	/// @throw ZipException if the entry info could not be obtained
	/// @throw ZipException if the archive is not open
	///
	public: void getEntryInfo(const std::string & name, ZipEntryInfo & info) const {
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

	///
	/// Get information on the entry with the specified index.
	///
	/// @param index the index of the entry
	/// @param info the zip entry info structure that will be filled with the info
	/// @throw ZipException if the entry info could not be obtained
	/// @throw ZipException if the archive is not open
	///
	public: void getEntryInfo(long long index, ZipEntryInfo & info) const {
		zip_stat_t sb {};

		if (zip_stat_index(Impl::ZipUtilities<>::cast(archive), index, 0, &sb) < 0) {
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

	///
	/// Get the comment on the entry if one exists.
	///
	/// A valid entry name must first be obtained by calling getEntryPaths().
	///
	/// @return the comment on the entry if one exists or an empty string
	/// @throw ZipException if the entry comment could not be obtained
	/// @throw ZipException if the archive is not open
	///
	public: std::string readEntryComment(const std::string & name) const {
		const std::string cleanName = cleanUpName(name, EntryType::Either);
		const long long index = getEntryIndex(cleanName);

		if (index < 0) {
			throwZipException("No entry exists with name " + cleanName);
		}

		return readEntryComment(static_cast<long long>(index));
	}

	///
	/// Get the comment on the entry if one exists.
	///
	/// @return the comment on the entry if one exists or an empty string
	/// @throw ZipException if the archive is not open
	///
	/// @throw ZipException if the entry comment could not be obtained
	///
	public: std::string readEntryComment(long long index) const {
		const char * comment = zip_file_get_comment(
			Impl::ZipUtilities<>::cast(archive)
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

	///
	/// Get the contents of the specified entry in the original archive if one exists.
	///
	/// A valid entry name must first be obtained by calling getEntryPaths().
	///
	/// @param name the name of the entry
	/// @return the contents of the specified entry as a char vector
	/// @throw ZipException if the entry could not be obtained
	/// @throw ZipException if the archive is not open
	///
	public: std::vector<char> readEntryAsBytes(const std::string & name) const {
		const std::string cleanName = cleanUpName(name, EntryType::File);
		ZipEntryInfo info {};
		getEntryInfo(cleanName, info);
		return readEntryAsBytesImpl(info, cleanName);
	}

	///
	/// Get the contents of the specified entry in the original archive if one exists.
	///
	/// @param index the index of the entry
	/// @return the contents of the specified entry as a char vector
	/// @throw ZipException if the entry could not be obtained
	/// @throw ZipException if the archive is not open
	///
	public: std::vector<char> readEntryAsBytes(long long index) const {
		ZipEntryInfo info {};
		getEntryInfo(index, info);
		return readEntryAsBytesImpl(info, "");
	}

	///
	/// Get the contents of the specified entry in the original archive if one exists.
	///
	/// A valid entry name must first be obtained by calling getEntryPaths().
	///
	/// @param name the name of the entry
	/// @return the contents of the specified entry as a UTF-8 string
	/// @throw ZipException if the entry could not be obtained
	/// @throw ZipException if the archive is not open
	///
	public: std::string readEntryAsString(const std::string & name) const {
		const std::string cleanName = cleanUpName(name, EntryType::File);
		ZipEntryInfo info {};
		getEntryInfo(cleanName, info);
		return readEntryAsStringImpl(info, cleanName);
	}

	///
	/// Get the contents of the specified entry in the original archive if one exists.
	///
	/// @param index the index of the entry
	/// @return the contents of the specified entry as a UTF-8 string
	/// @throw ZipException if the entry could not be obtained
	/// @throw ZipException if the archive is not open
	///
	public: std::string readEntryAsString(long long index) const {
		ZipEntryInfo info {};
		getEntryInfo(index, info);
		return readEntryAsStringImpl(info, "");
	}

	///
	/// Close the archive.
	/// If the archive has been modified, this will rollback the transaction.
	/// The archive file will be left untouched. If the transaction is working
	/// on a new archive, no file will be created.
	///
	///
	/// In order to continue using the instance, a subsequent call to open() is
	/// necessary.
	///
	/// If close is called without a corresponding previous call to open(),
	/// the result will be a NOP.
	///
	public: virtual void close() {
		if (!isOpen()) {
			return;
		}

		zip_discard(Impl::ZipUtilities<>::cast(archive));
		archive = nullptr;
		path = Resource::File();
	}

	///
	/// Destroy the unzipper, closing the archive if it is open.
	///
	public: virtual ~Unzipper() {
		close();
	}

	///////////////////////// Private implementation //////////////////////////

	friend class Resource::Impl::ZipEntrySource;
	friend class Resource::Impl::ZipEntrySink;

	private: std::vector<char> readEntryAsBytesImpl(const ZipEntryInfo & info, const std::string & cleanName) const {
		zip_file_t * zipFile = nullptr;

		zipFile = zip_fopen_index(Impl::ZipUtilities<>::cast(archive), info.index, 0);

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

	private: std::string readEntryAsStringImpl(const ZipEntryInfo & info, const std::string & cleanName) const {
		zip_file_t * zipFile = nullptr;

		zipFile = zip_fopen_index(Impl::ZipUtilities<>::cast(archive), info.index, 0);

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

	// Used in the cleanUpName method.
	protected: enum class EntryType {
		Directory, File, Either
	};

	protected: static bool isDirectoryName(const std::string & name) {
		return Strings::endsWith(name, "/");
	}

	protected: void openImpl(const Resource::File & path_, bool verify, const std::string & pw, int mode) {
		int flags = mode;
		const std::string modeStr = mode == ZIP_CREATE ? "writing" : "reading";

		if (verify) {
			flags |= ZIP_CHECKCONS;
		}

		int error = 0;
		archive = zip_open(path_.toRawString().c_str(), flags, &error);

		if (error) {
			const std::string errorStr = Impl::ZipUtilities<>::getLibZipErrorAsString(error);

			ThrowBalauException(
				Exception::ZipException, "Could not open zip archive for " + modeStr + ": " + errorStr, path_
			);
		}

		if (!pw.empty()) {
			if (zip_set_default_password(Impl::ZipUtilities<>::cast(archive), pw.c_str()) < 0) {
				const char * errorString = zip_error_strerror(zip_get_error(Impl::ZipUtilities<>::cast(archive)));
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

	// Clean up the supplied name's separator characters.
	protected: std::string cleanUpName(const std::string & name, EntryType entryType) const {
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

	protected: long long getEntryIndexClean(const std::string & cleanName) const {
		Impl::ZipUtilities<>::checkOpen(*this);
		zip_flags_t flags = ZIP_FL_ENC_GUESS;
		return zip_name_locate(Impl::ZipUtilities<>::cast(archive), cleanName.c_str(), flags);
	}

	protected: void throwZipException(const std::string & errorMessage) const {
		const char * errorString = zip_error_strerror(zip_get_error(Impl::ZipUtilities<>::cast(archive)));
		ThrowBalauException(Exception::ZipException, errorMessage + ": " + std::string(errorString) + ".", path);
	}

	protected: void * archive;
	protected: Resource::File path;
};

///
/// Transactional modification of entries in a new or existing zip archive.
///
/// No mutation is performed on the existing archive if the zipper is acting on
/// an existing file. When commit() is called, the existing archive will be
/// replaced with a revised file containing original unmodified data from the
/// original file, plus data for new or revised entries added during the transaction.
///
/// In addition to transactional zipping, an initialised zipper instance
/// provides random access to the original zip archive if there is one. All read
/// calls will return data from the original archive only.
///
/// Zipper instances are not thread safe and thus must be used in a single thread.
///
class Zipper : public Unzipper {
	///
	/// Begin a transaction to write to the specified archive.
	///
	/// If the archive does not exist, it is created and written to when commit()
	/// is called.
	///
	/// If the archive does exist, a new archive is created from the original one when
	/// commit() is called, with the modifications made in the transaction.
	///
	/// @param path_ the path to the archive
	/// @param verify if true, the implementation will first verify the archive
	/// @throw InvalidArchiveException if verify is true and the archive is deemed to be invalid
	///
	public: void open(const Resource::File & path_, bool verify) override {
		open(path_, verify, "");
	}

	///
	/// Begin a transaction to write to the specified archive.
	///
	/// If the archive does not exist, it is created and written to when commit()
	/// is called.
	///
	/// If the archive does exist, a new archive is created from the original one when
	/// commit() is called, with the modifications made in the transaction.
	///
	/// @param path_ the path to the archive
	/// @param verify if true, the implementation will first verify the archive
	/// @param pw the archive's password if encryption was used when writing
	/// @throw InvalidArchiveException if verify is true and the archive is deemed to be invalid
	///
	public: void open(const Resource::File & path_, bool verify, const std::string & pw) override {
		openImpl(path_, verify, pw, ZIP_CREATE);
	}

	///
	/// Set the comment in the archive, overwriting any existing comment.
	///
	/// @param text the text content to set the comment to
	/// @throw ZipException if the archive comment could not be set
	/// @throw ZipException if the archive is not open
	///
	public: void putArchiveComment(const std::string & text) {
		Impl::ZipUtilities<>::checkOpen(*this);
		const auto length = static_cast<zip_uint16_t>(text.length() <= USHRT_MAX ? text.length() : USHRT_MAX);

		if (zip_set_archive_comment(Impl::ZipUtilities<>::cast(archive), text.c_str(), length) != 0) {
			throwZipException("Could not set archive comment");
		}
	}

	///
	/// Deletes the comment in the archive.
	///
	/// @throw ZipException if the archive comment could not be deleted
	/// @throw ZipException if the archive is not open
	///
	public: void deleteComment() {
		putArchiveComment("");
	}

	///
	/// Ensure that the specified directory exists.
	///
	/// All parent directories will also be created if not present.
	///
	/// @param name the directory name
	/// @throw ZipException if a directory could not be created
	/// @throw ZipException if the archive is not open
	///
	public: void putDirectory(const std::string & name) {
		Impl::ZipUtilities<>::checkOpen(*this);

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
				if (zip_dir_add(Impl::ZipUtilities<>::cast(archive), directoryComponents.c_str(), ZIP_FL_ENC_GUESS) == -1) {
					throwZipException("Could not create directory " + directoryComponents);
				}
			}

			m++;
		}
	}

	///
	/// Create or overwrite an entry from the data in the supplied file.
	///
	/// @param name the name of the entry to create or overwrite
	/// @param bytes the file from which the data to create the entry with will be sourced
	/// @throw ZipException if the entry could not be created
	/// @throw ZipException if the archive is not open
	///
	public: void putEntry(const std::string & name, const Resource::File & bytes) {
		putEntry(name, [&bytes, this] () { return zip_source_file(Impl::ZipUtilities<>::cast(archive), bytes.toRawString().c_str(), 0, 0); });
	}

	///
	/// Create or overwrite an entry from the supplied std::vector<char>.
	///
	/// Note that all in memory supplied data will be copied before committing,
	/// so files should be used if the data is very large.
	///
	/// @param name the name of the entry to create or overwrite
	/// @param bytes the data to create the entry with
	/// @throw ZipException if the entry could not be created
	/// @throw ZipException if the archive is not open
	///
	public: void putEntry(const std::string & name, const std::vector<char> & bytes) {
		putEntry(name, [&bytes, this] () { return zip_source_buffer(Impl::ZipUtilities<>::cast(archive), bytes.data(), bytes.size(), 0); });
	}

	///
	/// Create or overwrite an entry from the supplied string.
	///
	/// Note that all in memory supplied data will be copied before committing,
	/// so files should be used if the data is very large.
	///
	/// @param name the name of the entry to create or overwrite
	/// @param bytes the data to create the entry with
	/// @throw ZipException if the entry could not be created
	/// @throw ZipException if the archive is not open
	///
	public: void putEntry(const std::string & name, const std::string & bytes) {
		putEntry(name, [&bytes, this] () { return zip_source_buffer(Impl::ZipUtilities<>::cast(archive), bytes.c_str(), bytes.length(), 0); });
	}

	///
	/// Rename an existing entry in the original archive.
	///
	/// If the entry is a directory, all files under that directory will also be renamed.
	///
	/// @param name the name of the existing entry in the original archive
	/// @param newName the new name to give to the entry
	/// @throw ZipException if the entry could not be renamed
	/// @throw ZipException if the archive is not open
	///
	public: void renameEntry(const std::string & name, const std::string & newName) {
		Impl::ZipUtilities<>::checkOpen(*this);
		const std::string cleanName = cleanUpName(name, EntryType::Either);

		if (isDirectoryName(cleanName)) {
			const std::string newCleanName = cleanUpName(newName, EntryType::Directory);
			renameDirectory(cleanName, newCleanName);
		} else {
			const std::string newCleanName = cleanUpName(newName, EntryType::File);
			renameFile(cleanName, newCleanName);
		}
	}

	///
	/// Delete an existing entry in the original archive.
	///
	/// If the entry is a directory, all files under that directory will also be deleted.
	///
	/// @param name the name of the existing entry in the original archive
	/// @throw ZipException if the entry could not be deleted
	/// @throw ZipException if the archive is not open
	///
	public: void deleteEntry(const std::string & name) {
		Impl::ZipUtilities<>::checkOpen(*this);
		const std::string cleanName = cleanUpName(name, EntryType::Either);

		if (isDirectoryName(cleanName)) {
			deleteDirectory(cleanName);
		} else {
			deleteFile(cleanName);
		}
	}

	///
	/// Rollback the transaction and close the archive.
	///
	/// If the transaction is working on an existing archive, the file will be
	/// left untouched. If the transaction is working on a new archive, no file
	/// will be created.
	///
	/// In order to continue using the instance, a subsequent call to open() is
	/// necessary.
	///
	/// If close is called without a corresponding previous call to open(),
	/// the result will be a NOP.
	///
	public: void close() override {
		if (!isOpen()) {
			return;
		}

		zip_discard(Impl::ZipUtilities<>::cast(archive));
		archive = nullptr;
		path = Resource::File();
	}

	///
	/// Commit the transaction and close the archive.
	///
	/// If the transaction is working on an existing archive, the file will be
	/// replaced with the revised file. If the transaction is working on a new
	/// archive, the file will be created.
	///
	/// In order to continue using the instance, a subsequent call to open() is
	/// necessary.
	///
	/// If commit is called without a corresponding previous call to open(),
	/// the result will be a NOP.
	///
	/// If this call fails, a ZipException will be thrown and the zipper instance
	/// will still contain the pending changes. In this case, a subsequent call
	/// to close() will be necessary in order to abandon the changes and close
	/// the archive.
	///
	public: void commit() {
		if (!isOpen()) {
			return;
		}

		if (zip_close(Impl::ZipUtilities<>::cast(archive)) != 0) {
			throwZipException("Failed to commit changes to zip archive");
		}

		archive = nullptr;
		path = Resource::File();
	}

	////////////////////////// Private implementation /////////////////////////

	// Common implementation for putEntry methods.
	private: template <typename SourceFunctionT> void putEntry(const std::string & name, SourceFunctionT getSource) {
		Impl::ZipUtilities<>::checkOpen(*this);
		const std::string cleanName = cleanUpName(name, EntryType::File);
		const size_t lastSlash = Strings::lastIndexOf(cleanName, "/");

		if (lastSlash != std::string::npos) {
			putDirectory(cleanName.substr(0, lastSlash + 1));
		}

		zip_source * const zipSource = getSource();

		if (zipSource != nullptr) {
			if (zip_file_add(Impl::ZipUtilities<>::cast(archive), cleanName.c_str(), zipSource, ZIP_FL_OVERWRITE | ZIP_FL_ENC_UTF_8) >= 0) {
				// LibZip has deleted the source object automatically.
				return;
			} else {
				zip_source_free(zipSource);
				throwZipException("Attempt to add data to entry " + name + " failed with error");
			}
		}

		throwZipException("Could not create a zip source object to write with");
	}

	// Rename a directory entry and all its descendants.
	private: void renameDirectory(const std::string & cleanName, const std::string & newCleanName) {
		std::vector<std::string> names = entryNames();

		for (auto & entryName : names) {
			if (Strings::startsWith(entryName, cleanName)) {
				std::string newEntryName = newCleanName + entryName.substr(cleanName.length());
				renameFile(cleanName, newEntryName);
			}
		}
	}

	// Rename a file entry.
	private: void renameFile(const std::string & cleanName, const std::string & newCleanName) {
		const long long index = getEntryIndex(cleanName);

		if (zip_file_rename(Impl::ZipUtilities<>::cast(archive), static_cast<long long>(index), newCleanName.c_str(), 0) != 0) {
			ThrowBalauException(Exception::ZipException, "Unable to rename entry: " + cleanName, path);
		}
	}

	// Delete a directory entry and all its descendants.
	private: void deleteDirectory(const std::string & cleanName) {
		std::vector<std::string> names = entryNames();

		for (auto & entryName : names) {
			if (Strings::startsWith(entryName, cleanName)) {
				deleteFile(cleanName);
			}
		}
	}

	// Delete a file entry.
	private: void deleteFile(const std::string & cleanName) {
		const long long index = getEntryIndex(cleanName);

		if (zip_delete(Impl::ZipUtilities<>::cast(archive), static_cast<long long>(index)) != 0) {
			ThrowBalauException(Exception::ZipException, "Unable to delete entry: " + cleanName, path);
		}
	}
};

namespace Impl {

template <typename Unused>
std::string ZipUtilities<Unused>::getLibZipErrorAsString(int error) {
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

template <typename Unused>
zip_t * ZipUtilities<Unused>::cast(void * archive) {
	return (zip_t *) archive;
}

template <typename Unused>
void ZipUtilities<Unused>::checkOpen(const Unzipper & instance) {
	if (!instance.isOpen()) {
		ThrowBalauException(
			Exception::ZipException
		, "Attempt to use an unzipper instance without opening an archive"
		, Resource::File()
		);
	}
}

template <typename Unused>
void ZipUtilities<Unused>::checkOpen(const Zipper & instance) {
	if (!instance.isOpen()) {
		ThrowBalauException(
			Exception::ZipException
		, "Attempt to use a zipper instance without opening an archive"
		, Resource::File()
		);
	}
}

} // namespace Impl

} // namespace Util

} // namespace Balau

#endif // COM_BORA_SOFTWARE_BALAU_UTIL_ZIP
