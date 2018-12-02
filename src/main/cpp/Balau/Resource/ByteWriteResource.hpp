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
/// @file ByteWriteResource.hpp
///
/// A write-only resource which is written as bytes.
///

#ifndef COM_BORA_SOFTWARE__BALAU_RESOURCE__BYTE_WRITE_RESOURCE
#define COM_BORA_SOFTWARE__BALAU_RESOURCE__BYTE_WRITE_RESOURCE

#include <Balau/Resource/Resource.hpp>

#include <ostream>

namespace Balau::Resource {

///
/// A write-only resource which is written from bytes.
///
class ByteWriteResource : public Resource {
	///
	/// Get the output stream of the byte write resource.
	///
	public: virtual std::ostream & writeStream() = 0;
};

} // namespace Balau::Resource

#endif // COM_BORA_SOFTWARE__BALAU_RESOURCE__BYTE_WRITE_RESOURCE
