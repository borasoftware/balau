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

///
/// @file Utf8To32ReadResource.hpp
///
/// A read-only UTF-8 resource which is read as UTF-32 characters.
///

#ifndef COM_BORA_SOFTWARE__BALAU_RESOURCE__UTF8_TO_32_READ_RESOURCE
#define COM_BORA_SOFTWARE__BALAU_RESOURCE__UTF8_TO_32_READ_RESOURCE

#include <Balau/Resource/Resource.hpp>
#include <Balau/Type/StdTypes.hpp>

namespace Balau::Resource {

///
/// A read-only UTF-8 resource which is read in as UTF-32 characters.
///
class Utf8To32ReadResource : public Resource {
	///
	/// Get the input stream of the UTF-8 to UTF-32 read resource.
	///
	public: virtual std::u32istream & readStream() = 0;
};

} // namespace Balau::Resource

#endif // COM_BORA_SOFTWARE__BALAU_RESOURCE__UTF8_TO_32_READ_RESOURCE
