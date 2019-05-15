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

HttpsByteReadResource::HttpsByteReadResource(const Https & url_)
	: url(new Https(url_))
	, stream(new boost::iostreams::stream<Impl::HttpsSource>(Impl::HttpsSource(url_))) {}

const Uri & HttpsByteReadResource::uri() const {
	return *url;
}

} // namespace Balau::Resource
