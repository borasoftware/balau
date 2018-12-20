// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2017 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include "Listener.hpp"
#include "../../../../Logging/Logger.hpp"

namespace Balau::Network::Http::Impl {

void Listener::onAccept(boost::system::error_code errorCode) {
	if (errorCode == boost::system::errc::operation_canceled) {
		BalauBalauLogInfo(
			  serverConfiguration->logger
			, "Listener {}:{} stopping due to: {}"
			, serverConfiguration->endpoint.address()
			, serverConfiguration->endpoint.port()
			, errorCode.message()
		);

		return;
	} else {
		httpSessions.start(
			std::make_shared<HttpSession>(httpSessions, clientSessions, serverConfiguration, std::move(socket))
		);
	}

	doAccept();
}

} // namespace Balau::Network::Http::Impl
