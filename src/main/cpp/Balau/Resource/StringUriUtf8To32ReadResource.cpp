
// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include "StringUriUtf8To32ReadResource.hpp"
#include "StringUri.hpp"
#include "../Exception/ResourceExceptions.hpp"

namespace Balau::Resource {

StringUriUtf8To32ReadResource::StringUriUtf8To32ReadResource(const StringUri & stringUri_)
	: stringUri(std::make_unique<StringUri>(stringUri_))
	, utf8Stream(new std::istringstream(stringUri->getString()))
	, ref(*utf8Stream)
	, utf32Stream(new istream_utf8_utf32(ref)) {}

const Uri & StringUriUtf8To32ReadResource::uri() const {
	return *stringUri;
}

} // namespace Balau::Resource
