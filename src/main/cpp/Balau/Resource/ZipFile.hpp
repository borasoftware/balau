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
/// @file ZipFile.hpp
///
/// A zip file on the local file system.
///

#ifndef COM_BORA_SOFTWARE__BALAU_RESOURCE__ZIP_FILE
#define COM_BORA_SOFTWARE__BALAU_RESOURCE__ZIP_FILE

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

		private: RecursiveZipFileIterator(ZipFile & zipFile, bool verify, const std::string & pw)
			: nextEntryIndex(0) {
			archive.open(zipFile, verify, pw);

			if (!archive.isOpen()) {
				ThrowBalauException(Exception::ZipException, "Could not open zip file for reading.", File(zipFile.entry));
			}

			entryCount = archive.entryCount();
		}

		private: Util::Unzipper archive;
		private: long long nextEntryIndex;
		private: long long entryCount;
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

		private: RecursiveZipFileUriIterator(const ZipFile & zipFile, bool verify, const std::string & pw)
			: nextEntryIndex(0) {
			archive.open(zipFile, verify, pw);

			if (!archive.isOpen()) {
				ThrowBalauException(Exception::ZipException, "Could not open zip file for reading.", File(zipFile.entry));
			}

			entryCount = archive.entryCount();
		}

		private: Util::Unzipper archive;
		private: long long nextEntryIndex;
		private: long long entryCount;
	};

	///
	/// Non-recursive iterator into a zip file.
	///
	/// TODO not yet implemented
	///
	public: class ZipFileIterator {
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

		private: explicit ZipFileIterator(ZipFile & zipFile, bool verify, const std::string & pw)
			: nextEntryIndex(0) {
			ThrowBalauException(Exception::NotImplementedException, "ZipFileIterator");

			archive.open(zipFile, verify, pw);

			if (!archive.isOpen()) {
				ThrowBalauException(Exception::ZipException, "Could not open zip file for reading.", File(zipFile.entry));
			}

			entryCount = archive.entryCount();
		}

		private: Util::Unzipper archive;
		private: long long nextEntryIndex;
		private: long long entryCount;
	};

	// TODO not yet implemented
	private: class ZipFileUriIterator : public UriIterator {
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

		private: ZipFileUriIterator(const ZipFile & zipFile, bool verify, const std::string & pw)
			: nextEntryIndex(0) {
			ThrowBalauException(Exception::NotImplementedException, "ZipFileUriIterator");

			archive.open(zipFile, verify, pw);

			if (!archive.isOpen()) {
				ThrowBalauException(Exception::ZipException, "Could not open zip file for reading.", File(zipFile.entry));
			}

			entryCount = archive.entryCount();
		}

		private: Util::Unzipper archive;
		private: long long nextEntryIndex;
		private: long long entryCount;
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

	public: std::unique_ptr<Uri> append(const std::string & pathComponent) const override {
		ThrowBalauException(Exception::UnsupportedOperationException, "ZipFile does not support path appending.");
	}

	public: void visit(UriVisitor & visitor) const override {
		visitor.visit(*this);
	}

	public: bool canReadFrom() const override {
		return false;
	}

	public: bool canWriteTo() const override {
		return false;
	}

	public: std::unique_ptr<ByteReadResource> byteReadResource() const override {
		ThrowBalauException(Exception::UnsupportedOperationException, "ZipFile URIs do not have a byte read resource.");
	}

	public: std::unique_ptr<Utf8To32ReadResource> utf8To32ReadResource() const override {
		ThrowBalauException(Exception::UnsupportedOperationException, "ZipFile URIs do not have a Unicode read resource.");
	}

	public: std::unique_ptr<ByteWriteResource> byteWriteResource() override {
		ThrowBalauException(Exception::UnsupportedOperationException, "ZipFile URIs do not have a byte write resource.");
	}

	public: std::unique_ptr<Utf32To8WriteResource> utf32To8WriteResource() override {
		ThrowBalauException(Exception::UnsupportedOperationException, "ZipFile URIs do not have a Unicode write resource.");
	}

	public: bool isRecursivelyIterable() const override {
		return true;
	}

	public: bool isIterable() const override {
		return true;
	}

	public: std::unique_ptr<RecursiveUriIterator> recursiveIterator() const override {
		return std::unique_ptr<RecursiveUriIterator>(new RecursiveZipFileUriIterator(*this, false, ""));
	}

	public: std::unique_ptr<UriIterator> iterator() const override {
		return std::unique_ptr<UriIterator>(new ZipFileUriIterator(*this, false, ""));
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

	///
	/// Get a non-recursive zip file iterator, specifying if the zip file should be
	/// verified and an optional encryption password.
	///
	/// @param verify if true, the archive should first be verified
	/// @param pw     the archive's password if encryption was used when writing
	///
	public: ZipFileIterator zipFileIterator(bool verify = false, const std::string & pw = "") {
		return ZipFileIterator(*this, verify, pw);
	}

	public: RecursiveFileIterator recursiveFileIterator() const override {
		ThrowBalauException(Exception::UnsupportedOperationException, "ZipFile URIs do not have recursive file iterators.");
	}

	public: FileIterator fileIterator() const override {
		ThrowBalauException(Exception::UnsupportedOperationException, "ZipFile URIs do not have file iterators.");
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

#endif // COM_BORA_SOFTWARE__BALAU_RESOURCE__ZIP_FILE
