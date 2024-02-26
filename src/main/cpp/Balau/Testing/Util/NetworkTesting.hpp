// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
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

///
/// @file NetworkTesting.hpp
///
/// Test utilities for tests involving the network.
///

#ifndef COM_BORA_SOFTWARE_BALAU_TESTING_UTIL_NETWORK_TESTING
#define COM_BORA_SOFTWARE_BALAU_TESTING_UTIL_NETWORK_TESTING

#include <Balau/Exception/NetworkExceptions.hpp>
#include <Balau/Network/Http/Server/NetworkTypes.hpp>
#include <Balau/Util/Strings.hpp>

#include <functional>

namespace Balau::Testing {

///
/// Utilities for testing network functionality.
///
struct NetworkTesting final {
	///
	/// Repeatedly execute the supplied code until there is no network exception due to an in use port.
	///
	/// This function mitigates the inherent race condition that exists between
	/// obtaining a free port and using it.
	///
	/// The supplied code must contain a call to getFreeTcpPort in order to obtain a
	/// free test port.
	///
	/// @param code the code to execute that contains a call to getFreeTcpPort
	/// @param attempts the number of attempts to try (default = 10)
	/// @return the TCP port number that was used in the supplied code
	/// @throw NetworkException if all the attempts to get a free port failed
	///
	static unsigned short initialiseWithFreeTcpPort(std::function<unsigned short ()> code, size_t attempts = 10) {
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

	///
	/// Get a TCP port number that is free at the time of the call.
	///
	/// The best use of this function is to call it in a loop,  via the , until the component being
	/// tested stops complaining about a used port. Hopefully this will happen on the
	/// first iteration, but in exceptional circumstances, it may fail once or twice
	/// due to the unavoidable race condition.
	///
	/// @param start the start port to try
	/// @param count the number of sequential ports to try
	/// @return a TCP port number that was free at the time of the call
	/// @throw NetworkException if the attempts to get a free port failed
	///
	static unsigned short getFreeTcpPort(unsigned short start = 1025, unsigned short count = 65535) {
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

	///////////////////////////////////////////////////////////////////////////

	NetworkTesting() = delete;
	NetworkTesting(const NetworkTesting &) = delete;
	NetworkTesting & operator = (const NetworkTesting &) = delete;
};

} // namespace Balau::Testing

#endif // COM_BORA_SOFTWARE_BALAU_TESTING_UTIL_NETWORK_TESTING
