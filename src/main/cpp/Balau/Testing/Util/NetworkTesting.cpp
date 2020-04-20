// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include "NetworkTesting.hpp"

#include "../../Network/Http/Server/NetworkTypes.hpp"

namespace Balau::Testing {

unsigned short NetworkTesting::initialiseWithFreeTcpPort(std::function<unsigned short ()> code, size_t attempts) {
	size_t attemptsLeft = attempts;
	unsigned short port {};

	while (attemptsLeft > 0) {
		try {
			port = code();
		} catch (const Exception::NetworkException & e) {
			if (Util::Strings::contains(e.what(), "address in use")) {
				--attemptsLeft;
				continue;
			} else {
				throw;
			}
		}

		break; // Success.
	}

	if (attemptsLeft == 0) {
		ThrowBalauException(
			  Exception::NetworkException
			, ::toString("Failed to obtain free port within ", attempts, " attempts.")
		);
	}

	return port;
}

unsigned short NetworkTesting::getFreeTcpPort(unsigned short start, unsigned short count) {
	boost::system::error_code errorCode;
	boost::asio::io_context context;
	Network::TCP::acceptor acceptor(context);
	unsigned short port = start;

	while (port < USHRT_MAX && port < start + count) {
		Network::TCP::endpoint endpoint { boost::asio::ip::tcp::v4(), port };

		acceptor.open(endpoint.protocol(), errorCode);

		if (errorCode) {
			ThrowBalauException(Exception::NetworkException, errorCode.message());
		}

		acceptor.set_option(boost::asio::socket_base::reuse_address(true), errorCode);

		if (errorCode) {
			ThrowBalauException(Exception::NetworkException, errorCode.message());
		}

		acceptor.bind(endpoint, errorCode);

		if (errorCode) {
			if (errorCode != boost::system::errc::address_in_use) {
				ThrowBalauException(Exception::NetworkException, errorCode.message());
			}

			acceptor.close();
			++port;
		} else {
			acceptor.close();
			return port;
		}
	}

	ThrowBalauException(Exception::NetworkException, "Could not find a free port.");

	// Prevent compiler warning.
	return 0;
}

} // namespace Balau::Testing
