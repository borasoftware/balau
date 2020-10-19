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

#ifndef COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP_SERVER_IMPL__CLIENT_SESSIONS
#define COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP_SERVER_IMPL__CLIENT_SESSIONS

#include <Balau/Network/Http/Server/ClientSession.hpp>
#include <Balau/System/Clock.hpp>

#include <mutex>

namespace Balau::Network::Http {

class HttpSession;

namespace Impl {

class Listener;

//
// Holds client session information (currently just the session Id).
//
// TODO replace mutex with concurrent data structure.
//
class ClientSessions final {
	friend class ::Balau::Network::Http::HttpSession;
	friend class Listener;

	//
	// Create a client session.
	//
	// TODO Determine if there is a concurrency issue. Can user agents make
	// TODO multiple simultaneous requests without sending the session cookie?
	// TODO The result of this would be multiple client sessions being created
	// TODO for a single user agent.
	//
	private: std::shared_ptr<ClientSession> create(const Balau::System::Clock & clock) {
		{
			std::lock_guard<std::mutex> lock(mutex);
			auto session = std::make_shared<ClientSession>(clock);
			sessions[session->sessionId] = session;
			return session;
		}
	}

	private: std::shared_ptr<ClientSession> get(const std::string & sessionId) {
		std::lock_guard<std::mutex> lock(mutex);
		auto iter = sessions.find(sessionId);

		if (iter != sessions.end()) {
			return iter->second;
		} else {
			return std::shared_ptr<ClientSession>();
		}
	}

	private: void remove(const std::string & sessionId) {
		std::lock_guard<std::mutex> lock(mutex);
		sessions.erase(sessionId);
	}

	// Called a single time by the listener when it is shutting down.
	private: void clear() {
		std::lock_guard<std::mutex> lock(mutex);
		sessions.clear();
	}

	private: std::map<std::string, std::shared_ptr<ClientSession>> sessions;
	private: std::mutex mutex;
};

} // Impl

} // namespace Balau::Network::Http

#endif // COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP_SERVER_IMPL__CLIENT_SESSIONS
