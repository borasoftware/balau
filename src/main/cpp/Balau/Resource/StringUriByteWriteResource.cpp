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
