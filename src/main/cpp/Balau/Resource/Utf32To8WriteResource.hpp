// @formatter:off
//
// Balau core C++ library
//
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
/// @file Utf32To8WriteResource.hpp
///
/// A write-only UTF-8 resource, which is written as UTF-32 characters.
///

#ifndef COM_BORA_SOFTWARE__BALAU_RESOURCE__UTF32_TO_8_WRITE_RESOURCE
#define COM_BORA_SOFTWARE__BALAU_RESOURCE__UTF32_TO_8_WRITE_RESOURCE

#include <Balau/Resource/Resource.hpp>
#include <Balau/Type/StdTypes.hpp>

namespace Balau::Resource {

///
/// A write-only UTF-8 resource, which is written with UTF-32 characters.
///
class Utf32To8WriteResource : public Resource {
	///
	/// Get the output stream of the UTF-32 to UTF-8 write resource.
	///
	public: virtual std::u32ostream & writeStream() = 0;
};

} // namespace Balau::Resource

#endif // COM_BORA_SOFTWARE__BALAU_RESOURCE__UTF32_TO_8_WRITE_RESOURCE
