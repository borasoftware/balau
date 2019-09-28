// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#ifndef COM_BORA_SOFTWARE__BALAU_TESTING_IMPL__STANDARD_RENDERERS
#define COM_BORA_SOFTWARE__BALAU_TESTING_IMPL__STANDARD_RENDERERS

#include <Balau/Type/ToString.hpp>
#include <Balau/Util/Strings.hpp>

#include <iomanip>

namespace Balau::Testing::TestRenderers {

///
/// The standard renderer. Calls the Balau toString functions for the inputs and prints each
/// resulting line side by side, along with an infix == or != according to line equality.
///
template <typename A, typename E>
std::string render(const A & actual, const E & expected) {
	auto replaceNonPrintableCharacters = [] (const std::string & input) -> std::string {
		std::string output = input;
		Util::Strings::replaceAll(output, "\r\n", "<CRLF>");
		Util::Strings::replaceAll(output, "\n\r", "<LFCR>");
		Util::Strings::replaceAll(output, "\n",   "<LF>");
		Util::Strings::replaceAll(output, "\r",   "<CR>");
		Util::Strings::replaceAll(output, "\t",   "<TAB>");
		return output;
	};

	using ::toString;

	// If compilation fails here, define a toString function in the
	// global namespace for the type that is failing to resolve.
	const std::string actualString = toString(actual);
	const std::string expectedString = toString(expected);

	std::istringstream actualStream(actualString);
	std::istringstream expectedStream(expectedString);
	std::string actualLine;
	std::string expectedLine;

	size_t maxActualWidth = 0;
	size_t maxLineNumberWidth = 0;
	size_t actualLineCount = 0;
	size_t expectedLineCount = 0;

	// Get max line lengths / line counts.

	while (std::getline(actualStream, actualLine)) {
		if (maxActualWidth < actualLine.length()) {
			maxActualWidth = actualLine.length();
		}

		++actualLineCount;

		size_t lineNumberWidth = toString(actualLineCount).length();
		if (maxLineNumberWidth < lineNumberWidth) {
			maxLineNumberWidth = lineNumberWidth;
		}
	}

	while (std::getline(expectedStream, expectedLine)) {
		++expectedLineCount;

		size_t lineNumberWidth = toString(expectedLineCount).length();
		if (maxLineNumberWidth < lineNumberWidth) {
			maxLineNumberWidth = lineNumberWidth;
		}
	}

	// Single line version.

	if (actualLineCount == 1 && expectedLineCount == 1) {
		return replaceNonPrintableCharacters(actualString)
			+ (actual == expected ? " == " : " != ")
			+ replaceNonPrintableCharacters(expectedString);
	}

	// Multiple line version.

	actualStream.clear();
	actualStream.seekg(0);
	expectedStream.clear();
	expectedStream.seekg(0);

	std::ostringstream builder;
	int lineCount = 1;

	if (maxActualWidth < std::string("ACTUAL").length()) {
		maxActualWidth = std::string("ACTUAL").length();
	}

	builder << std::endl
	        << std::right << std::setw((int) maxLineNumberWidth) << "#"
	        << "  "
	        << std::left << std::setw((int) maxActualWidth) << "ACTUAL"
	        << "      "
	        << std::right << std::setw((int) maxLineNumberWidth) << "#"
	        << "  "
	        << "EXPECTED"
	        << std::endl;

	const size_t minLineCount = actualLineCount < expectedLineCount ? actualLineCount : expectedLineCount;

	for (size_t m = 0; m < minLineCount; m++) {
		std::getline(actualStream, actualLine);
		std::getline(expectedStream, expectedLine);

		builder << std::right << std::setw((int) maxLineNumberWidth) << toString(lineCount)
		        << "  "
		        << std::left << std::setw((int) maxActualWidth) << replaceNonPrintableCharacters(actualLine)
		        << "  " << (actualLine == expectedLine ? "==" : "!=") << "  "
		        << std::right << std::setw((int) maxLineNumberWidth) << toString(lineCount)
		        << "  "
		        << replaceNonPrintableCharacters(expectedLine)
		        << std::endl;

		++lineCount;
	}

	bool actualExtra = false;
	bool expectedExtra = false;

	while (std::getline(actualStream, actualLine)) {
		builder << std::right << std::setw((int) maxLineNumberWidth) << toString(lineCount)
		        << "  "
		        << std::left << std::setw((int) maxActualWidth) << replaceNonPrintableCharacters(actualLine)
		        << "  !=  "
		        << std::right << std::setw((int) maxLineNumberWidth) << toString(lineCount)
		        << "  "
		        << std::endl;

		++lineCount;
		actualExtra = true;
	}

	while (std::getline(expectedStream, expectedLine)) {
		builder << std::right
		        << std::setw((int) maxLineNumberWidth)
		        << toString(lineCount++)
		        << "  ";

		for (size_t m = 0; m < maxActualWidth; m++) {
			builder << " ";
		}

		builder << "  !=  "
		        << replaceNonPrintableCharacters(expectedLine)
		        << std::endl;

		expectedExtra = true;
	}

	const bool actualEndsWithNewline = actualString.back() == '\n';
	const bool expectedEndsWithNewline = expectedString.back() == '\n';

	if (actualEndsWithNewline && actualExtra) {
		builder << std::right << std::setw((int) maxLineNumberWidth) << toString(lineCount)
		        << "  "
		        << std::left << std::setw((int) maxActualWidth) << " "
		        << "  !="
		        << std::endl;
	} else if (expectedEndsWithNewline && expectedExtra) {
		builder << std::right << std::setw((int) maxLineNumberWidth) << " "
		        << "  "
		        << std::left << std::setw((int) maxActualWidth) << " "
		        << "  !=  "
		        << std::right << std::setw((int) maxLineNumberWidth) << "  "
		        << std::endl;
	}

	return builder.str();
}

///
/// Helper function for rendering standard library containers.
///
template <typename ... A, typename ... E, template <typename ...> class ContainerA, template <typename ...> class ContainerE>
std::string renderHelper(const ContainerA<A ...> & actual, const ContainerE<E ...> & expected) {
	auto replaceNonPrintableCharacters = [] (const std::string & input) -> std::string {
		std::string output = input;
		Util::Strings::replaceAll(output, "\r\n", "<CRLF>");
		Util::Strings::replaceAll(output, "\n\r", "<LFCR>");
		Util::Strings::replaceAll(output, "\n",   "<LF>");
		Util::Strings::replaceAll(output, "\r",   "<CR>");
		Util::Strings::replaceAll(output, "\t",   "<TAB>");
		return output;
	};

	using ::toString;

	const size_t actualCount = actual.size();
	const size_t expectedCount = expected.size();

	std::stringstream builder;

	size_t columnWidths[3] = { 0, 0, 0 };

	auto actualIter = actual.begin();
	auto expectedIter = expected.begin();

	const size_t minCount = actualCount < expectedCount ? actualCount : expectedCount;

	for (size_t m = 0; m < minCount; m++) {
		const auto & actualElement = *actualIter++;
		const auto & expectedElement = *expectedIter++;

		size_t len;

		len = toString(m).length();
		if (columnWidths[0] < len) {
			columnWidths[0] = len;
		}

		len = replaceNonPrintableCharacters(toString(actualElement)).length();
		if (columnWidths[1] < len) {
			columnWidths[1] = len;
		}

		len = replaceNonPrintableCharacters(toString(expectedElement)).length();
		if (columnWidths[2] < len) {
			columnWidths[2] = len;
		}
	}

	for (size_t m = minCount; m < actualCount; m++) {
		const auto & actualElement = *actualIter++;

		size_t len;

		len = toString(m).length();
		if (columnWidths[0] < len) {
			columnWidths[0] = len;
		}

		len = replaceNonPrintableCharacters(toString(actualElement)).length();
		if (columnWidths[1] < len) {
			columnWidths[1] = len;
		}
	}

	for (size_t m = minCount; m < expectedCount; m++) {
		const auto & expectedElement = *expectedIter++;

		size_t len;

		len = toString(m).length();
		if (columnWidths[0] < len) {
			columnWidths[0] = len;
		}

		len = replaceNonPrintableCharacters(toString(expectedElement)).length();
		if (columnWidths[2] < len) {
			columnWidths[2] = len;
		}
	}

	builder << std::right << std::setw((int) columnWidths[0]) << "#"
	        << std::left  << std::setw((int) columnWidths[1]) << "ACTUAL"
	        << "          "
	        << std::right << std::setw((int) columnWidths[2]) << "EXPECTED\n";

	actualIter = actual.begin();
	expectedIter = expected.begin();

	for (size_t m = 0; m < minCount; m++) {
		const auto & actualElement = *actualIter++;
		const auto & expectedElement = *expectedIter++;

		builder << std::right << std::setw((int) columnWidths[0]) << m << ":  ";

		builder << std::right << std::setw((int) columnWidths[1])
		        << replaceNonPrintableCharacters(toString(actualElement));

		if (actualElement == expectedElement) {
			builder << "    ==    ";
		} else {
			builder << "    !=    ";
		}

		builder << std::left << std::setw((int) columnWidths[2])
		        << replaceNonPrintableCharacters(toString(expectedElement));

		builder << "\n";
	}

	for (size_t m = minCount; m < actualCount; m++) {
		const auto & actualElement = *actualIter++;

		builder << std::right << std::setw((int) columnWidths[0]) << m << ":  ";

		builder << std::right << std::setw((int) columnWidths[1])
		        << replaceNonPrintableCharacters(toString(actualElement));

		builder << "    !=    \n";
	}

	for (size_t m = minCount; m < expectedCount; m++) {
		const auto & expectedElement = *expectedIter++;

		builder << std::right << std::setw((int) columnWidths[0]) << m << ":  ";
		builder << std::right << std::setw((int) columnWidths[1]) << " ";
		builder << "    !=    ";

		builder << std::left << std::setw((int) columnWidths[2])
		        << replaceNonPrintableCharacters(toString(expectedElement));

		builder << "\n";
	}

	builder << "\n";

	return builder.str();
}

///
/// Overload for rendering std::vector.
///
template <typename A, typename E> std::string render(const std::vector<A> & actual, const std::vector<E> & expected) {
	return renderHelper(actual, expected);
}

///
/// Overload for rendering std::list.
///
template <typename A, typename E> std::string render(const std::list<A> & actual, const std::list<E> & expected) {
	return renderHelper(actual, expected);
}

///
/// Overload for rendering std::set.
///
template <typename A, typename E> std::string render(const std::set<A> & actual, const std::set<E> & expected) {
	return renderHelper(actual, expected);
}

} // namespace Balau::Testing::TestRenderers

#endif // COM_BORA_SOFTWARE__BALAU_TESTING_IMPL__STANDARD_RENDERERS
