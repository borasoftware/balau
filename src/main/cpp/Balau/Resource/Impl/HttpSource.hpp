// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#ifndef COM_BORA_SOFTWARE__BALAU_RESOURCE_IMPL__HTTP_SOURCE
#define COM_BORA_SOFTWARE__BALAU_RESOURCE_IMPL__HTTP_SOURCE

#include <Balau/Network/Http/Client/HttpClient.hpp>
#include <Balau/Resource/Uri.hpp>
#include <Balau/Resource/UriComponents.hpp>

#include <boost/iostreams/concepts.hpp>

namespace Balau::Resource {

class Http;

} // namespace Balau::Resource

namespace Balau::Impl {

///
/// Boost IO streams HTTP source, used in the HTTP input stream.
///
/// @todo implement chunked reception of data
///
class HttpSource : public boost::iostreams::source {
	private: std::shared_ptr<Resource::Http> http;
	private: Resource::UriComponents url;
	private: Network::Http::HttpClient client;
	private: std::vector<char> responseBody;
	private: std::streamsize currentPosition;

	public: HttpSource(const Resource::Http & url_);

	public: HttpSource(const HttpSource &) = default;

	public: ~HttpSource();

	public: std::streamsize read(char * s, std::streamsize n);

	public: const Resource::Http & getUrl() const;

	///////////////////////// Private implementation //////////////////////////

	private: Network::Http::HttpClient createClient();
	private: std::vector<char> getBody();
};

} // namespace Balau::Impl

#endif // COM_BORA_SOFTWARE__BALAU_RESOURCE_IMPL__HTTP_SOURCE
