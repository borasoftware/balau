// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

///
/// @file Compression.hpp
///
/// Compression utilities: gzip and zip reading/writing.
///

#ifndef COM_BORA_SOFTWARE__BALAU_UTIL__COMPRESSION
#define COM_BORA_SOFTWARE__BALAU_UTIL__COMPRESSION

#include <Balau/Exception/ResourceExceptions.hpp>
#include <Balau/Resource/File.hpp>
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

namespace Balau {

namespace Resource::Impl {

class ZipEntrySource;
class ZipEntrySink;

} // namespace Resource::Impl

namespace Util {

///
/// %GZip compression utilities.
///
struct GZip final {
	///
	/// Gzip the specified input file to the specified output file.
	///
	/// @param input the file to gzip
	/// @param output the file to write the gzipped stream to
	///
	static void gzip(const Resource::File & input, const Resource::File & output) {
		boost::filesystem::ofstream out(output.getEntry(), std::ios::binary);
		boost::iostreams::filtering_streambuf<boost::iostreams::output> filter;
		filter.push(boost::iostreams::gzip_compressor());
		filter.push(out);
		boost::filesystem::ifstream in(input.getEntry(), std::ios::binary);
		boost::iostreams::copy(in, filter);
	}

	///
	/// Gzip the supplied string to the specified output file.
	///
	/// @param input the string to gzip
	/// @param output the file to write the gzipped stream to
	///
	static void gzip(const std::string & input, const Resource::File & output) {
		boost::filesystem::ofstream out(output.getEntry(), std::ios::binary);
		boost::iostreams::filtering_streambuf<boost::iostreams::output> filter;
		filter.push(boost::iostreams::gzip_compressor());
		filter.push(out);
		std::istringstream in(input, std::ios::binary);
		boost::iostreams::copy(in, filter);
	}

	///
	/// Gzip the supplied input stream to the specified output file.
	///
	/// @param input the input stream to gzip
	/// @param output the file to write the gzipped stream to
	///
	static void gzip(std::istream & input, const Resource::File & output) {
		boost::filesystem::ofstream out(output.getEntry(), std::ios::binary);
		boost::iostreams::filtering_streambuf<boost::iostreams::output> filter;
		filter.push(boost::iostreams::gzip_compressor());
		filter.push(out);
		boost::iostreams::copy(input, filter);
	}

	///
	/// Gunzip the specified input file to the specified output file.
	///
	/// @param input the gzipped file
	/// @param output the file to write the uncompressed stream to
	///
	static void gunzip(const Resource::File & input, const Resource::File & output) {
		boost::filesystem::ifstream in(input.getEntry(), std::ios::binary);
		boost::iostreams::filtering_streambuf<boost::iostreams::input> filter;
		filter.push(boost::iostreams::gzip_decompressor());
		filter.push(in);
		boost::filesystem::ofstream out(output.getEntry(), std::ios::binary);
		boost::iostreams::copy(filter, out);
	}

	///
	/// Gunzip the specified file to a string.
	///
	/// Be sure that the data will fit in memory (e.g. check the file size before
	/// calling), or the application will cause a catastophic memory failure.
	///
	/// @param input the gzipped file
	/// @return a string containing the uncompressed data
	///
	static void gunzip(const Resource::File & input, std::string & output) {
		boost::filesystem::ifstream in(input.getEntry(), std::ios::binary);
		boost::iostreams::filtering_streambuf<boost::iostreams::input> filter;
		filter.push(boost::iostreams::gzip_decompressor());
		filter.push(in);
		std::ostringstream out(std::ios::binary);
		boost::iostreams::copy(filter, out);
		output = out.str();
	}

	///
	/// Get an input stream of the uncompressed contents of the specified gzipped input file.
	///
	/// @param input the gzipped file
	/// @return an input stream wrapper supplying a uncompressed stream
	///
	static void gunzip(const Resource::File & input, std::unique_ptr<std::istream> & stream) {
		boost::filesystem::ifstream inputStream(input.getEntry(), std::ios::binary);
		stream = std::make_unique<boost::iostreams::filtering_istream>();
		auto * filterStream = static_cast<boost::iostreams::filtering_istream *>(stream.get());
		filterStream->push(boost::iostreams::gzip_decompressor());
		filterStream->push(inputStream);
	}

	///////////////////////////////////////////////////////////////////////////

	GZip() = delete;
	GZip(const GZip &) = delete;
	GZip & operator = (const GZip &) = delete;
};

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
	public: virtual void open(const Resource::File & path_, bool verify);

	///
	/// Open the specified archive for reading.
	///
	/// @param path_ the path to the archive
	/// @param verify if true, the implementation will first verify the archive
	/// @param pw the archive's password if encryption was used when writing
	/// @throw InvalidArchiveException if verify is true and the archive is deemed to be invalid
	///
	public: virtual void open(const Resource::File & path_, bool verify, const std::string & pw);

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
	public: std::string readArchiveComment() const;

	///
	/// Get the number of entries in the original archive if one exists.
	///
	/// If the archive is being written to (via Zipper) and is new, this will
	/// return zero.
	///
	/// @return the number of entries in the original archive if one exists or zero
	/// @throw ZipException if the archive is not open
	///
	public: long long entryCount() const;

	///
	/// Get the entry names in the original archive if one exists.
	///
	/// If the archive is being written to (via Zipper) and is new, this will
	/// return an empty vector.
	///
	/// @return the entry names in the original archive if one exists or an empty vector
	/// @throw ZipException if the archive is not open
	///
	public: std::vector<std::string> entryNames() const;

	///
	/// Does the original archive have an entry with the specified name?
	///
	/// If the archive being written to is new, this will always return false.
	///
	/// @param name the name of the entry
	/// @return true if the original archive has an entry with the specified name
	/// @throw ZipException if the archive is not open
	///
	public: bool hasEntry(const std::string & name) const;

	///
	/// Get the index of the entry that corresponds to the supplied name.
	///
	/// @param name the name of the entry
	/// @return the index of the entry that corresponds to the supplied name
	/// @throw ZipException if there is no such entry
	/// @throw ZipException if the archive is not open
	///
	public: long long getEntryIndex(const std::string & name) const;

	///
	/// Get the name of the entry that corresponds to the supplied index.
	///
	/// @param index the index of the entry
	/// @return the name of the entry that corresponds to the supplied index
	/// @throw ZipException if the entry name could not be obtained
	/// @throw ZipException if the archive is not open
	///
	public: std::string getEntryName(long long index) const;

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
	public: void getEntryInfo(const std::string & name, ZipEntryInfo & info) const;

	///
	/// Get information on the entry with the specified index.
	///
	/// @param index the index of the entry
	/// @param info the zip entry info structure that will be filled with the info
	/// @throw ZipException if the entry info could not be obtained
	/// @throw ZipException if the archive is not open
	///
	public: void getEntryInfo(long long index, ZipEntryInfo & info) const;

	///
	/// Get the comment on the entry if one exists.
	///
	/// A valid entry name must first be obtained by calling getEntryPaths().
	///
	/// @return the comment on the entry if one exists or an empty string
	/// @throw ZipException if the entry comment could not be obtained
	/// @throw ZipException if the archive is not open
	///
	public: std::string readEntryComment(const std::string & name) const;

	///
	/// Get the comment on the entry if one exists.
	///
	/// @return the comment on the entry if one exists or an empty string
	/// @throw ZipException if the archive is not open
	///
	/// @throw ZipException if the entry comment could not be obtained
	///
	public: std::string readEntryComment(long long index) const;

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
	public: std::vector<char> readEntryAsBytes(const std::string & name) const;

	///
	/// Get the contents of the specified entry in the original archive if one exists.
	///
	/// @param index the index of the entry
	/// @return the contents of the specified entry as a char vector
	/// @throw ZipException if the entry could not be obtained
	/// @throw ZipException if the archive is not open
	///
	public: std::vector<char> readEntryAsBytes(long long index) const;

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
	public: std::string readEntryAsString(const std::string & name) const;

	///
	/// Get the contents of the specified entry in the original archive if one exists.
	///
	/// @param index the index of the entry
	/// @return the contents of the specified entry as a UTF-8 string
	/// @throw ZipException if the entry could not be obtained
	/// @throw ZipException if the archive is not open
	///
	public: std::string readEntryAsString(long long index) const;

	///
	/// Close the archive.
	///
	/// In order to continue using the instance, a subsequent call to open() is
	/// necessary.
	///
	/// If close is called without a corresponding previous call to open(),
	/// the result will be a NOP.
	///
	public: virtual void close();

	///
	/// Destroy the unzipper, closing the archive if it is open.
	///
	public: virtual ~Unzipper();

	///////////////////////// Private implementation //////////////////////////

	friend class Resource::Impl::ZipEntrySource;
	friend class Resource::Impl::ZipEntrySink;

	private: std::vector<char> readEntryAsBytesImpl(const ZipEntryInfo & info, const std::string & name) const;
	private: std::string readEntryAsStringImpl(const ZipEntryInfo & info, const std::string & name) const;

	// Used in the cleanUpName method.
	protected: enum class EntryType {
		Directory, File, Either
	};

	protected: void openImpl(const Resource::File & path_, bool verify, const std::string & pw, int mode);

	// Clean up the supplied name's separator characters.
	protected: std::string cleanUpName(const std::string & name, EntryType entryType) const;

	protected: long long getEntryIndexClean(const std::string & cleanName) const;
	protected: void throwZipException(const std::string & errorMessage) const;

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
	public: void open(const Resource::File & path_, bool verify) override;

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
	public: void open(const Resource::File & path_, bool verify, const std::string & pw) override;

	///
	/// Set the comment in the archive, overwriting any existing comment.
	///
	/// @param text the text content to set the comment to
	/// @throw ZipException if the archive comment could not be set
	/// @throw ZipException if the archive is not open
	///
	public: void putArchiveComment(const std::string & text);

	///
	/// Deletes the comment in the archive.
	///
	/// @throw ZipException if the archive comment could not be deleted
	/// @throw ZipException if the archive is not open
	///
	public: void deleteComment();

	///
	/// Ensure that the specified directory exists.
	///
	/// All parent directories will also be created if not present.
	///
	/// @param name the directory name
	/// @throw ZipException if a directory could not be created
	/// @throw ZipException if the archive is not open
	///
	public: void putDirectory(const std::string & name);

	///
	/// Create or overwrite an entry from the data in the supplied file.
	///
	/// @param name the name of the entry to create or overwrite
	/// @param bytes the file from which the data to create the entry with will be sourced
	/// @throw ZipException if the entry could not be created
	/// @throw ZipException if the archive is not open
	///
	public: void putEntry(const std::string & name, const Resource::File & bytes);

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
	public: void putEntry(const std::string & name, const std::vector<char> & bytes);

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
	public: void putEntry(const std::string & name, const std::string & bytes);

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
	public: void renameEntry(const std::string & name, const std::string & newName);

	///
	/// Delete an existing entry in the original archive.
	///
	/// If the entry is a directory, all files under that directory will also be deleted.
	///
	/// @param name the name of the existing entry in the original archive
	/// @throw ZipException if the entry could not be deleted
	/// @throw ZipException if the archive is not open
	///
	public: void deleteEntry(const std::string & name);

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
	public: void close() override;

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
	public: void commit();

	////////////////////////// Private implementation /////////////////////////

	// Common implementation for putEntry methods.
	private: template <typename SourceFunctionT> void putEntry(const std::string & name, SourceFunctionT getSource);

	// Rename a directory entry and all its descendants.
	private: void renameDirectory(const std::string & cleanName, const std::string & newCleanName);

	// Rename a file entry.
	private: void renameFile(const std::string & cleanName, const std::string & newCleanName);

	// Delete a directory entry and all its descendants.
	private: void deleteDirectory(const std::string & cleanName);

	// Delete a file entry.
	private: void deleteFile(const std::string & cleanName);
};

} // namespace Util

} // namespace Balau

inline std::string toString(const Balau::Util::ZipEntryInfo & info) {
	return Balau::Util::toString(info);
}

#endif // COM_BORA_SOFTWARE__BALAU_UTIL__COMPRESSION
