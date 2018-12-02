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
/// @file Utf8To32ReadResource.hpp
///
/// A read-only UTF-8 resource which is read as UTF-32 characters.
///

#ifndef COM_BORA_SOFTWARE__BALAU_RESOURCE__UTF8_TO_32_READ_RESOURCE
#define COM_BORA_SOFTWARE__BALAU_RESOURCE__UTF8_TO_32_READ_RESOURCE

#include <Balau/Resource/Resource.hpp>
#include <Balau/Type/StdTypes.hpp>

namespace Balau::Resource {

///
/// A read-only UTF-8 resource which is read in as UTF-32 characters.
///
class Utf8To32ReadResource : public Resource {
	///
	/// Get the input stream of the UTF-8 to UTF-32 read resource.
	///
	public: virtual std::u32istream & readStream() = 0;
};

} // namespace Balau::Resource

#endif // COM_BORA_SOFTWARE__BALAU_RESOURCE__UTF8_TO_32_READ_RESOURCE
