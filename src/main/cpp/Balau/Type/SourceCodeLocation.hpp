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
/// A type used to representing a source code file and line number pair, obtained via the __FILE__ and __LINE__ macros.
///
struct SourceCodeLocation {
	const char * file;
	unsigned int line;

	SourceCodeLocation()
		: file(nullptr)
		, line(0) {}

	SourceCodeLocation(const char * file_, unsigned int line_)
		: file(file_)
		, line(line_) {}
};

inline std::string toString(const SourceCodeLocation & location) {
	return ::toString(location.file) + ":" + ::toString(location.line);
}

} // namespace Balau

#endif // COM_BORA_SOFTWARE__BALAU_TYPE__SOURCE_CODE_LOCATION
