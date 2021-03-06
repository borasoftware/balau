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
/// @file CodeSpan.hpp
///
/// A position in a piece of multi-line text.
///

#ifndef COM_BORA_SOFTWARE__BALAU_LANG__CODE_SPAN
#define COM_BORA_SOFTWARE__BALAU_LANG__CODE_SPAN

#include <Balau/Type/ToString.hpp>
#include <Balau/Resource/Uri.hpp>

namespace Balau::Lang {

///
/// A position in a piece of multi-line text.
///
/// The values are 1-indexed.
///
class CodePosition {
	///
	/// The line of the code position (1-indexed).
	///
	public: unsigned int line;

	///
	/// The column of the code position (1-indexed).
	///
	public: unsigned int column;

	///
	/// Create a null code position (line and colunn equal to 0).
	///
	public: CodePosition() : line(0), column(0) {}

	///
	/// Create a code position with the supplied line and colunn values.
	///
	public: CodePosition(unsigned int line_, unsigned int column_) : line(line_), column(column_) {}

	///
	/// Create a code position by copying the supplied code position.
	///
	public: CodePosition(const CodePosition & copy) = default;

	///
	/// Assign the code position by copying the supplied code position line and column values.
	///
	/// @return the current object
	///
	public: CodePosition & operator = (const CodePosition & copy) = default;

	///
	/// Is this code position equal to the supplied code position?
	///
	public: bool operator == (const CodePosition & rhs) const {
		return line == rhs.line && column == rhs.column;
	}

	///
	/// Does this code position come before the supplied code position?
	///
	public: bool operator < (const CodePosition & rhs) const {
		return line < rhs.line || (line == rhs.line && column < rhs.column);
	}

	///
	/// Does this code position come after the supplied code position?
	///
	public: bool operator > (const CodePosition & rhs) const {
		return line > rhs.line || (line == rhs.line && column > rhs.column);
	}
};

///
/// Information on the span of some source code text.
///
/// An instance of this structure should be accompanied by a URI for full
/// identification of the source code span.
///
class CodeSpan {
	///
	/// The start position of the code span.
	///
	public: CodePosition start;

	///
	/// The end position of the code span.
	///
	public: CodePosition end;

	///
	/// Create a code span equal to the start of the first code span and the end of the end code span.
	///
	/// It is the responsibility of the caller to ensure that both code spans originate from the same source file.
	///
	/// @param start the start code span
	/// @param end the end code span
	/// @return a code span equal to the start of the first code span and the end of the end code span
	///
	public: static CodeSpan totalCodeSpan(const CodeSpan & start, const CodeSpan & end) {
		return CodeSpan(start.start.line, start.start.column, end.end.line, end.end.column);
	}

	///
	/// Create an empty code span equal to the start of the supplied code span.
	///
	/// @param next the code span
	/// @return an empty code span equal to the start of the supplied code span
	///
	public: static CodeSpan emptyCodeSpanStart(const CodeSpan & next) {
		return CodeSpan(next.start.line, next.start.column, next.start.line, next.start.column);
	}

	///
	/// Create an empty code span equal to the end of the supplied code span.
	///
	/// @param previous the code span
	/// @return an empty code span equal to the end of the supplied code span
	///
	public: static CodeSpan emptyCodeSpanEnd(const CodeSpan & previous) {
		return CodeSpan(previous.end.line, previous.end.column, previous.end.line, previous.end.column);
	}

	///
	/// Create a null code span.
	///
	public: CodeSpan() : start(0, 0), end(0, 0) {}

	///
	/// Create a code span equal to the supplied start and end positions.
	///
	public: CodeSpan(unsigned int lineStart_,
	                 unsigned int columnStart_,
	                 unsigned int lineEnd_,
	                 unsigned int columnEnd_)
		: start(lineStart_, columnStart_)
		, end(lineEnd_, columnEnd_) {}

	///
	/// Create a code span equal to the supplied start and end positions.
	///
	public: CodeSpan(const CodePosition & start_,
	                 const CodePosition & end_)
		: start(start_)
		, end(end_) {}

	///
	/// Create a code span by copying the supplied code span.
	///
	public: CodeSpan(const CodeSpan & copy) = default;

	///
	/// Create a code span by moving the supplied code span.
	///
	public: CodeSpan( CodeSpan && copy) = default;

	///
	/// Copy assign the supplied code span to the current code span.
	///
	/// @param rhs the code span to assign from
	/// @return the current object
	///
	public: CodeSpan & operator = (const CodeSpan & rhs) = default;

	///
	/// Move assign the supplied code span to the current code span.
	///
	/// @param rhs the code span to assign from
	/// @return the current object
	///
	public: CodeSpan & operator = (CodeSpan && rhs) = default;

	///
	/// Returns true if the current code span is equal to the supplied code span.
	///
	/// @param rhs the code span to compare
	/// @return true if the current code span is equal to the supplied code span
	///
	public: bool operator == (const CodeSpan & rhs) const {
		return start == rhs.start && end == rhs.end;
	}

	///
	/// Is this code span less than the supplied code span?
	///
	/// As one code span can contain another code span, the definition of less than
	/// that is used here is wholy with respect to the start point of the code span.
	///
	///
	public: bool operator < (const CodeSpan & rhs) const {
		return start < rhs.start;
	}

	///
	/// Is this code span greater than the supplied code span?
	///
	/// As one code span can contain another code span, the definition of greater than
	/// that is used here is wholy with respect to the start point of the code span.
	///
	///
	public: bool operator > (const CodeSpan & rhs) const {
		return start > rhs.start;
	}

	///
	/// Extend the current code span to the end of the supplied code span.
	///
	/// It is the responsibility of the caller to ensure that the supplied code span
	/// originate from the same source file.
	///
	/// @param rhs the code span to extend to
	/// @return the current object
	///
	public: CodeSpan & operator += (const CodeSpan & rhs) {
		end = rhs.end;
		return *this;
	}
};

///
/// Print the supplied code span as a UTF-8 string.
///
template <typename AllocatorT>
inline Balau::U8String<AllocatorT> toString(const CodeSpan & codeSpan) {
	return ::toString<AllocatorT>(
		  "[L", codeSpan.start.line
		, "C",  codeSpan.start.column
		, ":L", codeSpan.end.line
		, "C",  codeSpan.end.column
		, "]"
	);
}

///
/// Print the supplied code span as a UTF-8 string.
///
inline std::string toString(const CodeSpan & codeSpan) {
	return ::toString(
		  "[L", codeSpan.start.line
		, "C",  codeSpan.start.column
		, ":L", codeSpan.end.line
		, "C",  codeSpan.end.column
		, "]"
	);
}

} // namespace Balau::Lang

#endif // COM_BORA_SOFTWARE__BALAU_LANG__CODE_SPAN
