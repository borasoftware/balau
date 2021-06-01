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
/// @resource ResourceExceptions.hpp
///
/// %Balau exceptions for resources.
///

#ifndef COM_BORA_SOFTWARE__BALAU_EXCEPTION__RESOURCE_EXCEPTIONS
#define COM_BORA_SOFTWARE__BALAU_EXCEPTION__RESOURCE_EXCEPTIONS

#include <Balau/Resource/Resource.hpp>
#include <Balau/Resource/Uri.hpp>

namespace Balau::Exception {

///
/// Thrown when a URI is invalid.
///
class InvalidUriException : public BalauException {
	public: InvalidUriException(SourceCodeLocation location, const std::string & st, const std::string & text)
		: BalauException(location, st, "InvalidUri", text) {}

	public: InvalidUriException(const std::string & st, const std::string & text)
		: BalauException(st, "InvalidUri", text) {}
};

///
/// Abstract base class of URI exceptions.
///
class ResourceException : public BalauException {
	public: std::unique_ptr<const Resource::Uri> uri;

	protected: ResourceException(SourceCodeLocation location,
	                             const std::string & st,
	                             const std::string & name,
	                             std::unique_ptr<const Resource::Uri> uri_)
		: BalauException(location, st, name, uri_->toRawString())
		, uri(std::move(uri_)) {}

	protected: ResourceException(SourceCodeLocation location,
	                             const std::string & st,
	                             const std::string & name,
	                             const std::string & message,
	                             std::unique_ptr<const Resource::Uri> uri_)
		: BalauException(location, st, name, message + " - " + uri_->toRawString())
		, uri(std::move(uri_)) {}

	protected: ResourceException(const std::string & st,
	                             const std::string & name,
	                             const std::string & text,
	                             std::unique_ptr<const Resource::Uri> uri_)
		: BalauException(st, name, text)
		, uri(std::move(uri_)) {}

	protected: ResourceException(const std::string & st,
	                             const std::string & name,
	                             std::unique_ptr<const Resource::Uri> uri_)
		: BalauException(st, name, "")
		, uri(std::move(uri_)) {}
};

///
/// Thrown when a resource is not found.
///
class NotFoundException : public ResourceException {
	public: NotFoundException(SourceCodeLocation location,
	                          const std::string & st,
	                          const std::string & text,
	                          std::unique_ptr<const Resource::Uri> uri_)
		: ResourceException(location, st, "NotFound", text, std::move(uri_)) {}

	public: NotFoundException(const std::string & st,
	                          const std::string & text,
	                          std::unique_ptr<const Resource::Uri> uri_)
		: ResourceException(st, "NotFound", text, std::move(uri_)) {}

	protected: NotFoundException(std::unique_ptr<const Resource::Uri> uri_,
	                             SourceCodeLocation location,
	                             const std::string & st,
	                             const std::string & name,
	                             bool)
		: ResourceException(location, st, name, std::move(uri_)) {}

	protected: NotFoundException(std::unique_ptr<const Resource::Uri> uri_,
	                             const std::string & st,
	                             const std::string & name,
	                             bool)
		: ResourceException(st, name, std::move(uri_)) {}
};

///
/// Thrown when a resource could not be opened.
///
class CouldNotOpenException : public ResourceException {
	public: CouldNotOpenException(SourceCodeLocation location,
	                              const std::string & st,
	                              std::unique_ptr<const Resource::Uri> uri_)
		: ResourceException(location, st, "CouldNotOpen", std::move(uri_)) {}

	public: CouldNotOpenException(const std::string & st,
	                              std::unique_ptr<const Resource::Uri> uri_)
		: ResourceException(st, "CouldNotOpen", std::move(uri_)) {}

	public: CouldNotOpenException(SourceCodeLocation location,
	                              const std::string & st,
	                              const std::string & message_,
	                              std::unique_ptr<const Resource::Uri> uri_)
		: ResourceException(location, st, "CouldNotOpen", message_, std::move(uri_)) {}

	public: CouldNotOpenException(const std::string & st,
	                              const std::string & message_,
	                              std::unique_ptr<const Resource::Uri> uri_)
		: ResourceException(st, "CouldNotOpen", message_, std::move(uri_)) {}
};

inline bool operator == (const CouldNotOpenException & lhs, const CouldNotOpenException & rhs) {
	return lhs.message == rhs.message && *lhs.uri == *rhs.uri;
}

///
/// Thrown when a resource could not be created.
///
class CouldNotCreateException : public ResourceException {
	public: CouldNotCreateException(SourceCodeLocation location,
	                                const std::string & st,
	                                std::unique_ptr<const Resource::Uri> uri_)
		: ResourceException(location, st, "CouldNotCreate", std::move(uri_)) {}

	public: CouldNotCreateException(const std::string & st,
	                                std::unique_ptr<const Resource::Uri> uri_)
		: ResourceException(st, "CouldNotCreate", std::move(uri_)) {}

	public: CouldNotCreateException(SourceCodeLocation location,
	                                const std::string & st,
	                                const std::string & message_,
	                                std::unique_ptr<const Resource::Uri> uri_)
		: ResourceException(location, st, "CouldNotCreate", message_, std::move(uri_)) {}

	public: CouldNotCreateException(const std::string & st,
	                                const std::string & message_,
	                                std::unique_ptr<const Resource::Uri> uri_)
		: ResourceException(st, "CouldNotCreate", message_, std::move(uri_)) {}
};

inline bool operator == (const CouldNotCreateException & lhs, const CouldNotCreateException & rhs) {
	return lhs.message == rhs.message && *lhs.uri == *rhs.uri;
}

} // namespace Balau::Exception

#endif // COM_BORA_SOFTWARE__BALAU_EXCEPTION__RESOURCE_EXCEPTIONS
