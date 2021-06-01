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
/// @file NetworkExceptions.hpp
///
/// %Balau exceptions for network operations.
///

#ifndef COM_BORA_SOFTWARE__BALAU_EXCEPTION__NETWORK_EXCEPTIONS
#define COM_BORA_SOFTWARE__BALAU_EXCEPTION__NETWORK_EXCEPTIONS

#include <Balau/Exception/BalauException.hpp>

namespace Balau::Exception {

///
/// Base class of network exceptions.
///
class NetworkException : public BalauException {
	public: NetworkException(SourceCodeLocation location, const std::string & st, const std::string & text)
		: BalauException(location, st, "Network", text) {}

	public: NetworkException(const std::string & st, const std::string & text)
		: BalauException(st, "Network", text) {}
};

} // namespace Balau::Exception

#endif // COM_BORA_SOFTWARE__BALAU_EXCEPTION__NETWORK_EXCEPTIONS
