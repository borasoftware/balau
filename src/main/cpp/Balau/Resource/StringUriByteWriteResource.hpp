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
/// @file StringUriByteWriteResource.hpp
///
/// A write resource onto a String URI that is written as bytes.
///

#ifndef COM_BORA_SOFTWARE__BALAU_RESOURCE__STRING_URI_WRITE_RESOURCE
#define COM_BORA_SOFTWARE__BALAU_RESOURCE__STRING_URI_WRITE_RESOURCE

#include <Balau/Resource/ByteWriteResource.hpp>

#include <sstream>
#include <memory>

namespace Balau::Resource {

class StringUri;
class Uri;

///
/// A write only String URI that is written as bytes.
///
class StringUriByteWriteResource : public ByteWriteResource {
	///
	/// Create a new string Uri byte write resource from the supplied String URI.
	///
	public: explicit StringUriByteWriteResource(StringUri & stringUri_);

	public: ~StringUriByteWriteResource() {
		close();
	}

	public: std::ostream & writeStream() override {
		return stream;
	}

	public: const Uri & uri() const override;

	public: void close() override {}

	////////////////////////// Private implementation /////////////////////////

	private: std::unique_ptr<StringUri> stringUri;
	private: std::ostringstream stream;
};

} // namespace Balau::Resource

#endif // COM_BORA_SOFTWARE__BALAU_RESOURCE__STRING_URI_WRITE_RESOURCE
