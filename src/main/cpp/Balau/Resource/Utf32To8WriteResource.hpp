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
/// @file Utf32To8WriteResource.hpp
///
/// A write-only UTF-8 resource, which is written as UTF-32 characters.
///

#ifndef COM_BORA_SOFTWARE__BALAU_RESOURCE__UTF32_TO_8_WRITE_RESOURCE
#define COM_BORA_SOFTWARE__BALAU_RESOURCE__UTF32_TO_8_WRITE_RESOURCE

#include <Balau/Resource/Resource.hpp>
#include <Balau/Type/StdTypes.hpp>

namespace Balau::Resource {

///
/// A write-only UTF-8 resource, which is written with UTF-32 characters.
///
class Utf32To8WriteResource : public Resource {
	///
	/// Get the output stream of the UTF-32 to UTF-8 write resource.
	///
	public: virtual std::u32ostream & writeStream() = 0;
};

} // namespace Balau::Resource

#endif // COM_BORA_SOFTWARE__BALAU_RESOURCE__UTF32_TO_8_WRITE_RESOURCE
