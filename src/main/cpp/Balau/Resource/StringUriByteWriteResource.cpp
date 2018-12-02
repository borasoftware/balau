// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include "StringUriByteWriteResource.hpp"
#include "StringUri.hpp"
#include "../Exception/ResourceExceptions.hpp"

namespace Balau::Resource {

StringUriByteWriteResource::StringUriByteWriteResource(StringUri & stringUri_)
	: stringUri(std::make_unique<StringUri>(stringUri_))
	, stream(stringUri->getString()) {}

const Uri & StringUriByteWriteResource::uri() const {
	return *stringUri;
}

} // namespace Balau::Resource
