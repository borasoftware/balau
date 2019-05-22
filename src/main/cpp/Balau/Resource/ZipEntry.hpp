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
/// @file ZipEntry.hpp
///
/// An entry in a zip archive on the local file system.
///

#ifndef COM_BORA_SOFTWARE__BALAU_RESOURCE__ZIP_ENTRY
#define COM_BORA_SOFTWARE__BALAU_RESOURCE__ZIP_ENTRY

#ifdef BALAU_LIBZIP_ENABLED

#include <Balau/Resource/ZipEntryByteReadResource.hpp>
#include <Balau/Resource/ZipEntryUtf8To32ReadResource.hpp>
#include <Balau/Resource/UriComponents.hpp>
#include <Balau/Util/Files.hpp>

namespace Balau::Resource {

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

	public: std::unique_ptr<Uri> resolve(std::string_view path) const override {
		static const std::regex scheme { "[a-zA-Z][a-zA-Z0-9+-\\.]*:" };

		auto cleanPath = Util::Strings::trim(path);
		auto str = std::string(cleanPath);

		if (Util::Strings::startsWithRegex(str, scheme)) {
			std::unique_ptr<Uri> uri;
			fromString(uri, str);
			return uri;
		}

		if (Util::Strings::startsWith(str, "/")) {
			// Absolute path.
			return std::unique_ptr<Uri>(new ZipEntry(archive, archive.getEntryIndex(str)));
		} else {
			// Relative path.
			const std::string entryName = archive.getEntryName(entryIndex);
			const std::string normalisedName = UriComponents::normalizePath(entryName);
			const auto newName = normalisedName + str;
			return std::unique_ptr<Uri>(new ZipEntry(archive, archive.getEntryIndex(newName)));
		}
	}

	public: std::string toUriString() const override;

	public: std::string toRawString() const override {
		return toUriString();
	}

	///
	/// Return true if the entry is an archive directory.
	///
	/// @return true if the entry is an archive directory
	///
	public: bool isEntryDirectory() const;

	///
	/// Return true if the entry is an archive file.
	///
	/// @return true if the entry is an archive file
	///
	public: bool isEntryFile() const;

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
	public: size_t size() const;

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

	public: size_t hashcode() const noexcept override;

	public: bool operator == (const Uri & rhs) const override;

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
	public: ZipEntryByteReadResource getByteReadResource() const {
		return ZipEntryByteReadResource(*this);
	}

	///
	/// Get a byte read resource for this zip entry.
	///
	/// @throw ZipException if the resource could not be created
	///
	public: ZipEntryUtf8To32ReadResource getUtf8To32ReadResource() const {
		return ZipEntryUtf8To32ReadResource(*this);
	}

	public: std::unique_ptr<ByteReadResource> byteReadResource() const override {
		return std::unique_ptr<ByteReadResource>(new ZipEntryByteReadResource(*this));
	}

	public: std::unique_ptr<Utf8To32ReadResource> utf8To32ReadResource() const override {
		return std::unique_ptr<Utf8To32ReadResource>(new ZipEntryUtf8To32ReadResource(*this));
	}

	public: std::unique_ptr<ByteWriteResource> byteWriteResource() override {
		ThrowBalauException(Exception::NotImplementedException, "ZipEntry URIs do not have a byte write resource.");
	}

	public: std::unique_ptr<Utf32To8WriteResource> utf32To8WriteResource() override {
		ThrowBalauException(Exception::NotImplementedException, "ZipEntry URIs do not have a Unicode write resource.");
	}

	public: bool isRecursivelyIterable() const override {
		return false;
	}

	public: std::unique_ptr<RecursiveUriIterator> recursiveIterator() const override {
		ThrowBalauException(Exception::NotImplementedException, "Zip entry URIs do not have a recursive iterator");
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

	private: ZipEntry(Util::Unzipper & archive_, long long entryIndex_)
		: archive(archive_)
		, entryIndex(entryIndex_) {}

	private: Util::Unzipper & archive;
	private: long long entryIndex;
};

} // namespace Balau::Resource

#endif // BALAU_LIBZIP_ENABLED

#endif // COM_BORA_SOFTWARE__BALAU_RESOURCE__ZIP_ENTRY
