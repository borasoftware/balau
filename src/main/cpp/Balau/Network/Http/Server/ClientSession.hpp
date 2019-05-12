// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2017 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
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
