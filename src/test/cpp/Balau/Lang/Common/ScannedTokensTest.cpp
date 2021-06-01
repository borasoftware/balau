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

#include "Balau/Lang/Common/ScannedTokens.hpp"
#include <TestResources.hpp>

namespace Balau {

using Testing::is;

namespace Lang {

using UInt = unsigned int;

inline CodeSpan C(unsigned int lineStart, unsigned int columnStart, unsigned int lineEnd, unsigned int columnEnd) {
	return CodeSpan(lineStart, columnStart, lineEnd, columnEnd);
}

enum Token { Text, Blank, LineBreak, CommentLine, CommentBlock, EndOfFile };

const std::string text = "first second\nthird fourth fifth\n";

const std::vector<Token> tokens        = {       Text,      Blank,        Text,   LineBreak,       Text,      Blank,        Text,        Blank,         Text,   LineBreak,  EndOfFile };
const std::vector<UInt> offsets        = {          0,          5,           6,          12,         13,         18,          19,           25,           26,          31,         32 };
const std::vector<CodeSpan> codeSpans  = { C(1,1,1,6), C(1,6,1,7), C(1,7,1,13), C(1,13,2,1), C(2,1,2,6), C(2,6,2,7), C(2,7,2,13), C(2,13,2,14), C(2,14,2,19), C(2,19,3,1), C(3,1,3,1) };

struct ScannedTokensTest : public Testing::TestGroup<ScannedTokensTest> {
	ScannedTokensTest() {
		RegisterTestCase(randomAccessBuilding);
		RegisterTestCase(scannerApiConsumeAndPutBack);
	}

	void randomAccessBuilding() {
		RandomAccessScannedTokens<Token> sc(
			Lang::ScannedTokens<Token>(
				nullptr, std::string(text), std::vector<Token>(tokens), std::vector<UInt>(offsets)
			)
		);

		const std::vector<CodeSpan> & actualCodeSpans = sc.getCodeSpans();

		AssertThat(actualCodeSpans, is(codeSpans));
	}

	void scannerApiConsumeAndPutBack() {
		ScannerApiScannedTokens<Token> sc(
			Lang::ScannedTokens<Token>(
				nullptr, std::string(text), std::vector<Token>(tokens), std::vector<UInt>(offsets)
			)
		);

		ScannedToken<Token> token = sc.get();

		for (size_t m = 0; m < tokens.size(); m++) {
			const Token expectedToken = tokens[m];
			const CodeSpan expectedCodeSpan = codeSpans[m];

			const std::string expectedText = text.substr(
				offsets[m], (m == tokens.size() - 1 ? text.length() : offsets[m + 1]) - offsets[m]
			);

			AssertThat(token.token, is(expectedToken));
			AssertThat(token.codeSpan, is(expectedCodeSpan));
			AssertThat(token.text, is(expectedText));

			sc.consume();
			token = sc.get();
		}

		AssertThat(token.token, is(Token::EndOfFile));
		AssertThat(token.codeSpan, is(CodeSpan::emptyCodeSpanEnd(codeSpans[codeSpans.size() - 1])));

		int m = (int) tokens.size() - 1;

		while (m > 0) {
			sc.putBack();
			token = sc.get();

			// Retreated.
			m--;

			const Token expectedToken = tokens[m];
			const CodeSpan expectedCodeSpan = codeSpans[m];

			const std::string expectedText = text.substr(
				offsets[m], (m == (int) tokens.size() - 1 ? text.length() : offsets[m + 1]) - offsets[m]
			);

			AssertThat(token.token, is(expectedToken));
			AssertThat(token.codeSpan, is(expectedCodeSpan));
			AssertThat(token.text, is(expectedText));
		}
	}
};

} // namespace Lang

} // namespace Balau
