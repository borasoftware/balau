// @formatter:off
//
// Balau core C++ library
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

#ifndef COM_BORA_SOFTWARE__BALAU_TESTING_UTIL__NETWORK_TESTING
#define COM_BORA_SOFTWARE__BALAU_TESTING_UTIL__NETWORK_TESTING

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
	static unsigned short initialiseWithFreeTcpPort(std::function<unsigned short ()> code, size_t attempts = 10);

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
	static unsigned short getFreeTcpPort(unsigned short start = 1025, unsigned short count = 65535);

	///////////////////////////////////////////////////////////////////////////

	NetworkTesting() = delete;
	NetworkTesting(const NetworkTesting &) = delete;
	NetworkTesting & operator = (const NetworkTesting &) = delete;
};

} // namespace Balau::Testing

#endif // COM_BORA_SOFTWARE__BALAU_TESTING_UTIL__NETWORK_TESTING
