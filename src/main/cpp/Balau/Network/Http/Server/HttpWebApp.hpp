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

///
/// @file HttpWebApp.hpp
///
/// Abstract base class of HTTP web application handlers.
///

#ifndef COM_BORA_SOFTWARE__BALAU_NETWORK_SERVER__HTTP_WEB_APPLICATION
#define COM_BORA_SOFTWARE__BALAU_NETWORK_SERVER__HTTP_WEB_APPLICATION

#include <Balau/Network/Http/Server/NetworkTypes.hpp>
#include <Balau/Network/Http/Server/Impl/HttpWebAppFactory.hpp>

namespace Balau::Network::Http {

class HttpSession;

///
/// Abstract base class of HTTP web application handlers.
///
/// All concrete HTTP web applications must have a constructor of the following signature:
///
///  <pre>const EnvironmentProperties & configuration</pre>
///
/// This constructor will be used to instantiate the web application during initialisation
/// of the HTTP server.
///
/// This class also contains some canned responses for use in handler implementations.
///
class HttpWebApp {
	///
	/// Register an HTTP web application.
	///
	/// @tparam the HTTP web application type
	/// @param name the name of the composite property that provides the web app's configuration
	///
	public: template <typename WebAppT> static void registerHttpWebApp(const std::string & name) {
		Impl::HttpWebAppFactory::registerHttpWebApp<WebAppT>(name);
	}

	/////// Canned responses for use in web application implementations ///////

///////////////////// HTTP 200 /////////////////////

	///
	/// Create an empty 200 response.
	///
	public: static StringResponse createOkResponse(HttpSession & session, const StringRequest & request);

	///
	/// Create an empty 200 response for a head request.
	///
	public: static EmptyResponse createOkHeadResponse(HttpSession & session, const StringRequest & request);

	///////////////////// HTTP 300 /////////////////////

	///
	/// Create a redirect response.
	///
	public: static EmptyResponse createRedirectResponse(HttpSession & session, const StringRequest & request, std::string_view location);

	///
	/// Create a permanent redirect response.
	///
	public: static EmptyResponse createPermanentRedirectResponse(HttpSession & session, const StringRequest & request, std::string_view location);

	///////////////////// HTTP 400 /////////////////////

	///
	/// Create a bad request response.
	///
	public: static StringResponse createBadRequestResponse(HttpSession & session, const StringRequest & request, std::string_view errorMessage);

	///
	/// Create a bad request response for a head request.
	///
	public: static EmptyResponse createBadRequestHeadResponse(HttpSession & session, const StringRequest & request);

	///
	/// Create a not found response.
	///
	static StringResponse createNotFoundStringResponse(HttpSession & session, const StringRequest & request);

	///
	/// Create a not found response for a head request.
	///
	static EmptyResponse createNotFoundHeadResponse(HttpSession & session, const StringRequest & request);

	///////////////////// HTTP 500 /////////////////////

	///
	/// Create a server error response.
	///
	public: static StringResponse createServerErrorResponse(HttpSession & session, const StringRequest & request, std::string_view errorMessage);

	///
	/// Create a server error response for a head request.
	///
	public: static EmptyResponse createServerErrorHeadResponse(HttpSession & session, const StringRequest & request);

	///////////////////////////////////////////////////////////////////////////

	///
	/// Handle a GET request.
	///
	/// The supplied session provides shared server state and the sendResponse method.
	///
	/// @param session the HTTP session object, also containing the client session
	/// @param request the HTTP request object
	/// @param variables the request variables that are generated and consumed during the request
	///
	public: virtual void handleGetRequest(HttpSession & session,
	                                      const StringRequest & request,
	                                      std::map<std::string, std::string> & variables) = 0;

	///
	/// Handle a HEAD request.
	///
	/// The supplied session provides shared server state and the sendResponse method.
	///
	/// @param session the HTTP session object, also containing the client session
	/// @param request the HTTP request object
	/// @param variables the request variables that are generated and consumed during the request
	///
	public: virtual void handleHeadRequest(HttpSession & session,
	                                       const StringRequest & request,
	                                       std::map<std::string, std::string> & variables) = 0;

	///
	/// Handle a POST request.
	///
	/// The supplied session provides shared server state and the sendResponse method.
	///
	/// @param session the HTTP session object, also containing the client session
	/// @param request the HTTP request object
	/// @param variables the request variables that are generated and consumed during the request
	///
	public: virtual void handlePostRequest(HttpSession & session,
	                                       const StringRequest & request,
	                                       std::map<std::string, std::string> & variables) = 0;

	///////////////////////////////////////////////////////////////////////////

	///
	/// Destroy the HTTP web application instance.
	///
	public: virtual ~HttpWebApp() = default;
};

} // namespace Balau::Network::Http

#endif // COM_BORA_SOFTWARE__BALAU_NETWORK_SERVER__HTTP_WEB_APPLICATION
