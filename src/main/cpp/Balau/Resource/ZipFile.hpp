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
/// @file ZipFile.hpp
///
/// A zip file on the local file system.
///

#ifndef COM_BORA_SOFTWARE__BALAU_RESOURCE__ZIP_FILE
#define COM_BORA_SOFTWARE__BALAU_RESOURCE__ZIP_FILE

#ifdef BALAU_LIBZIP_ENABLED

#include <Balau/Resource/File.hpp>
#include <Balau/Resource/ZipEntry.hpp>

namespace Balau::Resource {

///
/// A zip file on the local file system.
///
/// Zip file iterators iterate over the zip entries in the zip file.
/// From each zip entry, a read or write stream may be obtained to read from or
/// modify the entry data.
///
/// If random read access is required over the entries of a zip file , it is better
/// to use the Unzipper class directly instead of a ZipFile URI and iterator.
///
class ZipFile : public File {
	///
	/// Recursive iterator into a zip file.
	///
	public: class RecursiveZipFileIterator {
		///
		/// Returns true if there is another item available in the iterator.
		///
		public: bool hasNext() const {
			return nextEntryIndex < entryCount;
		}

		///
		/// Get the next item in the iterator.
		///
		public: ZipEntry next() {
			return ZipEntry(archive, nextEntryIndex++);
		}

		friend class ZipFile;

		private: explicit RecursiveZipFileIterator(ZipFile & zipFile, bool verify, const std::string & pw)
			: nextEntryIndex(0) {
			archive.open(zipFile, verify, pw);

			if (!archive.isOpen()) {
				ThrowBalauException(
					Exception::ZipException, "Could not open zip file for reading.", File(zipFile.entry)
				);
			}

			entryCount = (unsigned long long) archive.entryCount();
		}

		private: Util::Unzipper archive;
		private: unsigned long long nextEntryIndex;
		private: unsigned long long entryCount;
	};

	private: class RecursiveZipFileUriIterator : public RecursiveUriIterator {
		///
		/// Returns true if there is another item available in the iterator.
		///
		public: bool hasNext() const override {
			return nextEntryIndex < entryCount;
		}

		///
		/// Get the next item in the iterator.
		///
		public: std::unique_ptr<Uri> next() override {
			return std::unique_ptr<Uri>(new ZipEntry(archive, nextEntryIndex++));
		}

		friend class ZipFile;

		private: explicit RecursiveZipFileUriIterator(const ZipFile & zipFile, bool verify, const std::string & pw)
			: nextEntryIndex(0) {
			archive.open(zipFile, verify, pw);

			if (!archive.isOpen()) {
				ThrowBalauException(
					Exception::ZipException, "Could not open zip file for reading.", File(zipFile.entry)
				);
			}

			entryCount = (unsigned long long) archive.entryCount();
		}

		private: Util::Unzipper archive;
		private: unsigned long long nextEntryIndex;
		private: unsigned long long entryCount;
	};

	///
	/// Create a new zip file for the specified file.
	///
	/// The caller is responsible for a path that points to a zip file when the
	/// %ZipFile URI is subsequently used.
	///
	/// @param file_ the file URI pointing to the zip file
	/// @param password_ an optional encryption password for the zip file
	///
	public: explicit ZipFile(File file_, std::string password_ = "")
		: File(std::move(file_))
		, password(std::move(password_)) {}

	///
	/// Create a new zip file by copying the supplied instance.
	///
	public: ZipFile(const ZipFile & zipFile) = default;

	///
	/// Create a new zip file by moving the contents of the supplied instance.
	///
	public: ZipFile(ZipFile && zipFile) noexcept
		: File(std::move(zipFile)) {}

	///
	/// Assign the zip file by copying the supplied instance.
	///
	public: ZipFile & operator = (const ZipFile & zipFile) = default;

	///
	/// Assign the zip file by moving the contents of the supplied instance.
	///
	public: ZipFile & operator = (ZipFile && zipFile) = default;

	public: std::unique_ptr<Uri> clone() const override {
		return std::unique_ptr<Uri>(new ZipFile(*this));
	}

	public: std::unique_ptr<Uri> resolve(std::string_view path) const override {
		static const std::regex scheme { "[a-zA-Z][a-zA-Z0-9+-\\.]*:" };

		auto cleanPath = Util::Strings::trim(path);
		auto str = std::string(cleanPath);

		if (Util::Strings::startsWithRegex(str, scheme)) {
			std::unique_ptr<Uri> uri;
			fromString(uri, str);
			return uri;
		}

		boost::filesystem::path p(str);

		if (p.is_relative()) {
			auto p2 = boost::filesystem::relative(p, entry.path());
			return std::unique_ptr<Uri>(new ZipFile(File(p2)));
		} else { // absolute
			return std::unique_ptr<Uri>(new ZipFile(File(p)));
		}
	}

	public: bool canReadFrom() const override {
		return false;
	}

	public: bool canWriteTo() const override {
		return false;
	}

	public: std::unique_ptr<ByteReadResource> byteReadResource() const override {
		ThrowBalauException(Exception::NotImplementedException, "ZipFile URIs do not have a byte read resource.");
	}

	public: std::unique_ptr<Utf8To32ReadResource> utf8To32ReadResource() const override {
		ThrowBalauException(Exception::NotImplementedException, "ZipFile URIs do not have a Unicode read resource.");
	}

	public: std::unique_ptr<ByteWriteResource> byteWriteResource() override {
		ThrowBalauException(Exception::NotImplementedException, "ZipFile URIs do not have a byte write resource.");
	}

	public: std::unique_ptr<Utf32To8WriteResource> utf32To8WriteResource() override {
		ThrowBalauException(Exception::NotImplementedException, "ZipFile URIs do not have a Unicode write resource.");
	}

	public: bool isRecursivelyIterable() const override {
		return true;
	}

	public: std::unique_ptr<RecursiveUriIterator> recursiveIterator() const override {
		return std::unique_ptr<RecursiveUriIterator>(new RecursiveZipFileUriIterator(*this, false, ""));
	}

	///
	/// Get a recursive zip file iterator, specifying if the zip file should be
	/// verified and an optional encryption password.
	///
	/// @param verify if true, the archive should first be verified
	/// @param pw     the archive's password if encryption was used when writing
	///
	public: RecursiveZipFileIterator recursiveZipFileIterator(bool verify = false, const std::string & pw = "") {
		return RecursiveZipFileIterator(*this, verify, pw);
	}

	public: RecursiveFileIterator recursiveFileIterator() override {
		ThrowBalauException(Exception::NotImplementedException, "ZipFile URIs do not have recursive file iterators.");
	}

	public: bool operator == (const Uri & rhs) const override {
		const auto * o = dynamic_cast<const ZipFile *>(&rhs);
		return o == nullptr ? false : getEntry() == o->getEntry();
	}

	public: bool isRegularDirectory() const override {
		return false;
	}

	public: bool isRegularFile() const override {
		return true;
	}

	public: void dispatch(UriDispatcher & dispatcher) const override {
		dispatcher.dispatch(*this);
	}

	////////////////////////// Private implementation /////////////////////////

	private: std::string password;
};

///
/// Print the zip file URI as a UTF-8 string.
///
/// @return a UTF-8 string representing the zip file URI
///
template <typename AllocatorT>
inline Balau::U8String<AllocatorT> toString(const ZipFile & zipFile) {
	return zipFile.toRawString<AllocatorT>();
}

///
/// Print the zip file URI as a UTF-8 string.
///
/// @return a UTF-8 string representing the zip file URI
///
inline std::string toString(const ZipFile & zipFile) {
	return zipFile.toRawString();
}

} // namespace Balau::Resource

namespace std { // NOLINT

template <> struct hash<Balau::Resource::ZipFile> {
	size_t operator () (const Balau::Resource::ZipFile & zipFile) const noexcept {
		return zipFile.hashcode();
	}
};

template <> struct equal_to<Balau::Resource::ZipFile> {
	bool operator () (const Balau::Resource::ZipFile & lhs, const Balau::Resource::ZipFile & rhs) const {
		return lhs == rhs;
	}
};

} // namespace std

#endif // BALAU_LIBZIP_ENABLED

#endif // COM_BORA_SOFTWARE__BALAU_RESOURCE__ZIP_FILE
