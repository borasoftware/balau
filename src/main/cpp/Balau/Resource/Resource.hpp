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
/// @file Resource.hpp
///
/// Abstract base class of resource classes.
///

#ifndef COM_BORA_SOFTWARE__BALAU_RESOURCE__RESOURCE
#define COM_BORA_SOFTWARE__BALAU_RESOURCE__RESOURCE

namespace Balau::Resource {

class Uri;

///
/// Abstract base class of resource classes.
///
/// Instances of concrete classes are typically used within a
/// std::unique_ptr<Resource> or std::shared_ptr<Resource>.
///
class Resource {
	///
	/// Get the URI associated with this resource.
	///
	public: virtual const Uri & uri() const = 0;

	///
	/// Close the stream(s) in the resource.
	///
	public: virtual void close() = 0;

	///
	/// Destroy the resource instance.
	///
	public: virtual ~Resource() = default;
};

} // namespace Balau::Resource

#endif // COM_BORA_SOFTWARE__BALAU_RESOURCE__RESOURCE
