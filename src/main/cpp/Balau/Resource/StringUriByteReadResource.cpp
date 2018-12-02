// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include "StringUri.hpp"

namespace Balau::Resource {

StringUriByteReadResource::StringUriByteReadResource(const StringUri & stringUri_)
	: stringUri(new StringUri(stringUri_))
	, stream(std::string(stringUri_.getString())) {}


const Uri & StringUriByteReadResource::uri() const {
	return *stringUri;
}

} // namespace Balau::Resource
