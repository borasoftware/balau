// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2017 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#ifndef COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP_SERVER_WEB_APP_IMPL__CURL_EMAIL_SENDER
#define COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP_SERVER_WEB_APP_IMPL__CURL_EMAIL_SENDER

#ifdef BALAU_LIBCURL_ENABLED

#include <string>
#include <vector>

namespace Balau {

class Logger;

namespace Network::Http::HttpWebApps::Impl {

//
// An email sender based on libcurl.
// TODO non blocking version (queues emails to send and returns immediately).
//
class CurlEmailSender {
	public: CurlEmailSender(std::string host_,
	                        unsigned short port_,
	                        std::string user_,
	                        std::string pw_,
	                        std::string userAgent_,
	                        bool verifyCertificate_);

	public: void sendEmail(const std::string & from,
	                       const std::string & to,
	                       const std::vector<std::string> & cc,
	                       const std::string & subject,
	                       const std::string & body);

	///////////////////////// Private implementation /////////////////////////

	public: static Logger & log;

	private: const std::string host;
	private: const unsigned short port;
	private: const std::string user;
	private: const std::string pw;
	private: const std::string userAgent;
	private: bool verifyCertificate;
};

} // namespace Network::Http::HttpWebApps::Impl

} // namespace Balau

#endif // COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP_SERVER_WEB_APP_IMPL__CURL_EMAIL_SENDER

#endif // BALAU_LIBCURL_ENABLED
