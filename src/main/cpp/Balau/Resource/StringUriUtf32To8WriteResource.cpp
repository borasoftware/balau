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
#include "StringUriUtf32To8WriteResource.hpp"
#include "StringUri.hpp"
#include "../Exception/ResourceExceptions.hpp"

namespace Balau::Resource {

StringUriUtf32To8WriteResource::StringUriUtf32To8WriteResource(StringUri & stringUri_)
	: stringUri(stringUri_)
	, utf8Stream(new std::ostringstream())
	, ref(*utf8Stream)
	, utf32Stream(new ostream_utf32_utf8(ref)) {}

const Uri & StringUriUtf32To8WriteResource::uri() const {
	return stringUri;
}

void StringUriUtf32To8WriteResource::close() {
	// TODO Determine why the temporary padding is required.. is this a defect?
	*utf32Stream << U"XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
	utf32Stream->flush();
	auto s = utf8Stream->str();
	std::string_view sv = s;
	std::string_view sv2 = sv.substr(0, sv.length() - 100);
	stringUri.append(sv2);
}

} // namespace Balau::Resource
