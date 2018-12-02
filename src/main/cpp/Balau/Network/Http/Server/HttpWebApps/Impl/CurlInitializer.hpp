// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2017 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#ifndef COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP_SERVER_WEBAPP_IMPL__CURL_INITIALIZER
#define COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP_SERVER_WEBAPP_IMPL__CURL_INITIALIZER

namespace Balau::Network::Http::HttpWebApps::Impl {

//
// All uses of Curl functionality in the library must call ensureCurlInitialization()
// before using Curl, and Curl's initialisation and clean up will be managed by Balau.
//
void ensureCurlInitialization();

} // namespace Balau::Network::Http::HttpWebApps::Impl

#endif // COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP_SERVER_WEBAPP_IMPL__CURL_INITIALIZER
