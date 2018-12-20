// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2017 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#ifndef COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP_SERVER_IMPL__HTTP_SESSIONS
#define COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP_SERVER_IMPL__HTTP_SESSIONS

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
class HttpSessions {
	public: void start(std::shared_ptr<HttpSession> session) {
		{
			std::lock_guard<std::mutex> lock(mutex);
			sessions.insert(session);
		}

		session->doRead();
	}

	public: void stop(const std::shared_ptr<HttpSession> & session) {
		std::lock_guard<std::mutex> lock(mutex);
		sessions.erase(session);
	}

	// Called a single time by the listener when it is shutting down.
	public: void stopAll() {
		std::set<std::shared_ptr<HttpSession>> sessionsCopy = sessions;

		for (auto & session : sessions) {
			session->stop();
		}

		sessions.clear();
	}

	private: std::set<std::shared_ptr<HttpSession>> sessions;
	private: std::mutex mutex;
};

} // namespace Balau::Network::Http::Impl

#endif // COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP_SERVER_IMPL__HTTP_SESSIONS
