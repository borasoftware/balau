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
