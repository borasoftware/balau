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
/// @file ByteWriteResource.hpp
///
/// A write-only resource which is written as bytes.
///

#ifndef COM_BORA_SOFTWARE__BALAU_RESOURCE__BYTE_WRITE_RESOURCE
#define COM_BORA_SOFTWARE__BALAU_RESOURCE__BYTE_WRITE_RESOURCE

#include <Balau/Resource/Resource.hpp>

#include <ostream>

namespace Balau::Resource {

///
/// A write-only resource which is written from bytes.
///
class ByteWriteResource : public Resource {
	///
	/// Get the output stream of the byte write resource.
	///
	public: virtual std::ostream & writeStream() = 0;
};

} // namespace Balau::Resource

#endif // COM_BORA_SOFTWARE__BALAU_RESOURCE__BYTE_WRITE_RESOURCE
