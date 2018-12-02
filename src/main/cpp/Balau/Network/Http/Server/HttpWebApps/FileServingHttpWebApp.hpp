// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2017 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

///
/// @file FileServingHttpWebApp.hpp
///
/// An HTTP web application handler that serves files from the file system.
///

#ifndef COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP_SERVER_HTTP_WEB_APPS__FILE_SERVING_WEB_APP
#define COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP_SERVER_HTTP_WEB_APPS__FILE_SERVING_WEB_APP

#include <Balau/Network/Http/Server/HttpWebApp.hpp>

namespace Balau::Network::Http::HttpWebApps {

///
/// An HTTP web application handler that serve files from the file system.
///
class FileServingHttpWebApp : public HttpWebApp {
	///
	/// Construct a file serving web application.
	///
	/// @param documentRoot_ a local file system folder that is the root of the file hierarchy to serve
	/// @param defaultFile_ the default file to serve if only a folder has been specified as the path
	///
	public: explicit FileServingHttpWebApp(Resource::File documentRoot_, std::string defaultFile_ = "index.html")
		: documentRoot(std::move(documentRoot_))
		, defaultFile(std::move(defaultFile_)) {}

	public: void handleGetRequest(HttpSession & session, const StringRequest & request) override;

	public: void handleHeadRequest(HttpSession & session, const StringRequest & request) override;

	public: void handlePostRequest(HttpSession & session, const StringRequest & request) override;

	///////////////////////// Private implementation //////////////////////////

	private: Resource::File resolvePath(HttpSession & session, const StringRequest & request);

	private: FileBodyValue getBody(HttpSession & session, const StringRequest & request, const std::string & pathStr);

	private: const Resource::File documentRoot;
	private: const std::string defaultFile;
};

} // namespace Balau::HttpWebApps::Http::Network

#endif // COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP_SERVER_HTTP_WEB_APPS__FILE_SERVING_WEB_APP
