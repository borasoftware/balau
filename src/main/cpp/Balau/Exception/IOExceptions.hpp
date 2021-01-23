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
/// @file IOExceptions.hpp
///
/// %Balau exceptions for I/O.
///

#ifndef COM_BORA_SOFTWARE__BALAU_EXCEPTION__IO_EXCEPTIONS
#define COM_BORA_SOFTWARE__BALAU_EXCEPTION__IO_EXCEPTIONS

#include <Balau/Exception/BalauException.hpp>
#include <Balau/Resource/Uri.hpp>

namespace Balau::Exception {

///
/// Thrown when an IO exception occurs.
///
class IOException : public BalauException {
	public: IOException(SourceCodeLocation location, const std::string & st, const std::string & text)
		: BalauException(location, st, "IO", text) {}

	public: IOException(const std::string & st, const std::string & text)
		: BalauException(st, "IO", text) {}
};

} // namespace Balau::Exception

#endif // COM_BORA_SOFTWARE__BALAU_EXCEPTION__IO_EXCEPTIONS
