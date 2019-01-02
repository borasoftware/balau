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
	protected: ResourceException(const char * file,
	                             int line,
	                             const std::string & st,
	                             const std::string & name,
	                             const std::string & text)
		: BalauException(file, line, st, name, text) {}
};

///
/// Thrown when a URI is invalid.
///
class InvalidUriException : public ResourceException {
	public: InvalidUriException(const char * file, int line, const std::string & st, const std::string & text)
		: ResourceException(file, line, st, "InvalidUri", text) {}
};

///
/// Thrown when a resource is not found.
///
class NotFoundException : public ResourceException {
	public: NotFoundException(const char * file, int line, const std::string & st, const std::string & text)
		: ResourceException(file, line, st, "NotFound", text) {}

	protected: NotFoundException(const char * file,
	                             int line,
	                             const std::string & st,
	                             const std::string & name,
	                             const std::string & text)
		: ResourceException(file, line, st, name, text) {}
};

///
/// Thrown when a resource is not found.
///
class FileNotFoundException : public NotFoundException {
	public: const Resource::File file;

	public: FileNotFoundException(const char * file, int line, const std::string & st, const Resource::File & file_)
		: NotFoundException(file, line, st, "FileNotFound", file_.toRawString())
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

	public: CouldNotOpenException(const char * file, int line, const std::string & st, const Resource::File & file_)
		: ResourceException(file, line, st, "CouldNotOpen", file_.toRawString())
		, file(file_) {}

	public: CouldNotOpenException(const char * file,
	                              int line,
	                              const std::string & st,
	                              const std::string & message_,
	                              const Resource::File & file_)
		: ResourceException(file, line, st, "CouldNotOpen", message_ + " - " + file_.toRawString())
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

	public: CouldNotCreateException(const char * file, int line, const std::string & st, const Resource::File & file_)
		: ResourceException(file, line, st, "CouldNotCreate", file_.toRawString())
		, file(file_) {}

	public: CouldNotCreateException(const char * file,
	                                int line,
	                                const std::string & st,
	                                const std::string & message_,
	                                const Resource::File & file_)
		: ResourceException(file, line, st, "CouldNotCreate", message_ + " - " + file_.toRawString())
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

	public: ZipException(const char * file, int line, const std::string & st, const Resource::File & file_)
		: ResourceException(file, line, st, "Zip", file_.toRawString())
		, file(file_) {}

	public: ZipException(const char * file,
	                     int line,
	                     const std::string & st,
	                     const std::string & message_,
	                     const Resource::File & file_)
		: ResourceException(file, line, st, "Zip", message_ + " - " + file_.toRawString())
		, file(file_) {}
};

inline bool operator == (const ZipException & lhs, const ZipException & rhs) {
	return lhs.message == rhs.message && lhs.file == rhs.file;
}

} // namespace Balau::Exception

#endif // COM_BORA_SOFTWARE__BALAU_EXCEPTION__RESOURCE_EXCEPTIONS
