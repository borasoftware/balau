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
/// @file HttpUtf8To32ReadResource.hpp
///
/// A read-only HTTP UTF-8 resource which is read as UTF-32 characters.
///

#ifndef COM_BORA_SOFTWARE__BALAU_RESOURCE__HTTP_UTF8_TO_32_READ_RESOURCE
#define COM_BORA_SOFTWARE__BALAU_RESOURCE__HTTP_UTF8_TO_32_READ_RESOURCE

#include <Balau/Resource/Utf8To32ReadResource.hpp>
#include <Balau/Resource/Impl/HttpSource.hpp>

#include <boost/iostreams/code_converter.hpp>
#include <boost/iostreams/stream.hpp>

#include <codecvt>

namespace Balau::Resource {

class Http;

///
/// A read-only HTTP UTF-8 resource which is read as UTF-32 characters.
///
class HttpUtf8To32ReadResource : public Utf8To32ReadResource {
	private: using idevice_utf8_utf32 = boost::iostreams::code_converter<std::istream, std::codecvt_utf8<char32_t, 0x10ffff, std::consume_header>>;
	private: using istream_utf8_utf32 = boost::iostreams::stream<idevice_utf8_utf32>;

	///
	/// Create a new HTTP UTF-8 to UTF-32 read resource from the supplied HTTP URI.
	///
	/// @throw InvalidUriException if no host information was provided
	///
	public: explicit HttpUtf8To32ReadResource(const Http & url_);

	public: HttpUtf8To32ReadResource(HttpUtf8To32ReadResource && rhs) noexcept
		: url(std::move(rhs.url))
		, utf8Stream(std::move(rhs.utf8Stream))
		, ref(rhs.ref)
		, utf32Stream(std::move(rhs.utf32Stream)) {}

	public: ~HttpUtf8To32ReadResource() override {
		close();
	}

	public: std::u32istream & readStream() override {
		return *utf32Stream;
	}

	public: const Uri & uri() const override;

	public: void close() override {
		utf32Stream->close();
	}

	////////////////////////// Private implementation /////////////////////////

	private: std::unique_ptr<Http> url;
	private: std::unique_ptr<boost::iostreams::stream<Impl::HttpSource>> utf8Stream;
	private: std::istream & ref;
	private: std::unique_ptr<istream_utf8_utf32> utf32Stream;
};

} // namespace Balau::Resource

#endif // COM_BORA_SOFTWARE__BALAU_RESOURCE__HTTP_UTF8_TO_32_READ_RESOURCE
