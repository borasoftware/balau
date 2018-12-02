// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include "CharacterTest.hpp"

#include <Balau/Type/Character.hpp>

namespace Balau {

using Testing::assertThat;
using Testing::is;

//// Classification ////

void CharacterTest::isLower() {}
void CharacterTest::isUpper() {}
void CharacterTest::isDigit() {}
void CharacterTest::isHexDigit() {}
void CharacterTest::isOctalDigit() {}
void CharacterTest::isBinaryDigit() {}

void CharacterTest::isAlpha() {	// C0 Controls and Basic Latin
	// Range: 0000–007F
	assertThat(Character::isAlpha(U'\u0000'), is(false)); // NUL
	assertThat(Character::isAlpha(U'\u0020'), is(false)); // SP
	assertThat(Character::isAlpha(U'\u002B'), is(false)); // +
	assertThat(Character::isAlpha(U'\u0037'), is(false)); // 7
	assertThat(Character::isAlpha(U'\u004B'), is(true));  // K
	assertThat(Character::isAlpha(U'\u0055'), is(true));  // U
	assertThat(Character::isAlpha(U'\u007E'), is(false)); // ~
	assertThat(Character::isAlpha(U'\u007F'), is(false)); // DEL

	// C1 Controls and Latin-1 Supplement
	// Range: 0080–00FF
	assertThat(Character::isAlpha(U'\u0080'), is(false)); // XXX
	assertThat(Character::isAlpha(U'\u00A3'), is(false)); // £
	assertThat(Character::isAlpha(U'\u00B6'), is(false)); // ¶
	assertThat(Character::isAlpha(U'\u00C0'), is(true));  // À
	assertThat(Character::isAlpha(U'\u00E0'), is(true));  // à
	assertThat(Character::isAlpha(U'\u00E7'), is(true));  // ç
	assertThat(Character::isAlpha(U'\u00F7'), is(false)); // ÷
	assertThat(Character::isAlpha(U'\u00FF'), is(true));  // ÿ

	// Latin Extended-A
	// Range: 0100–017F
	assertThat(Character::isAlpha(U'\u0100'), is(true));  // Ā
	assertThat(Character::isAlpha(U'\u0150'), is(true));  // Ő
	assertThat(Character::isAlpha(U'\u0153'), is(true));  // œ
	assertThat(Character::isAlpha(U'\u015A'), is(true));  // Ś
	assertThat(Character::isAlpha(U'\u0167'), is(true));  // ŧ
	assertThat(Character::isAlpha(U'\u016B'), is(true));  // ū
	assertThat(Character::isAlpha(U'\u016F'), is(true));  // ů
	assertThat(Character::isAlpha(U'\u017F'), is(true));  // s s

	// Greek and Coptic
	// Range: 0370–03FF
	assertThat(Character::isAlpha(U'\u037E'), is(false)); // ;
	assertThat(Character::isAlpha(U'\u039E'), is(true));  // Ξ
	assertThat(Character::isAlpha(U'\u03A0'), is(true));  // Π
	assertThat(Character::isAlpha(U'\u03B4'), is(true));  // δ
	assertThat(Character::isAlpha(U'\u03BB'), is(true));  // λ
	assertThat(Character::isAlpha(U'\u03F4'), is(true));  // Θ
	assertThat(Character::isAlpha(U'\u03FB'), is(true));  // ϻ
	assertThat(Character::isAlpha(U'\u03FF'), is(true));  // Ͽ
}


void CharacterTest::isAlphaOrDecimal() {}
void CharacterTest::isControlCharacter() {}
void CharacterTest::isSpace() {}
void CharacterTest::isSpaceExcludingCRLF() {}

void CharacterTest::isWhitespace() {
	assertThat(Character::isWhitespace(U' '),  is(true)); // ;
	assertThat(Character::isWhitespace(U'a'),  is(false));  // Ξ
	assertThat(Character::isWhitespace(U'b'),  is(false));  // Π
	assertThat(Character::isWhitespace(U'\n'), is(true));  // δ
	assertThat(Character::isWhitespace(U'\r'), is(true));  // λ
	assertThat(Character::isWhitespace(U'\t'), is(true));  // Θ
}

void CharacterTest::isBlank() {}
void CharacterTest::isPrintable() {}
void CharacterTest::isPunctuation() {}
void CharacterTest::isIdStart() {}
void CharacterTest::isIdPart() {}
void CharacterTest::isBreakableCharacter() {}
void CharacterTest::isInclusiveBreakableCharacter() {}

//// Iteration ////

void CharacterTest::getNextUtf8() {}
void CharacterTest::advanceUtf8() {}
void CharacterTest::retreatUtf8() {}

//// Mutation ////

void CharacterTest::toUpper() {}
void CharacterTest::toLower() {}
void CharacterTest::setUtf8AndAdvanceOffset() {}

} // namespace Balau
