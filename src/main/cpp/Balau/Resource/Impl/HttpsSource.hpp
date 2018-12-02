// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#ifndef COM_BORA_SOFTWARE__BALAU_RESOURCE_IMPL__HTTPS_SOURCE
#define COM_BORA_SOFTWARE__BALAU_RESOURCE_IMPL__HTTPS_SOURCE

#include <Balau/Network/Http/Client/HttpsClient.hpp>
#include <Balau/Resource/UriComponents.hpp>

#include <boost/iostreams/concepts.hpp>

namespace Balau::Resource {

class Https;

namespace Impl {

///
/// Boost IO streams HTTPS source, used in the HTTPS input stream.
///
class HttpsSource : public boost::iostreams::source {
	private: std::shared_ptr<Https> https;
	private: UriComponents url;
	private: Network::Http::HttpsClient client;
	private: std::vector<char> responseBody;
	private: std::streamsize currentPosition;

	public: HttpsSource(const Https & url_);

	public: HttpsSource(const HttpsSource &) = default;

	public: ~HttpsSource();

	public: std::streamsize read(char * s, std::streamsize n);

	public: const Https & getUrl() const;

	///////////////////////// Private implementation //////////////////////////

	private: Network::Http::HttpsClient createClient();
	private: std::vector<char> getBody();
};

} // namespace Impl

} // namespace Balau::Resource

#endif // COM_BORA_SOFTWARE__BALAU_RESOURCE_IMPL__HTTPS_SOURCE
