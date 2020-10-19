// @formatter:off
//
// Balau core C++ library
// Copyright (C) 2017 Bora Software (contact@borasoftware.com)
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

#ifndef COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP_SERVER_WEB_APP_IMPL__CURL_EMAIL_SENDER
#define COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP_SERVER_WEB_APP_IMPL__CURL_EMAIL_SENDER

#include <Balau/Type/StdTypes.hpp>

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
