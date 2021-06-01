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
