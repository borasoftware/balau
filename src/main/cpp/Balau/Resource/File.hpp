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
/// @file File.hpp
///
/// A file on the local file system.
///

#ifndef COM_BORA_SOFTWARE__BALAU_RESOURCE__FILE
#define COM_BORA_SOFTWARE__BALAU_RESOURCE__FILE

#include <Balau/Exception/IOExceptions.hpp>
#include <Balau/Exception/ResourceExceptions.hpp>
#include <Balau/Resource/ByteReadResource.hpp>
#include <Balau/Resource/ByteWriteResource.hpp>
#include <Balau/Resource/Utf8To32ReadResource.hpp>
#include <Balau/Resource/Utf32To8WriteResource.hpp>
#include <Balau/Util/Strings.hpp>

#include <boost/filesystem.hpp>
#include <chrono>

namespace Balau::Resource {

class FileByteReadResource;
class FileUtf8To32ReadResource;
class FileByteWriteResource;
class FileUtf32To8WriteResource;
class UriResolver;

///
/// A file on the local file system.
///
class File : public Uri {
	///
	/// Recursive iteration into a directory structure.
	///
	public: class RecursiveFileIterator {
		///
		/// Returns true if there is another item available in the iterator.
		///
		public: bool hasNext() const {
			return iterator != end;
		}

		///
		/// Get the next item in the iterator.
		///
		public: File next() {
			auto file = File(*iterator);
			++iterator;
			return file;
		}

		friend class File;

		private: explicit RecursiveFileIterator(const File & file)
			: iterator(boost::filesystem::recursive_directory_iterator(file.entry)) {}

		private: boost::filesystem::recursive_directory_iterator iterator;
		private: boost::filesystem::recursive_directory_iterator end;
	};

	///
	/// Recursive iteration into a directory structure (URI version).
	///
	private: class RecursiveFileUriIterator : public RecursiveUriIterator {
		///
		/// Returns true if there is another item available in the iterator.
		///
		public: bool hasNext() const override {
			return iterator != end;
		}

		///
		/// Get the next item in the iterator.
		///
		public: std::unique_ptr<Uri> next() override {
			boost::filesystem::directory_entry e = *iterator;
			++iterator;
			return std::unique_ptr<Uri>(new File(e));
		}

		friend class File;

		private: explicit RecursiveFileUriIterator(const File & file)
			: iterator(boost::filesystem::recursive_directory_iterator(file.entry)) {}

		private: boost::filesystem::recursive_directory_iterator iterator;
		private: boost::filesystem::recursive_directory_iterator end;
	};

	///
	/// Non-recursive iteration over a directory's contents.
	///
	public: class FileIterator {
		///
		/// Returns true if there is another item available in the iterator.
		///
		public: bool hasNext() const {
			return iterator != end;
		}

		///
		/// Get the next item in the iterator.
		///
		public: File next() {
			auto file = File(*iterator);
			++iterator;
			return file;
		}

		friend class File;

		private: explicit FileIterator(const File & file)
			: iterator(boost::filesystem::directory_iterator(file.entry)) {}

		private: boost::filesystem::directory_iterator iterator;
		private: boost::filesystem::directory_iterator end;
	};

	///
	/// Non-recursive iteration over a directory's contents (URI version).
	///
	private: class FileUriIterator : public UriIterator {
		///
		/// Returns true if there is another item available in the iterator.
		///
		public: bool hasNext() const override {
			return iterator != end;
		}

		///
		/// Get the next item in the iterator.
		///
		public: std::unique_ptr<Uri> next() override {
			boost::filesystem::directory_entry e = *iterator;
			++iterator;
			return std::unique_ptr<Uri>(new File(e));
		}

		friend class File;

		private: explicit FileUriIterator(const File & file)
			: iterator(boost::filesystem::directory_iterator(file.entry)) {}

		private: boost::filesystem::directory_iterator iterator;
		private: boost::filesystem::directory_iterator end;
	};

	///
	/// The path separator character for the platform.
	///
	public: static const boost::filesystem::path::value_type separator = boost::filesystem::path::preferred_separator;

	///
	/// Create an empty file URI.
	///
	public: File() = default;

	///
	/// Create a file from the supplied directory entry.
	///
	public: explicit File(boost::filesystem::directory_entry entry_) : entry(std::move(entry_)) {}

	///
	/// Create a file from the supplied Boost path.
	///
	public: explicit File(const boost::filesystem::path & entry_) : entry(entry_) {}

	///
	/// Create a file path with the supplied path string.
	///
	/// The path must be formatted correctly for the platform.
	///
	public: explicit File(const std::string & path) : entry(boost::filesystem::directory_entry(path)) {}

	///
	/// Create a file path with the supplied path string.
	///
	/// The path must be formatted correctly for the platform.
	///
	public: explicit File(const char * path) : entry(boost::filesystem::directory_entry(path)) {}

	///
	/// Create a file path with the supplied path string.
	///
	/// The path must be formatted correctly for the platform.
	///
	public: explicit File(std::string_view path) : entry(boost::filesystem::directory_entry(std::string(path))) {}

	///
	/// Create a file path with the supplied path string and filename string.
	///
	/// The path must be formatted correctly for the platform.
	///
	public: File(const std::string & path, const std::string & name) : entry(create(path, name)) {}

	///
	/// Create a file path with the supplied path and filename.
	///
	/// The path must be formatted correctly for the platform.
	///
	public: File(const File & path, const std::string & name) : entry(append(path.entry, name)) {}

	///
	/// Move an existing file URI into a new File URI.
	///
	public: File(File && file) noexcept : entry(std::move(file.entry)) {}

	///
	/// Copy an existing file URI into a new File URI.
	///
	public: File(const File & file) : entry(file.entry) {}

	public: std::unique_ptr<Uri> clone() const override {
		return std::unique_ptr<Uri>(new File(*this));
	}

	public: std::unique_ptr<Uri> append(const std::string & pathComponent) const override {
		return std::unique_ptr<Uri>(new File(*this / pathComponent));
	}

	public: void visit(UriVisitor & visitor) const override {
		visitor.visit(*this);
	}

	public: bool canReadFrom() const override {
		return true;
	}

	public: bool canWriteTo() const override {
		return true;
	}

	public: std::unique_ptr<ByteReadResource> byteReadResource() const override;

	public: std::unique_ptr<Utf8To32ReadResource> utf8To32ReadResource() const override;

	public: std::unique_ptr<ByteWriteResource> byteWriteResource() override;

	public: std::unique_ptr<Utf32To8WriteResource> utf32To8WriteResource() override;

	///
	/// Get a file byte read resource for this file URI.
	///
	/// @return a file byte read resource
	///
	public: FileByteReadResource getByteReadResource() const;

	///
	/// Get a file UTF-8 to UTF-32 read resource for this file URI.
	///
	/// @return a file UTF-8 to UTF-32 read resource
	///
	public: FileUtf8To32ReadResource getUtf8To32ReadResource() const;

	///
	/// Get a file byte write resource for this file URI.
	///
	/// @return a file byte write resource
	///
	public: FileByteWriteResource getByteWriteResource() const;

	///
	/// Get a file UTF-8 to UTF-32 write resource for this file URI.
	///
	/// @return a file UTF-8 to UTF-32 write resource
	///
	public: FileUtf32To8WriteResource getUtf32To8WriteResource() const;

	public: bool isRecursivelyIterable() const override {
		return isRegularDirectory();
	}

	public: bool isIterable() const override {
		return isRegularDirectory();
	}

	public: std::unique_ptr<RecursiveUriIterator> recursiveIterator() const override {
		if (!isRegularDirectory()) {
			ThrowBalauException(Exception::NotImplementedException, "Plain files do not have a recursive iterator.");
		}

		return std::unique_ptr<RecursiveUriIterator>(new RecursiveFileUriIterator(*this));
	}

	public: std::unique_ptr<UriIterator> iterator() const override {
		if (!isRegularDirectory()) {
			ThrowBalauException(Exception::NotImplementedException, "Plain files do not have an iterator.");
		}

		return std::unique_ptr<UriIterator>(new FileUriIterator(*this));
	}

	///
	/// Get a recursive file iterator for this file (directory).
	///
	/// @return a recursive file iterator
	/// @throw boost::filesystem::filesystem_error if the current object is not a directory
	///
	public: virtual RecursiveFileIterator recursiveFileIterator() const {
		return RecursiveFileIterator(*this);
	}

	///
	/// Get a non-recursive file iterator for this file (directory).
	///
	/// @return a file iterator
	/// @throw boost::filesystem::filesystem_error if the current object is not a directory
	///
	public: virtual FileIterator fileIterator() const {
		return FileIterator(*this);
	}

	///
	/// Get the underlying directory entry for this file URI.
	///
	/// @return a directory entry
	///
	public: boost::filesystem::directory_entry getEntry() const {
		return entry;
	}

	///
	/// Copy assign the supplied file to this file.
	///
	/// @return the current object
	///
	public: File & operator = (const File & file) {
		entry = file.entry;
		return *this;
	}

	///
	/// Move assign the supplied file to this file.
	///
	/// @return the current object
	///
	public: File & operator = (File && file) noexcept {
		entry = std::move(file.entry);
		return *this;
	}

	///
	/// Copy the supplied directory entry to this file.
	///
	/// @return the current object
	///
	public: File & operator = (const boost::filesystem::directory_entry & entry_) {
		entry = entry_;
		return *this;
	}

	///
	/// Copy the supplied file system path to this file.
	///
	/// @return the current object
	///
	public: File & operator = (const boost::filesystem::path & path) {
		entry = boost::filesystem::directory_entry(path);
		return *this;
	}

	///
	/// Returns true if an item exits in the file system for the file URI.
	///
	/// @return true if an item exits in the file system for the file URI
	///
	public: bool exists() const {
		return boost::filesystem::exists(entry);
	}

	///
	/// Get the size of the file if it exists or -1 if the file does not exist.
	///
	/// @throw IOException if there was an error when getting the file size
	///
	public: size_t size() const {
		boost::system::error_code errorCode;
		boost::uintmax_t sz = boost::filesystem::file_size(entry, errorCode);

		if (errorCode) {
			ThrowBalauException(
				  Exception::IOException
				, std::string("An error occurred when trying to get the file size: ") + errorCode.message()
			);
		}

		return sz;
	}

	///
	/// Get the modified time of the file.
	///
	/// @return the modified time of the file
	///
	public: std::chrono::system_clock::time_point getModifiedTimestamp() const {
		return std::chrono::system_clock::from_time_t(boost::filesystem::last_write_time(entry));
	}

	///
	/// Returns true if the file URI points to a directory.
	///
	/// @return true if the file URI points to a directory
	///
	public: bool isRegularDirectory() const override {
		return boost::filesystem::is_directory(entry);
	}

	///
	/// Returns true if the file URI points to a regular file.
	///
	/// @return true if the file URI points to a regular file
	///
	public: bool isRegularFile() const override {
		return boost::filesystem::is_regular_file(entry);
	}

	///
	/// Get the parent directory of the file URI.
	///
	/// @return the parent directory of the file URI
	///
	public: File getParentDirectory() const {
		return File(boost::filesystem::directory_entry(entry.path().parent_path()));
	}

	///
	/// Create a new file representing the specified sub-directory of the file URI.
	///
	/// @return a new file representing the specified sub-directory of the file URI
	///
	public: File getSubDirectory(std::string subDirectory) const {
		return File(
			boost::filesystem::directory_entry(boost::filesystem::path(entry.path()) /= subDirectory)
		);
	}

	///
	/// @todo document this
	///
	public: File getChildEntry(std::string child) const {
		boost::filesystem::path p = entry.path();
		p /= child;
		return File(boost::filesystem::directory_entry(p));
	}

	///
	/// Create all intermediate directories and the final directory.
	///
	/// A successful call to this method with false returned indicates
	/// that the directories existed already.
	///
	/// @return true if one or more of the directories were created, false otherwise
	/// @throw filesystem_error when there was an error attempting to create one or more of the directories
	///
	public: bool createDirectories() const {
		return boost::filesystem::create_directories(entry);
	}

	///
	/// Remove the file if it exists.
	///
	/// @return true if the file was removed
	///
	public: bool removeFile() const {
		return boost::filesystem::remove(entry);
	}

	///
	/// Convert the file to an absolute path if it is relative, using the current working directory.
	///
	/// @return a copy of the file converted to an absolute path
	///
	public: File toAbsolutePath() const {
		return File(boost::filesystem::absolute(entry));
	}

	///
	/// Get the relative path of the current object, compared to the supplied file.
	///
	/// @return the relative path of the current object, compared to the supplied file
	///
	public: File relative(const File & base) const {
		return File(boost::filesystem::relative(entry, base.entry));
	}

	public: std::string toUriString() const override {
		return std::string("file://") + Util::Strings::replaceAll(entry.path().string(), "\\", "/");
	}

	public: std::string toRawString() const override {
		return entry.path().string();
	}

	public: template <typename AllocatorT> Balau::U8String<AllocatorT> toRawString() const {
		return ::toString<AllocatorT>(entry.path().string());
	}

	public: size_t hashcode() const noexcept override {
		return std::hash<std::string>()(entry.path().string());
	}

	///
	/// Compare the supplied file to the current file.
	///
	/// @return true if the supplied file is equal to the current file.
	///
	public: bool operator == (const File & rhs) const {
		return entry == rhs.entry;
	}

	public: bool operator == (const Uri & rhs) const override {
		const auto * o = dynamic_cast<const File *>(&rhs);
		return o == nullptr ? false : entry == o->entry;
	}

	///
	/// Return true if the current object is less than the supplied instance.
	///
	/// @return true if the current object is less than the supplied instance
	///
	public: bool operator < (const File & rhs) const {
		return entry < rhs.entry;
	}

	///
	/// Append a path component to the path represented by the file.
	///
	/// @return a new file instance with the appended path component
	///
	public: File operator / (const char * component) const {
		return File(append(entry, component));
	}

	///
	/// Append a path component to the path represented by the file.
	///
	/// @return a new file instance with the appended path component
	///
	public: File operator / (const std::string & component) const {
		return File(append(entry, component));
	}

	///
	/// Append a path component to the path represented by the file.
	///
	/// @return a new file instance with the appended path component
	///
	public: File operator / (const std::string_view & component) const {
		return File(append(entry, std::string(component)));
	}

	///
	/// Append a sequence of path components to the path represented by the file.
	///
	/// @tparam T the string type in the container
	/// @tparam Container the container type
	/// @param container the container containing the components to append
	/// @return a new file instance with the appended path components
	///
	public: template <typename ... T, template <typename ...> class Container>
	File operator / (const Container<T ...> & container) const {
		using ::toString;

		boost::filesystem::path p = entry.path();

		for (const auto & component : container) {
			p /= toString(component);
		}

		return File(boost::filesystem::directory_entry(p));
	}

	///
	/// Append a path component to the path represented by the file in place.
	///
	/// @return the current object
	///
	public: File & operator /= (const char * component) {
		entry = append(entry, component);
		return *this;
	}

	///
	/// Append a path component to the path represented by the file in place.
	///
	/// @return the current object
	///
	public: File & operator /= (const std::string & component) {
		entry = append(entry, component);
		return *this;
	}

	///
	/// Append a path component to the path represented by the file in place.
	///
	/// @return the current object
	///
	public: File & operator /= (const std::string_view & component) {
		entry = append(entry, std::string(component));
		return *this;
	}

	///
	/// Concatenate the supplied path fragment to the end of the path represented by the file.
	///
	/// @return a new file instance with the concatenated fragment
	///
	public: File operator + (const std::string & fragment) const {
		boost::filesystem::path p = entry.path();
		p += fragment;
		return File(boost::filesystem::directory_entry(p));
	}

	public: void dispatch(UriDispatcher & dispatcher) const override {
		dispatcher.dispatch(*this);
	}

	///////////////////////// Private implementation //////////////////////////

	protected: boost::filesystem::directory_entry entry;

	friend class UriResolve;

	private: static boost::filesystem::directory_entry create(const std::string & path,
	                                                          const std::string & name) {
		boost::filesystem::directory_entry ret(path);
		return append(ret, name);
	}

	private: static boost::filesystem::directory_entry append(const boost::filesystem::directory_entry & de,
	                                                          const std::string & name) {
		boost::filesystem::path p = de.path();
		p /= name;
		return boost::filesystem::directory_entry(p);
	}
};

///
/// Print the file URI as a UTF-8 string.
///
/// @return a UTF-8 string representing the file URI
///
template <typename AllocatorT>
inline Balau::U8String<AllocatorT> toString(const File & file) {
	return file.toRawString<AllocatorT>();
}

///
/// Overwrite the supplied file URI by assignment by converting the supplied UTF-8 string to a file URI.
///
/// @return a file URI
///
inline void fromString(File & destination, std::string_view value) {
	destination = File(value);
}

} // namespace Balau::Resource

namespace Balau::Exception {

///
/// Thrown when a resource is not found.
///
class FileNotFoundException : public Exception::NotFoundException {
	public: FileNotFoundException(SourceCodeLocation location, const std::string & st, const Resource::File & file_)
		: Exception::NotFoundException(file_.clone(), location, st, "FileNotFound", false) {}

	public: FileNotFoundException(const std::string & st, const Resource::File & file_)
		: Exception::NotFoundException(file_.clone(), st, "FileNotFound", false) {}
};

inline bool operator == (const FileNotFoundException & lhs, const FileNotFoundException & rhs) {
	return lhs.message == rhs.message && *lhs.uri == *rhs.uri;
}

} // namespace Balau::Exception

namespace Balau::Resource {

///
/// A read-only standard file on a file system which is read as bytes.
///
class FileByteReadResource : public ByteReadResource {
	///
	/// Create a new file byte read resource from the supplied file URI.
	///
	/// @throw NotFoundException if the file does not exist
	///
	public: explicit FileByteReadResource(const File & file_)
		: file(std::make_unique<File>(file_))
		, stream(new boost::filesystem::ifstream(file_.getEntry())) {

		if (!stream->is_open()) {
			ThrowBalauException(Exception::FileNotFoundException, file_);
		}
	}

	public: FileByteReadResource(FileByteReadResource && rhs) noexcept
		: file(std::move(rhs.file))
		, stream(std::move(rhs.stream)) {}

	public: ~FileByteReadResource() override {
		close();
	}

	public: std::istream & readStream() override {
		return *stream;
	}

	public: const Uri & uri() const override {
		return *file;
	}

	///
	/// Get the file URI.
	///
	/// @return the file URI
	///
	public: const File & getFile() const {
		return *file;
	}

	public: void close() override {
		stream->close();
	}

	////////////////////////// Private implementation /////////////////////////

	private: std::unique_ptr<File> file;
	private: std::unique_ptr<boost::filesystem::ifstream> stream;
};

///
/// A write only standard file on a file system which is written as bytes.
///
class FileByteWriteResource : public ByteWriteResource {
	///
	/// Create a new file byte write resource from the supplied file URI.
	///
	public: explicit FileByteWriteResource(const File & file_)
		: file(std::make_unique<File>(file_))
		, stream(new boost::filesystem::ofstream(file_.getEntry())) {

		if (!stream->is_open()) {
			ThrowBalauException(Exception::FileNotFoundException, file_);
		}
	}

	public: FileByteWriteResource(FileByteWriteResource && rhs) noexcept
		: file(std::move(rhs.file))
		, stream(std::move(rhs.stream)) {}

	public: ~FileByteWriteResource() override {
		close();
	}

	public: std::ostream & writeStream() override {
		return *stream;
	}

	public: const Uri & uri() const override {
		return *file;
	}

	///
	/// Get the file URI.
	///
	/// @return the file URI
	///
	public: const File & getFile() const {
		return *file;
	}

	public: void close() override {
		stream->close();
	}

	////////////////////////// Private implementation /////////////////////////

	private: std::unique_ptr<File> file;
	private: std::unique_ptr<boost::filesystem::ofstream> stream;
};

///
/// A read-only file based UTF-8 resource which is read as UTF-32 characters.
///
class FileUtf8To32ReadResource : public Utf8To32ReadResource {
	///
	/// Create a new file UTF-8 to UTF-32 read resource from the supplied file URI.
	///
	/// @throw NotFoundException if the file does not exist
	///
	public: explicit FileUtf8To32ReadResource(const File & file_)
		: file(std::make_unique<File>(file_))
		, stream(new std::u32ifstream(file_.getEntry().path().native())) {
		// Overload 7 is typically called with its second argument, f, obtained
		// directly from a new-expression: the locale is responsible for calling
		// the matching delete from its own destructor.
		stream->imbue(std::locale(std::locale(), new std::codecvt<char32_t, char, std::mbstate_t>));

		if (!stream->is_open()) {
			ThrowBalauException(Exception::FileNotFoundException, file_);
		}
	}

	public: FileUtf8To32ReadResource(FileUtf8To32ReadResource && rhs) noexcept
		: file(std::move(rhs.file))
		, stream(std::move(rhs.stream)) {}

	public: ~FileUtf8To32ReadResource() override {
		close();
	}

	public: std::u32istream & readStream() override {
		return *stream;
	}

	public: const Uri & uri() const override {
		return *file;
	}

	///
	/// Get the file URI.
	///
	/// @return the file URI
	///
	public: const File & getFile() const {
		return *file;
	}

	public: void close() override {
		stream->close();
	}

	////////////////////////// Private implementation /////////////////////////

	private: std::unique_ptr<File> file;
	private: std::unique_ptr<std::u32ifstream> stream;
};

///
/// A write-only UTF-8 resource in a standard file on a file system, which is written with UTF-32 characters.
///
class FileUtf32To8WriteResource : public Utf32To8WriteResource {
	///
	/// Create a new file UTF-32 to UTF-8 write resource from the supplied file URI.
	///
	/// @throw NotFoundException if the file does not exist
	///
	public: explicit FileUtf32To8WriteResource(const File & file_)
		: file(std::make_unique<File>(file_))
		, stream(new std::u32ofstream(file_.getEntry().path().native())) {
		// Overload 7 is typically called with its second argument, f, obtained
		// directly from a new-expression: the locale is responsible for calling
		// the matching delete from its own destructor.
		stream->imbue(std::locale(std::locale(), new std::codecvt<char32_t, char, std::mbstate_t>));

		if (!stream->is_open()) {
			ThrowBalauException(Exception::FileNotFoundException, file_);
		}
	}

	public: FileUtf32To8WriteResource(FileUtf32To8WriteResource && rhs) noexcept
		: file(std::move(rhs.file))
		, stream(std::move(rhs.stream)) {}

	public: ~FileUtf32To8WriteResource() override {
		close();
	}

	public: std::u32ostream & writeStream() override {
		return *stream;
	}

	public: const Uri & uri() const override {
		return *file;
	}

	///
	/// Get the file URI.
	///
	/// @return the file URI
	///
	public: const File & getFile() const {
		return *file;
	}

	public: void close() override {
		stream->close();
	}

	////////////////////////// Private implementation /////////////////////////

	private: std::unique_ptr<File> file;
	private: std::unique_ptr<std::u32ofstream> stream;
};

inline std::unique_ptr<ByteReadResource> File::byteReadResource() const {
	return std::unique_ptr<ByteReadResource>(new FileByteReadResource(*this));
}

inline std::unique_ptr<Utf8To32ReadResource> File::utf8To32ReadResource() const {
	return std::unique_ptr<Utf8To32ReadResource>(new FileUtf8To32ReadResource(*this));
}

inline std::unique_ptr<ByteWriteResource> File::byteWriteResource() {
	return std::unique_ptr<ByteWriteResource>(new FileByteWriteResource(*this));
}

inline std::unique_ptr<Utf32To8WriteResource> File::utf32To8WriteResource() {
	return std::unique_ptr<Utf32To8WriteResource>(new FileUtf32To8WriteResource(*this));
}

inline FileByteReadResource File::getByteReadResource() const {
	return FileByteReadResource(*this);
}

inline FileUtf8To32ReadResource File::getUtf8To32ReadResource() const {
	return FileUtf8To32ReadResource(*this);
}

inline FileByteWriteResource File::getByteWriteResource() const {
	return FileByteWriteResource(*this);
}

inline FileUtf32To8WriteResource File::getUtf32To8WriteResource() const {
	return FileUtf32To8WriteResource(*this);
}

} // namespace Balau::Resource

namespace std { // NOLINT

template <> struct hash<Balau::Resource::File> {
	size_t operator () (const Balau::Resource::File & file) const noexcept {
		return file.hashcode();
	}
};

template <> struct equal_to<Balau::Resource::File> {
	bool operator () (const Balau::Resource::File & lhs, const Balau::Resource::File & rhs) const {
		return lhs == rhs;
	}
};

} // namespace std

#endif // COM_BORA_SOFTWARE__BALAU_RESOURCE__FILE
