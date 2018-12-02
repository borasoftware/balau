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
/// @file ByteReadResource.hpp
///
/// A read-only resource which is read as bytes.
///

#ifndef COM_BORA_SOFTWARE__BALAU_RESOURCE__BYTE_READ_RESOURCE
#define COM_BORA_SOFTWARE__BALAU_RESOURCE__BYTE_READ_RESOURCE

#include <Balau/Resource/Resource.hpp>

#include <istream>

namespace Balau::Resource {

///
/// A read-only resource which is read as bytes.
///
class ByteReadResource : public Resource {
	///
	/// Get the input stream of the byte read resource.
	///
	public: virtual std::istream & readStream() = 0;
};

} // namespace Balau::Resource

#endif // COM_BORA_SOFTWARE__BALAU_RESOURCE__BYTE_READ_RESOURCE
