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

#include <TestResources.hpp>
#include <Balau/Type/Character.hpp>

namespace Balau {

using Testing::is;

struct CharacterTest : public Testing::TestGroup<CharacterTest> {
	CharacterTest() {
		//// Classification ////

		RegisterTestCase(isLower);
		RegisterTestCase(isUpper);
		RegisterTestCase(isDigit);
		RegisterTestCase(isHexDigit);
		RegisterTestCase(isOctalDigit);
		RegisterTestCase(isBinaryDigit);
		RegisterTestCase(isAlpha);
		RegisterTestCase(isAlphaOrDecimal);
		RegisterTestCase(isControlCharacter);
		RegisterTestCase(isSpace);
		RegisterTestCase(isSpaceExcludingCRLF);
		RegisterTestCase(isWhitespace);
		RegisterTestCase(isBlank);
		RegisterTestCase(isPrintable);
		RegisterTestCase(isPunctuation);
		RegisterTestCase(isIdStart);
		RegisterTestCase(isIdPart);
		RegisterTestCase(isBreakableCharacter);
		RegisterTestCase(isInclusiveBreakableCharacter);

		//// Iteration ////

		RegisterTestCase(getNextUtf8);
		RegisterTestCase(advanceUtf8);
		RegisterTestCase(retreatUtf8);

		//// Mutation ////

		RegisterTestCase(toUpper);
		RegisterTestCase(toLower);
		RegisterTestCase(setUtf8AndAdvanceOffset);
		RegisterTestCase(setUtf8AndAdvanceOffset);
	}

	// TODO implement the rest of the character tests.

	//// Classification ////

	void isLower() {}
	void isUpper() {}
	void isDigit() {}
	void isHexDigit() {}
	void isOctalDigit() {}
	void isBinaryDigit() {}

	void isAlpha() {	// C0 Controls and Basic Latin
		// Range: 0000–007F
		AssertThat(Character::isAlpha(U'\u0000'), is(false)); // NUL
		AssertThat(Character::isAlpha(U'\u0020'), is(false)); // SP
		AssertThat(Character::isAlpha(U'\u002B'), is(false)); // +
		AssertThat(Character::isAlpha(U'\u0037'), is(false)); // 7
		AssertThat(Character::isAlpha(U'\u004B'), is(true));  // K
		AssertThat(Character::isAlpha(U'\u0055'), is(true));  // U
		AssertThat(Character::isAlpha(U'\u007E'), is(false)); // ~
		AssertThat(Character::isAlpha(U'\u007F'), is(false)); // DEL

		// C1 Controls and Latin-1 Supplement
		// Range: 0080–00FF
		AssertThat(Character::isAlpha(U'\u0080'), is(false)); // XXX
		AssertThat(Character::isAlpha(U'\u00A3'), is(false)); // £
		AssertThat(Character::isAlpha(U'\u00B6'), is(false)); // ¶
		AssertThat(Character::isAlpha(U'\u00C0'), is(true));  // À
		AssertThat(Character::isAlpha(U'\u00E0'), is(true));  // à
		AssertThat(Character::isAlpha(U'\u00E7'), is(true));  // ç
		AssertThat(Character::isAlpha(U'\u00F7'), is(false)); // ÷
		AssertThat(Character::isAlpha(U'\u00FF'), is(true));  // ÿ

		// Latin Extended-A
		// Range: 0100–017F
		AssertThat(Character::isAlpha(U'\u0100'), is(true));  // Ā
		AssertThat(Character::isAlpha(U'\u0150'), is(true));  // Ő
		AssertThat(Character::isAlpha(U'\u0153'), is(true));  // œ
		AssertThat(Character::isAlpha(U'\u015A'), is(true));  // Ś
		AssertThat(Character::isAlpha(U'\u0167'), is(true));  // ŧ
		AssertThat(Character::isAlpha(U'\u016B'), is(true));  // ū
		AssertThat(Character::isAlpha(U'\u016F'), is(true));  // ů
		AssertThat(Character::isAlpha(U'\u017F'), is(true));  // s s

		// Greek and Coptic
		// Range: 0370–03FF
		AssertThat(Character::isAlpha(U'\u037E'), is(false)); // ;
		AssertThat(Character::isAlpha(U'\u039E'), is(true));  // Ξ
		AssertThat(Character::isAlpha(U'\u03A0'), is(true));  // Π
		AssertThat(Character::isAlpha(U'\u03B4'), is(true));  // δ
		AssertThat(Character::isAlpha(U'\u03BB'), is(true));  // λ
		AssertThat(Character::isAlpha(U'\u03F4'), is(true));  // Θ
		AssertThat(Character::isAlpha(U'\u03FB'), is(true));  // ϻ
		AssertThat(Character::isAlpha(U'\u03FF'), is(true));  // Ͽ
	}


	void isAlphaOrDecimal() {}
	void isControlCharacter() {}
	void isSpace() {}
	void isSpaceExcludingCRLF() {}

	void isWhitespace() {
		AssertThat(Character::isWhitespace(U' '),  is(true)); // ;
		AssertThat(Character::isWhitespace(U'a'),  is(false));  // Ξ
		AssertThat(Character::isWhitespace(U'b'),  is(false));  // Π
		AssertThat(Character::isWhitespace(U'\n'), is(true));  // δ
		AssertThat(Character::isWhitespace(U'\r'), is(true));  // λ
		AssertThat(Character::isWhitespace(U'\t'), is(true));  // Θ
	}

	void isBlank() {}
	void isPrintable() {}
	void isPunctuation() {}
	void isIdStart() {}
	void isIdPart() {}
	void isBreakableCharacter() {}
	void isInclusiveBreakableCharacter() {}

	//// Iteration ////

	void getNextUtf8() {}
	void advanceUtf8() {}
	void retreatUtf8() {}

	//// Mutation ////

	void toUpper() {}
	void toLower() {}
	void setUtf8AndAdvanceOffset() {}
};

} // namespace Balau
