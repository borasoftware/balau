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
/// @file ResourceExceptions.hpp
///
/// %Balau exceptions for resources.
///

#ifndef COM_BORA_SOFTWARE__BALAU_EXCEPTION__RESOURCE_EXCEPTIONS
#define COM_BORA_SOFTWARE__BALAU_EXCEPTION__RESOURCE_EXCEPTIONS

#include <Balau/Exception/BalauException.hpp>
#include <Balau/Resource/File.hpp>

namespace Balau::Exception {

///
/// Abstract base class of resource classes.
///
class ResourceException : public BalauException {
	protected: ResourceException(SourceCodeLocation location,
	                             const std::string & st,
	                             const std::string & name,
	                             const std::string & text)
		: BalauException(location, st, name, text) {}

	protected: ResourceException(const std::string & st,
	                             const std::string & name,
	                             const std::string & text)
		: BalauException(st, name, text) {}
};

///
/// Thrown when a URI is invalid.
///
class InvalidUriException : public ResourceException {
	public: InvalidUriException(SourceCodeLocation location, const std::string & st, const std::string & text)
		: ResourceException(location, st, "InvalidUri", text) {}

	public: InvalidUriException(const std::string & st, const std::string & text)
		: ResourceException(st, "InvalidUri", text) {}
};

///
/// Thrown when a resource is not found.
///
class NotFoundException : public ResourceException {
	public: NotFoundException(SourceCodeLocation location, const std::string & st, const std::string & text)
		: ResourceException(location, st, "NotFound", text) {}

	public: NotFoundException(const std::string & st, const std::string & text)
		: ResourceException(st, "NotFound", text) {}

	protected: NotFoundException(SourceCodeLocation location,
	                             const std::string & st,
	                             const std::string & name,
	                             const std::string & text)
		: ResourceException(location, st, name, text) {}

	protected: NotFoundException(const std::string & st,
	                             const std::string & name,
	                             const std::string & text)
		: ResourceException(st, name, text) {}
};

///
/// Thrown when a resource is not found.
///
class FileNotFoundException : public NotFoundException {
	public: const Resource::File file;

	public: FileNotFoundException(SourceCodeLocation location, const std::string & st, const Resource::File & file_)
		: NotFoundException(location, st, "FileNotFound", file_.toRawString())
		, file(file_) {}

	public: FileNotFoundException(const std::string & st, const Resource::File & file_)
		: NotFoundException(st, "FileNotFound", file_.toRawString())
		, file(file_) {}
};

inline bool operator == (const FileNotFoundException & lhs, const FileNotFoundException & rhs) {
	return lhs.message == rhs.message && lhs.file == rhs.file;
}

///
/// Thrown when a resource could not be opened.
///
class CouldNotOpenException : public ResourceException {
	public: const Resource::File file;

	public: CouldNotOpenException(SourceCodeLocation location, const std::string & st, const Resource::File & file_)
		: ResourceException(location, st, "CouldNotOpen", file_.toRawString())
		, file(file_) {}

	public: CouldNotOpenException(const std::string & st, const Resource::File & file_)
		: ResourceException(st, "CouldNotOpen", file_.toRawString())
		, file(file_) {}

	public: CouldNotOpenException(SourceCodeLocation location,
	                              const std::string & st,
	                              const std::string & message_,
	                              const Resource::File & file_)
		: ResourceException(location, st, "CouldNotOpen", message_ + " - " + file_.toRawString())
		, file(file_) {}

	public: CouldNotOpenException(const std::string & st,
	                              const std::string & message_,
	                              const Resource::File & file_)
		: ResourceException(st, "CouldNotOpen", message_ + " - " + file_.toRawString())
		, file(file_) {}
};

inline bool operator == (const CouldNotOpenException & lhs, const CouldNotOpenException & rhs) {
	return lhs.message == rhs.message && lhs.file == rhs.file;
}

///
/// Thrown when a resource could not be created.
///
class CouldNotCreateException : public ResourceException {
	public: const Resource::File file;

	public: CouldNotCreateException(SourceCodeLocation location, const std::string & st, const Resource::File & file_)
		: ResourceException(location, st, "CouldNotCreate", file_.toRawString())
		, file(file_) {}

	public: CouldNotCreateException(const std::string & st, const Resource::File & file_)
		: ResourceException(st, "CouldNotCreate", file_.toRawString())
		, file(file_) {}

	public: CouldNotCreateException(SourceCodeLocation location,
	                                const std::string & st,
	                                const std::string & message_,
	                                const Resource::File & file_)
		: ResourceException(location, st, "CouldNotCreate", message_ + " - " + file_.toRawString())
		, file(file_) {}

	public: CouldNotCreateException(const std::string & st,
	                                const std::string & message_,
	                                const Resource::File & file_)
		: ResourceException(st, "CouldNotCreate", message_ + " - " + file_.toRawString())
		, file(file_) {}
};

inline bool operator == (const CouldNotCreateException & lhs, const CouldNotCreateException & rhs) {
	return lhs.message == rhs.message && lhs.file == rhs.file;
}

///
/// Thrown when a zip resource has an error.
///
class ZipException : public ResourceException {
	public: const Resource::File file;

	public: ZipException(SourceCodeLocation location, const std::string & st, const Resource::File & file_)
		: ResourceException(location, st, "Zip", file_.toRawString())
		, file(file_) {}

	public: ZipException(const std::string & st, const Resource::File & file_)
		: ResourceException(st, "Zip", file_.toRawString())
		, file(file_) {}

	public: ZipException(SourceCodeLocation location,
	                     const std::string & st,
	                     const std::string & message_,
	                     const Resource::File & file_)
		: ResourceException(location, st, "Zip", message_ + " - " + file_.toRawString())
		, file(file_) {}

	public: ZipException(const std::string & st,
	                     const std::string & message_,
	                     const Resource::File & file_)
		: ResourceException(st, "Zip", message_ + " - " + file_.toRawString())
		, file(file_) {}
};

inline bool operator == (const ZipException & lhs, const ZipException & rhs) {
	return lhs.message == rhs.message && lhs.file == rhs.file;
}

} // namespace Balau::Exception

#endif // COM_BORA_SOFTWARE__BALAU_EXCEPTION__RESOURCE_EXCEPTIONS
