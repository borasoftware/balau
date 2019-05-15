// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include "Http.hpp"

namespace Balau::Resource {

HttpByteReadResource::HttpByteReadResource(const Http & url_)
	: url(new Http(url_))
	, stream(new boost::iostreams::stream<Impl::HttpSource>(Impl::HttpSource(url_))) {}

const Uri & HttpByteReadResource::uri() const {
	return *url;
}

} // namespace Balau::Resource
