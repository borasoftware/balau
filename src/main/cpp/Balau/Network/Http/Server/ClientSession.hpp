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

#ifndef COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP__CLIENT_SESSION
#define COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP__CLIENT_SESSION

#include <Balau/Type/UUID.hpp>
#include <Balau/Util/DateTime.hpp>

namespace Balau::Network::Http {

///
/// The session object for a client.
///
/// The client can connect through multiple HTTP sessions. The client session
/// object is the same for all of the associated HTTP sessions.
///
/// The identity of a client session is made via the session cookie.
///
/// Client sessions are valid until the client does not connect for the configurable
/// timeout period or when a web application explicitly deletes the client session.
///
class ClientSession final {
	public: const std::string sessionId;

	///
	/// Create a client session object with a new session id.
	///
	public: ClientSession(const System::Clock & clock)
		: sessionId(UUID().asString() + ::toString(clock.millitime())) {}

	public: ClientSession(const ClientSession & ) = delete;
	public: ClientSession(ClientSession && ) = delete;
};

} // namespace Balau::Network::Http

#endif // COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP__CLIENT_SESSION
