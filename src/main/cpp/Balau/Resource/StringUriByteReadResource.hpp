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
/// @file StringUriByteReadResource.hpp
///
/// A read-only string URI resource which is read as bytes.
///

#ifndef COM_BORA_SOFTWARE__BALAU_RESOURCE__STRING_URI_BYTE_READ_STRING
#define COM_BORA_SOFTWARE__BALAU_RESOURCE__STRING_URI_BYTE_READ_STRING

#include <Balau/Resource/ByteReadResource.hpp>

#include <memory>
#include <sstream>

namespace Balau::Resource {

class StringUri;
class Uri;

///
/// A read-only String URI resource which is read as bytes.
///
class StringUriByteReadResource : public ByteReadResource {
	///
	/// Create a new string byte read resource from the supplied String URI.
	///
	public: explicit StringUriByteReadResource(const StringUri & stringUri_);

	public: StringUriByteReadResource(StringUriByteReadResource && rhs) noexcept
		: stringUri(std::move(rhs.stringUri))
		, stream(std::move(rhs.stream)) {}

	public: std::istream & readStream() override {
		return stream;
	}

	public: const Uri & uri() const override;

	public: void close() override {}

	////////////////////////// Private implementation /////////////////////////

	private: std::unique_ptr<Uri> stringUri;
	private: std::istringstream stream;
};

} // namespace Balau::Resource

#endif // COM_BORA_SOFTWARE__BALAU_RESOURCE__STRING_URI_BYTE_READ_STRING
