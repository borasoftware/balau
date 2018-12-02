// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2017 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#ifndef COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP_SERVER_IMPL__SESSION_OBJECT
#define COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP_SERVER_IMPL__SESSION_OBJECT

namespace Balau::Network::Http::Impl {

//
// The session object for a client.
//
// The client can connect through multiple HTTP sessions. The client session
// object is the same for all of the associated HTTP sessions.
//
// The identity of a client session is made via the session cookie.
// Client sessions are valid until the client does not connect for the configurable timeout period.
//
class ClientSession {
};

} // namespace Balau::Network::Http::Impl

#endif // COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP_SERVER_IMPL__SESSION_OBJECT
