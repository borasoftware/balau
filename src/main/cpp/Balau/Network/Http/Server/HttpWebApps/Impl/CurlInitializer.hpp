// @formatter:off
//
// Balau core C++ library
//
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

#ifndef COM_BORA_SOFTWARE_BALAU_NETWORK_HTTP_SERVER_WEBAPP_IMPL_CURL_INITIALIZER
#define COM_BORA_SOFTWARE_BALAU_NETWORK_HTTP_SERVER_WEBAPP_IMPL_CURL_INITIALIZER

#include <Balau/Type/StdTypes.hpp>

namespace Balau::Network::Http::HttpWebApps::Impl {

//
// All uses of Curl functionality in the library must call ensureCurlInitialization()
// before using Curl, and Curl's initialisation and clean up will be managed by Balau.
//
void ensureCurlInitialization();

} // namespace Balau::Network::Http::HttpWebApps::Impl

#endif // COM_BORA_SOFTWARE_BALAU_NETWORK_HTTP_SERVER_WEBAPP_IMPL_CURL_INITIALIZER

