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
/// @file ByteReadResource.hpp
///
/// A read-only resource which is read as bytes.
///

#ifndef COM_BORA_SOFTWARE__BALAU_RESOURCE__BYTE_READ_RESOURCE
#define COM_BORA_SOFTWARE__BALAU_RESOURCE__BYTE_READ_RESOURCE

#include <Balau/Resource/Resource.hpp>

#include <istream>

namespace Balau::Resource {

///
/// A read-only resource which is read as bytes.
///
class ByteReadResource : public Resource {
	///
	/// Get the input stream of the byte read resource.
	///
	public: virtual std::istream & readStream() = 0;
};

} // namespace Balau::Resource

#endif // COM_BORA_SOFTWARE__BALAU_RESOURCE__BYTE_READ_RESOURCE
