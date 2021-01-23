// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

///
/// @file NetworkExceptions.hpp
///
/// %Balau exceptions for network operations.
///

#ifndef COM_BORA_SOFTWARE__BALAU_EXCEPTION__NETWORK_EXCEPTIONS
#define COM_BORA_SOFTWARE__BALAU_EXCEPTION__NETWORK_EXCEPTIONS

#include <Balau/Exception/BalauException.hpp>
#include <Balau/Resource/File.hpp>

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
