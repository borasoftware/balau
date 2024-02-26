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
/// @file ParsingExceptions.hpp
///
/// %Balau exceptions for the parser tools.
///

#ifndef COM_BORA_SOFTWARE_BALAU_EXCEPTION_PARSING_EXCEPTIONS
#define COM_BORA_SOFTWARE_BALAU_EXCEPTION_PARSING_EXCEPTIONS

#include <Balau/Exception/BalauException.hpp>
#include <Balau/Lang/Common/CodeSpan.hpp>

namespace Balau::Exception {

///
/// Thrown when a parser incurs invalid syntax.
///
class SyntaxErrorException : public BalauException {
	public: const Lang::CodeSpan codeSpan;

	public: SyntaxErrorException(SourceCodeLocation location,
	                             const std::string & st,
	                             const std::string & text,
	                             const Lang::CodeSpan & codeSpan_)
		: BalauException(location, st, "Network", text)
		, codeSpan(codeSpan_) {}

	public: SyntaxErrorException(const std::string & st,
	                             const std::string & text,
	                             const Lang::CodeSpan & codeSpan_)
		: BalauException(st, "Network", text)
		, codeSpan(codeSpan_) {}
};

inline bool operator == (const SyntaxErrorException & lhs, const SyntaxErrorException & rhs) {
	return lhs.message == rhs.message && lhs.codeSpan == rhs.codeSpan;
}

template <typename AllocatorT>
inline Balau::U8String<AllocatorT> toString(const SyntaxErrorException & e) {
	return toString<AllocatorT>(e.message, " - ", e.codeSpan);
}

inline std::string toString(const SyntaxErrorException & e) {
	return e.message + " - " + toString(e.codeSpan);
}

} // namespace Balau::Exception

#endif // COM_BORA_SOFTWARE_BALAU_EXCEPTION_PARSING_EXCEPTIONS
