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
/// @file ParsingExceptions.hpp
///
/// %Balau exceptions for the parser tools.
///

#ifndef COM_BORA_SOFTWARE__BALAU_EXCEPTION__PARSING_EXCEPTIONS
#define COM_BORA_SOFTWARE__BALAU_EXCEPTION__PARSING_EXCEPTIONS

#include <Balau/Exception/BalauException.hpp>
#include <Balau/Lang/Common/CodeSpan.hpp>

namespace Balau::Exception {

///
/// Thrown when a parser incurs invalid syntax.
///
class SyntaxErrorException : public BalauException {
	public: const Lang::CodeSpan codeSpan;

	public: SyntaxErrorException(const char * file,
	                             int line,
	                             const std::string & text,
	                             const Lang::CodeSpan & codeSpan_)
		: BalauException(file, line, "Network", text)
		, codeSpan(codeSpan_) {}
};

inline bool operator == (const SyntaxErrorException & lhs, const SyntaxErrorException & rhs) {
	return lhs.message == rhs.message && lhs.codeSpan == rhs.codeSpan;
}

inline std::string toString(const SyntaxErrorException & e) {
	return e.message + " - " + toString(e.codeSpan);
}

} // namespace Balau::Exception

inline std::string toString(const Balau::Exception::SyntaxErrorException & e) {
	return e.message + " - " + ::toString(e.codeSpan);
}

#endif // COM_BORA_SOFTWARE__BALAU_EXCEPTION__PARSING_EXCEPTIONS
