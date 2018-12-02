// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include "StringUriUtf32To8WriteResource.hpp"
#include "StringUri.hpp"
#include "../Exception/ResourceExceptions.hpp"

namespace Balau::Resource {

StringUriUtf32To8WriteResource::StringUriUtf32To8WriteResource(StringUri & stringUri_)
	: stringUri(stringUri_)
	, utf8Stream()
	, ref(utf8Stream)
	, utf32Stream(ref) {}

const Uri & StringUriUtf32To8WriteResource::uri() const {
	return stringUri;
}

void StringUriUtf32To8WriteResource::close() {
	// TODO Determine why the temporary padding is required.. is this a defect?
	utf32Stream << U"XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
	utf32Stream.flush();
	auto s = utf8Stream.str();
	std::string_view sv = s;
	std::string_view sv2 = sv.substr(0, sv.length() - 100);
	stringUri.append(sv2);
}

} // namespace Balau::Resource
