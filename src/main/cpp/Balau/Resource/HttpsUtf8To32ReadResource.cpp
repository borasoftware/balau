// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include "Https.hpp"

namespace Balau::Resource {

HttpsUtf8To32ReadResource::HttpsUtf8To32ReadResource(const Https & url_)
	: url(new Https(url_))
	, utf8Stream(url_)
	, ref(utf8Stream)
	, utf32Stream(ref) {}

const Uri & HttpsUtf8To32ReadResource::uri() const {
	return *url;
}

} // namespace Balau::Resource
