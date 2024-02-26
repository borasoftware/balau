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
/// @file ZipEntry.hpp
///
/// An entry in a zip archive on the local file system.
///

#ifndef COM_BORA_SOFTWARE_BALAU_RESOURCE_ZIP_ENTRY
#define COM_BORA_SOFTWARE_BALAU_RESOURCE_ZIP_ENTRY

#include <Balau/Exception/ResourceExceptions.hpp>
#include <Balau/Resource/ByteReadResource.hpp>
#include <Balau/Resource/UriComponents.hpp>
#include <Balau/Resource/Utf8To32ReadResource.hpp>
#include <Balau/Util/Files.hpp>
#include <Balau/Util/Zip.hpp>

#include <boost/iostreams/categories.hpp>
#include <boost/iostreams/code_converter.hpp>
#include <boost/iostreams/concepts.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/device/file_descriptor.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/stream.hpp>

#include <codecvt>

namespace Balau::Resource {

class UriResolve;
class ZipEntryByteReadResource;
class ZipEntryUtf8To32ReadResource;

///
/// An entry in a zip archive on the local file system.
///
/// ZipEntry instances can only be obtained by iteration over a ZipFile.
///
/// If random read access is required over the entries of a zip file , it is better
/// to use the Unzipper class directly instead of a ZipFile URI and iterator.
///
class ZipEntry : public Uri {
	public: std::unique_ptr<Uri> clone() const override {
		return std::unique_ptr<Uri>(new ZipEntry(*this));
	}

	public: std::unique_ptr<Uri> append(const std::string & pathComponent) const override {
		ThrowBalauException(Exception::UnsupportedOperationException, "ZipEntry does not support path appending.");
	}

	public: void visit(UriVisitor & visitor) const override {
		visitor.visit(*this);
	}

	public: std::string toUriString() const override {
		Util::ZipEntryInfo info {};
		archive.getEntryInfo(entryIndex, info);
		return archive.getPath().toUriString() + "#" + ::toString(info.name);
	}

	public: std::string toRawString() const override {
		return toUriString();
	}

	///
	/// Return true if the entry is an archive directory.
	///
	/// @return true if the entry is an archive directory
	///
	public: bool isEntryDirectory() const {
		Util::ZipEntryInfo info {};
		archive.getEntryInfo(entryIndex, info);
		return Util::Strings::endsWith(info.name, "/");
	}

	///
	/// Return true if the entry is an archive file.
	///
	/// @return true if the entry is an archive file
	///
	public: bool isEntryFile() const {
		return !isEntryDirectory();
	}

	///
	/// Get the name of the entry.
	///
	/// @return the name of the entry that corresponds to the supplied index
	///
	public: std::string name() const {
		return archive.getEntryName(entryIndex);
	}

	///
	/// Get the uncompressed size of the entry in bytes.
	///
	/// @return the uncompressed size of the entry in bytes
	///
	public: size_t size() const {
		Util::ZipEntryInfo info {};
		archive.getEntryInfo(entryIndex, info);
		return info.uncompressedSize;
	}

	///
	/// Get the comment on the entry if one exists.
	///
	/// @return the comment on the entry if one exists or an empty string
	/// @throw ZipException if the entry comment could not be obtained
	///
	public: std::string readEntryComment() const {
		return archive.readEntryComment(entryIndex);
	}

	///
	/// Get the contents of the entry as a char vector.
	///
	/// @param name the name of the entry
	/// @return the contents of the specified entry as a char vector
	///
	public: std::vector<char> readEntryAsBytes() const {
		return archive.readEntryAsBytes(entryIndex);
	}

	///
	/// Get the contents of the entry as a UTF-8 string.
	///
	/// @return the contents of the entry as a UTF-8 string
	///
	public: std::string readEntryAsString() const {
		return archive.readEntryAsString(entryIndex);
	}

	public: size_t hashcode() const noexcept override {
		Util::ZipEntryInfo info {};
		archive.getEntryInfo(entryIndex, info);
		return archive.getPath().hashcode() ^ std::hash<std::string>()(::toString(info.name));
	}

	public: bool operator == (const Uri & rhs) const override {
		const auto * o = dynamic_cast<const ZipEntry *>(&rhs);
		return o == nullptr ? false : (archive.getPath() == o->archive.getPath() && entryIndex == o->entryIndex);
	}

	public: bool canReadFrom() const override {
		return true;
	}

	public: bool canWriteTo() const override {
		return false;
	}

	///
	/// Get a byte read resource for this zip entry.
	///
	/// @throw ZipException if the resource could not be created
	///
	public: ZipEntryByteReadResource getByteReadResource() const;

	///
	/// Get a byte read resource for this zip entry.
	///
	/// @throw ZipException if the resource could not be created
	///
	public: ZipEntryUtf8To32ReadResource getUtf8To32ReadResource() const;

	public: std::unique_ptr<ByteReadResource> byteReadResource() const override;

	public: std::unique_ptr<Utf8To32ReadResource> utf8To32ReadResource() const override;

	public: std::unique_ptr<ByteWriteResource> byteWriteResource() override {
		ThrowBalauException(Exception::NotImplementedException, "ZipEntry URIs do not have a byte write resource.");
	}

	public: std::unique_ptr<Utf32To8WriteResource> utf32To8WriteResource() override {
		ThrowBalauException(Exception::NotImplementedException, "ZipEntry URIs do not have a Unicode write resource.");
	}

	public: bool isRecursivelyIterable() const override {
		return false;
	}

	public: bool isIterable() const override {
		return false;
	}

	public: std::unique_ptr<RecursiveUriIterator> recursiveIterator() const override {
		ThrowBalauException(Exception::NotImplementedException, "Zip entry URIs do not have recursive iterators.");
	}

	public: std::unique_ptr<UriIterator> iterator() const override {
		ThrowBalauException(Exception::NotImplementedException, "Zip entry URIs do not have iterators.");
	}

	public: bool isRegularDirectory() const override {
		return false;
	}

	public: bool isRegularFile() const override {
		return false;
	}

	public: void dispatch(UriDispatcher & dispatcher) const override {
		dispatcher.dispatch(*this);
	}

	////////////////////////// Private implementation /////////////////////////

	friend class ZipFile;
	friend class ZipEntryByteReadResource;
	friend class ZipEntryUtf8To32ReadResource;
	friend class UriResolve;

	private: ZipEntry(Util::Unzipper & archive_, long long entryIndex_)
		: archive(archive_)
		, entryIndex(entryIndex_) {}

	private: Util::Unzipper & archive;
	private: long long entryIndex;
};

namespace Impl {

///
/// Boost IO streams zip entry source, used in the zip entry input stream.
///
class ZipEntrySource : public boost::iostreams::source {
	private: Util::Unzipper & archive;
	private: unsigned long long index;
	private: std::shared_ptr<void> zipFile;

	public: ZipEntrySource(Util::Unzipper & archive_, unsigned long long index_)
		: archive(archive_)
		, index(index_)
		, zipFile(createZipFilePointer(archive)) {}

	public: ZipEntrySource(const ZipEntrySource & copy) = default;

	public: std::streamsize read(char * s, std::streamsize n) {
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

	////////////////////////// Private implementation /////////////////////////

	private: std::shared_ptr<void> createZipFilePointer(Util::Unzipper & archive) {
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

	private: zip_file_t * cast(std::shared_ptr<void> & zipFileHandle) {
		return (zip_file_t *) zipFileHandle.get();
	}
};

//
// Boost IO streams zip entry sink, used in the zip entry output stream.
//
// todo implement this class
//
//class ZipEntrySink : public boost::iostreams::sink {
//	private: Util::Zipper & archive;
//	private: unsigned long long index;
//	private: void * zipFile;
//
//	public: ZipEntrySink(Util::Zipper & archive_, unsigned long long index_);
//	: archive(archive_)
//	, index(index_)
//	, zipFile(zip_fopen_index((zip_t *) archive.archive, index, 0)) {
//
//	if (zipFile == nullptr) {
//		ThrowBalauException(Exception::ZipException, "Could not open zip file.", archive.path);
//	}
//}
//
//	public: ~ZipEntrySink() {
//	if (zipFile != nullptr) {
//		zip_fclose(cast(zipFile));
//	}
//}
//
//	public: std::streamsize write(char * s, std::streamsize n) {
////	// TODO
////}
//};

} // namespace Impl

///
/// A read-only entry in a zip file which is read as bytes.
///
/// Zip entry byte read resources are created by calling ZipEntry::getByteReadResource.
///
class ZipEntryByteReadResource : public ByteReadResource {
	public: ZipEntryByteReadResource(ZipEntryByteReadResource && rhs) noexcept
		: entry(rhs.entry)
		, stream(std::move(rhs.stream)) {}

	public: ~ZipEntryByteReadResource() override = default;

	public: void close() override {
		// NOP
	}

	public: const Uri & uri() const override {
		return entry;
	}

	public: std::istream & readStream() override {
		return *stream;
	}

	////////////////////////// Private implementation /////////////////////////

	friend class ZipEntry;

	private: explicit ZipEntryByteReadResource(const ZipEntry & entry_)
		: entry(entry_)
		, stream(new boost::iostreams::stream<Impl::ZipEntrySource>(Impl::ZipEntrySource(entry.archive, entry.entryIndex))) {}

	private: const ZipEntry & entry;
	private: std::unique_ptr<boost::iostreams::stream<Impl::ZipEntrySource>> stream;
};

///
/// A read-only entry in a zip file which is read as UTF-8 characters and implicitly converted to UTF-32 characters.
///
/// Zip entry UTF-8 to UTF-32 read resources are created by calling ZipEntry::ZipEntryUtf8To32ReadResource.
///
class ZipEntryUtf8To32ReadResource : public Utf8To32ReadResource {
	private: using idevice_utf8_utf32 = boost::iostreams::code_converter<std::istream, std::codecvt_utf8<char32_t, 0x10ffff, std::consume_header>>;
	private: using istream_utf8_utf32 = boost::iostreams::stream<idevice_utf8_utf32>;

	public: ~ZipEntryUtf8To32ReadResource() override = default;

	public: ZipEntryUtf8To32ReadResource(ZipEntryUtf8To32ReadResource && rhs) noexcept
		: entry(rhs.entry)
		, utf8Stream(std::move(rhs.utf8Stream))
		, ref(rhs.ref)
		, utf32Stream(std::move(rhs.utf32Stream)) {}

	public: void close() override {
		// NOP
	}

	public: const Uri & uri() const override {
		return entry;
	}

	public: std::u32istream & readStream() override {
		return *utf32Stream;
	}

	///
	/// Create a zip entry UTF-8 to UTF-32 read resource for the supplied zip entry URI.
	///
	public: explicit ZipEntryUtf8To32ReadResource(const ZipEntry & entry_)
		: entry(entry_)
		, utf8Stream(new boost::iostreams::stream<Impl::ZipEntrySource>(entry.archive, entry.entryIndex))
		, ref(*utf8Stream)
		, utf32Stream(new istream_utf8_utf32(ref)) {}

	////////////////////////// Private implementation /////////////////////////

	friend class ZipEntry;

	private: const ZipEntry & entry;
	private: std::unique_ptr<boost::iostreams::stream<Impl::ZipEntrySource>> utf8Stream;
	private: std::istream & ref;
	private: std::unique_ptr<istream_utf8_utf32> utf32Stream;
};

inline ZipEntryByteReadResource ZipEntry::getByteReadResource() const {
	return ZipEntryByteReadResource(*this);
}

inline ZipEntryUtf8To32ReadResource ZipEntry::getUtf8To32ReadResource() const {
	return ZipEntryUtf8To32ReadResource(*this);
}

inline std::unique_ptr<ByteReadResource> ZipEntry::byteReadResource() const {
	return std::unique_ptr<ByteReadResource>(new ZipEntryByteReadResource(*this));
}

inline std::unique_ptr<Utf8To32ReadResource> ZipEntry::utf8To32ReadResource() const {
	return std::unique_ptr<Utf8To32ReadResource>(new ZipEntryUtf8To32ReadResource(*this));
}

} // namespace Balau::Resource

#endif // COM_BORA_SOFTWARE_BALAU_RESOURCE_ZIP_ENTRY
