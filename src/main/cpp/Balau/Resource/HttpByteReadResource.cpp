// @formatter:off
//
// Balau core C++ library
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
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
