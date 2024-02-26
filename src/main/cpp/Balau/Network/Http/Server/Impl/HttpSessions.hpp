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

#ifndef COM_BORA_SOFTWARE_BALAU_NETWORK_HTTP_SERVER_IMPL_HTTP_SESSIONS
#define COM_BORA_SOFTWARE_BALAU_NETWORK_HTTP_SERVER_IMPL_HTTP_SESSIONS

#include <Balau/Network/Http/Server/HttpSession.hpp>

#include <mutex>
#include <set>

namespace Balau::Network::Http::Impl {

//
// Maintains a thread-safe set of all HTTP sessions in order to allow forced
// closure if the server is halted.
//
// Each HTTP session has a reference to its owning HttpSessions instance.
//
// TODO replace mutex with concurrent data structure.
//
class HttpSessions final {
	public: void registerSession(const std::shared_ptr<HttpSession> & session) {
		std::lock_guard<std::recursive_mutex> lock(mutex);
		sessions.insert(session);
	}

	public: void unregisterSession(const std::shared_ptr<HttpSession> & session) {
		std::lock_guard<std::recursive_mutex> lock(mutex);
		sessions.erase(session);
	}

	// Called a single time by the listener when it is shutting down.
	public: void unregisterAllSessions(const BalauLogger & logger) {
		std::lock_guard<std::recursive_mutex> lock(mutex);

		BalauBalauLogInfo(logger, "HttpsSessions: unregistering {} HTTP sessions.", sessions.size());

		for (auto & session : sessions) {
			session->close();
		}

		sessions.clear();
	}
	private: std::set<std::shared_ptr<HttpSession>> sessions;
	private: std::recursive_mutex mutex;
};

} // namespace Balau::Network::Http::Impl

#endif // COM_BORA_SOFTWARE_BALAU_NETWORK_HTTP_SERVER_IMPL_HTTP_SESSIONS
