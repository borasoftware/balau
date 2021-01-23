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
/// @file TestExceptions.hpp
///
/// %Balau exceptions for the test framework.
///

#ifndef COM_BORA_SOFTWARE__BALAU_TYPE__SOURCE_CODE_LOCATION
#define COM_BORA_SOFTWARE__BALAU_TYPE__SOURCE_CODE_LOCATION

#include <Balau/Type/ToString.hpp>

namespace Balau {

///
/// A type used to representing a source code file and line number pair,
/// obtained by combining the __FILE__ and __LINE__ macros into a const char *.
///
/// Instances of this class are normally implicit, via the logging macros.
///
struct SourceCodeLocation {
	const char * location;

	explicit SourceCodeLocation(const char * location_ = nullptr)
		: location(location_) {}
};

inline std::string toString(const SourceCodeLocation & location) {
	return location.location;
}

} // namespace Balau

#endif // COM_BORA_SOFTWARE__BALAU_TYPE__SOURCE_CODE_LOCATION
