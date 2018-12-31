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
	public: NetworkException(const char * file, int line, const std::string & st, const std::string & text)
		: BalauException(file, line, st, "Network", text) {}
};

template <typename AllocatorT>
inline Balau::U8String<AllocatorT> toString(const Balau::Exception::NetworkException & e) {
	return toString<AllocatorT>(e.what());
}

inline std::string toString(const Balau::Exception::NetworkException & e) {
	return e.what();
}

} // namespace Balau::Exception

#endif // COM_BORA_SOFTWARE__BALAU_EXCEPTION__NETWORK_EXCEPTIONS
