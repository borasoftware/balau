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
};

///
/// Returns true if the two code positions are equal.
///
/// @return true if the two code positions are equal
///
inline bool operator == (const CodePosition & lhs, const CodePosition & rhs) {
	return lhs.line == rhs.line && lhs.column == rhs.column;
}

///
/// Information on the span of some source code text.
///
class CodeSpan {
	///
	/// The location of the source file.
	///
	public: std::shared_ptr<const Resource::Uri> uri;

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
		return CodeSpan(start.uri, start.start.line, start.start.column, end.end.line, end.end.column);
	}

	///
	/// Create an empty code span equal to the start of the supplied code span.
	///
	/// @param next the code span
	/// @return an empty code span equal to the start of the supplied code span
	///
	public: static CodeSpan emptyCodeSpanStart(const CodeSpan & next) {
		return CodeSpan(next.uri, next.start.line, next.start.column, next.start.line, next.start.column);
	}

	///
	/// Create an empty code span equal to the end of the supplied code span.
	///
	/// @param previous the code span
	/// @return an empty code span equal to the end of the supplied code span
	///
	public: static CodeSpan emptyCodeSpanEnd(const CodeSpan & previous) {
		return CodeSpan(previous.uri, previous.end.line, previous.end.column, previous.end.line, previous.end.column);
	}

	///
	/// Create a null code span.
	///
	public: CodeSpan() : uri(), start(0, 0), end(0, 0) {}

	///
	/// Create a code span equal to the supplied start and end positions.
	///
	public: CodeSpan(const std::shared_ptr<const Resource::Uri> & uri_,
	                 unsigned int lineStart_,
	                 unsigned int columnStart_,
	                 unsigned int lineEnd_,
	                 unsigned int columnEnd_)
		: uri(uri_)
		, start(lineStart_, columnStart_)
		, end(lineEnd_, columnEnd_) {}

	///
	/// Create a code span equal to the supplied start and end positions.
	///
	public: CodeSpan(const std::shared_ptr<const Resource::Uri> & uri_,
	                 const CodePosition & start_,
	                 const CodePosition & end_)
		: uri(uri_)
		, start(start_)
		, end(end_) {}

	///
	/// Create a code span by copying the supplied code span.
	///
	public: CodeSpan(const CodeSpan & copy) = default;

	///
	/// Assign the supplied code span to the current code span.
	///
	/// @param rhs the code span to assign from
	/// @return the current object
	///
	public: CodeSpan & operator = (const CodeSpan & rhs) = default;

	///
	/// Returns true if the current code span is equal to the supplied code span.
	///
	/// @param rhs the code span to compare
	/// @return true if the current code span is equal to the supplied code span
	///
	public: bool operator == (const CodeSpan & rhs) const {
		return ((!uri && !rhs.uri) || (uri && rhs.uri && *uri == *rhs.uri)) && start == rhs.start && end == rhs.end;
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

template <typename AllocatorT>
inline Balau::U8String<AllocatorT> toString(const Balau::Lang::CodeSpan & codeSpan) {
	return toString<AllocatorT>(
		  "[L", codeSpan.start.line
		, "C",  codeSpan.start.column
		, ":L", codeSpan.end.line
		, "C",  codeSpan.end.column
		, "]"
	);
}

inline std::string toString(const Balau::Lang::CodeSpan & codeSpan) {
	return toString(
		  "[L", codeSpan.start.line
		, "C",  codeSpan.start.column
		, ":L", codeSpan.end.line
		, "C",  codeSpan.end.column
		, "]"
	);
}

#endif // COM_BORA_SOFTWARE__BALAU_LANG__CODE_SPAN
