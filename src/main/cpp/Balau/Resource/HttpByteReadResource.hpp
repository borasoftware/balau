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
/// @file HttpByteReadResource.hpp
///
/// A read-only HTTP resource which is read as bytes.
///

#ifndef COM_BORA_SOFTWARE__BALAU_RESOURCE__HTTP_BYTE_READ_RESOURCE
#define COM_BORA_SOFTWARE__BALAU_RESOURCE__HTTP_BYTE_READ_RESOURCE

#include <Balau/Resource/ByteReadResource.hpp>
#include <Balau/Resource/Impl/HttpSource.hpp>

#include <boost/iostreams/stream.hpp>

namespace Balau::Resource {

class Http;
class Uri;

///
/// A read-only HTTP resource which is read as bytes.
///
class HttpByteReadResource : public ByteReadResource {
	///
	/// Create a new HTTP byte read resource from the supplied HTTP URI.
	///
	/// @throw InvalidUriException if no host information was provided
	///
	public: explicit HttpByteReadResource(const Http & url_);

	public: HttpByteReadResource(HttpByteReadResource && rhs) noexcept
		: url(std::move(rhs.url))
		, stream(std::move(rhs.stream)) {}

	public: ~HttpByteReadResource() override {
		close();
	}

	public: std::istream & readStream() override {
		return *stream;
	}

	public: const Uri & uri() const override;

	public: void close() override {
		stream->close();
	}

	////////////////////////// Private implementation /////////////////////////

	private: std::unique_ptr<Http> url;
	private: std::unique_ptr<boost::iostreams::stream<Impl::HttpSource>> stream;
};

} // namespace Balau::Resource

#endif // COM_BORA_SOFTWARE__BALAU_RESOURCE__HTTPS_BYTE_READ_RESOURCE
