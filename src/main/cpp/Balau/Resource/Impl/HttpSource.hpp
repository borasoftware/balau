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
#ifndef COM_BORA_SOFTWARE__BALAU_RESOURCE_IMPL__HTTP_SOURCE
#define COM_BORA_SOFTWARE__BALAU_RESOURCE_IMPL__HTTP_SOURCE

#include <Balau/Network/Http/Client/HttpClient.hpp>
#include <Balau/Resource/Uri.hpp>
#include <Balau/Resource/UriComponents.hpp>

#include <boost/iostreams/concepts.hpp>

namespace Balau::Resource {

class Http;

namespace Impl {

///
/// Boost IO streams HTTP source, used in the HTTP input stream.
///
/// @todo implement chunked reception of data
///
class HttpSource : public boost::iostreams::source {
	private: std::shared_ptr<Http> http;
	private: UriComponents url;
	private: Network::Http::HttpClient client;
	private: std::vector<char> responseBody;
	private: std::streamsize currentPosition;

	public: HttpSource(const Http & url_);

	public: HttpSource(const HttpSource &) = default;

	public: ~HttpSource();

	public: std::streamsize read(char * s, std::streamsize n);

	public: const Http & getUrl() const;

	///////////////////////// Private implementation //////////////////////////

	private: Network::Http::HttpClient createClient();
	private: std::vector<char> getBody();
};

} // namespace Impl

} // namespace Balau::Resource

#endif // COM_BORA_SOFTWARE__BALAU_RESOURCE_IMPL__HTTP_SOURCE
