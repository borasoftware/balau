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
#include <TestResources.hpp>
#include <Balau/Type/Character.hpp>

namespace Balau {

using Testing::is;

struct CharacterTest : public Testing::TestGroup<CharacterTest> {
	CharacterTest() {
		//// Classification ////

		RegisterTest(isLower);
		RegisterTest(isUpper);
		RegisterTest(isDigit);
		RegisterTest(isHexDigit);
		RegisterTest(isOctalDigit);
		RegisterTest(isBinaryDigit);
		RegisterTest(isAlpha);
		RegisterTest(isAlphaOrDecimal);
		RegisterTest(isControlCharacter);
		RegisterTest(isSpace);
		RegisterTest(isWhitespace);
		RegisterTest(isBlank);
		RegisterTest(isPrintable);
		RegisterTest(isPunctuation);
		RegisterTest(isIdStart);
		RegisterTest(isIdPart);
		RegisterTest(isBreakableCharacter);
		RegisterTest(isInclusiveBreakableCharacter);

		//// Iteration ////

		RegisterTest(getNextUtf8);
		RegisterTest(advanceUtf8);
		RegisterTest(retreatUtf8);

		//// Mutation ////

		RegisterTest(toLower);
		RegisterTest(toUpper);
		RegisterTest(setUtf8AndAdvanceOffset);
		RegisterTest(setUtf8AndAdvanceOffset);
	}

	//// Classification ////

	void isLower() {
		const bool fals = false;
		constexpr size_t c = 32;
		const char32_t i[c] = { U'0', U'3', U'-', U'(', U'#', U'$', U'.', U'>', U'a', U'A', U'd', U'D', U'z', U'Z', U'φ', U'Φ', U'ξ', U'Ξ', U'δ', U'Δ', U'ā', U'Ā', U'ő', U'Ő', U'œ', U'Œ', U'ś', U'Ś', U'ŧ', U'Ŧ', U'ů', U'Ů' };
		const bool     e[c] = { fals, fals, fals, fals, fals, fals, fals, fals, true, fals, true, fals, true, fals, true, fals, true, fals, true, fals, true, fals, true, fals, true, fals, true, fals, true, fals, true, fals };

		for (size_t m = 0; m < c; m++) {
			const auto in = i[m];
			const auto ac = Character::isLower(in);
			const auto ex = e[m];
			AssertThat(::toString(in, " tested"), ac, is(ex));
		}
	}

	void isUpper() {
		const bool fals = false;
		constexpr size_t c = 32;
		const char32_t i[c] = { U'0', U'3', U'-', U'(', U'#', U'$', U'.', U'>', U'a', U'A', U'd', U'D', U'z', U'Z', U'φ', U'Φ', U'ξ', U'Ξ', U'δ', U'Δ', U'ā', U'Ā', U'ő', U'Ő', U'œ', U'Œ', U'ś', U'Ś', U'ŧ', U'Ŧ', U'ů', U'Ů' };
		const bool     e[c] = { fals, fals, fals, fals, fals, fals, fals, fals, fals, true, fals, true, fals, true, fals, true, fals, true, fals, true, fals, true, fals, true, fals, true, fals, true, fals, true, fals, true };

		for (size_t m = 0; m < c; m++) {
			const auto in = i[m];
			const auto ac = Character::isUpper(in);
			const auto ex = e[m];
			AssertThat(::toString(in, " tested"), ac, is(ex));
		}
	}

	void isDigit() {
		const bool fals = false;
		constexpr size_t c = 32;
		const char32_t i[c] = { U'0', U'3', U'-', U'(', U'#', U'$', U'.', U'>', U'a', U'A', U'd', U'D', U'z', U'Z', U'φ', U'Φ', U'ξ', U'Ξ', U'δ', U'Δ', U'ā', U'Ā', U'ő', U'Ő', U'œ', U'Œ', U'ś', U'Ś', U'ŧ', U'Ŧ', U'ů', U'Ů' };
		const bool     e[c] = { true, true, fals, fals, fals, fals, fals, fals, fals, fals, fals, fals, fals, fals, fals, fals, fals, fals, fals, fals, fals, fals, fals, fals, fals, fals, fals, fals, fals, fals, fals, fals };

		for (size_t m = 0; m < c; m++) {
			const auto in = i[m];
			const auto ac = Character::isDigit(in);
			const auto ex = e[m];
			AssertThat(::toString(in, " tested"), ac, is(ex));
		}
	}

	void isHexDigit() {
		const bool fals = false;
		constexpr size_t c = 32;
		const char32_t i[c] = { U'0', U'3', U'-', U'(', U'#', U'$', U'.', U'>', U'a', U'A', U'd', U'D', U'z', U'Z', U'φ', U'Φ', U'ξ', U'Ξ', U'δ', U'Δ', U'ā', U'Ā', U'ő', U'Ő', U'œ', U'Œ', U'ś', U'Ś', U'ŧ', U'Ŧ', U'ů', U'Ů' };
		const bool     e[c] = { true, true, fals, fals, fals, fals, fals, fals, true, true, true, true, fals, fals, fals, fals, fals, fals, fals, fals, fals, fals, fals, fals, fals, fals, fals, fals, fals, fals, fals, fals };

		for (size_t m = 0; m < c; m++) {
			const auto in = i[m];
			const auto ac = Character::isHexDigit(in);
			const auto ex = e[m];
			AssertThat(::toString(in, " tested"), ac, is(ex));
		}
	}

	void isOctalDigit() {
		const bool fals = false;
		constexpr size_t c = 32;
		const char32_t i[c] = { U'0', U'1', U'2', U'3', U'4', U'5', U'6', U'7', U'8', U'#', U'$', U'.', U'>', U'a', U'A', U'd', U'D', U'z', U'Z', U'φ', U'Φ', U'ξ', U'Ξ', U'δ', U'Δ', U'ā', U'Ā', U'ő', U'Ő', U'œ', U'Œ', U'ś' };
		const bool     e[c] = { true, true, true, true, true, true, true, true, fals, fals, fals, fals, fals, fals, fals, fals, fals, fals, fals, fals, fals, fals, fals, fals, fals, fals, fals, fals, fals, fals, fals, fals };

		for (size_t m = 0; m < c; m++) {
			const auto in = i[m];
			const auto ac = Character::isOctalDigit(in);
			const auto ex = e[m];
			AssertThat(::toString(in, " tested"), ac, is(ex));
		}
	}

	void isBinaryDigit() {
		const bool fals = false;
		constexpr size_t c = 32;
		const char32_t i[c] = { U'0', U'1', U'2', U'3', U'#', U'$', U'.', U'>', U'a', U'A', U'd', U'D', U'z', U'Z', U'φ', U'Φ', U'ξ', U'Ξ', U'δ', U'Δ', U'ā', U'Ā', U'ő', U'Ő', U'œ', U'Œ', U'ś', U'Ś', U'ŧ', U'Ŧ', U'ů', U'Ů' };
		const bool     e[c] = { true, true, fals, fals, fals, fals, fals, fals, fals, fals, fals, fals, fals, fals, fals, fals, fals, fals, fals, fals, fals, fals, fals, fals, fals, fals, fals, fals, fals, fals, fals, fals };

		for (size_t m = 0; m < c; m++) {
			const auto in = i[m];
			const auto ac = Character::isBinaryDigit(in);
			const auto ex = e[m];
			AssertThat(::toString(in, " tested"), ac, is(ex));
		}
	}

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
		AssertThat(Character::isAlpha(U'\u017F'), is(true));  // s

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

	void isAlphaOrDecimal() {
		// Range: 0000–007F
		AssertThat(Character::isAlphaOrDecimal(U'\u0000'), is(false)); // NUL
		AssertThat(Character::isAlphaOrDecimal(U'\u0020'), is(false)); // SP
		AssertThat(Character::isAlphaOrDecimal(U'\u002B'), is(false)); // +
		AssertThat(Character::isAlphaOrDecimal(U'\u0037'), is(true));  // 7
		AssertThat(Character::isAlphaOrDecimal(U'\u004B'), is(true));  // K
		AssertThat(Character::isAlphaOrDecimal(U'\u0055'), is(true));  // U
		AssertThat(Character::isAlphaOrDecimal(U'\u007E'), is(false)); // ~
		AssertThat(Character::isAlphaOrDecimal(U'\u007F'), is(false)); // DEL

		AssertThat(Character::isAlphaOrDecimal(U'0'), is(true));  // 0
		AssertThat(Character::isAlphaOrDecimal(U'1'), is(true));  // 1
		AssertThat(Character::isAlphaOrDecimal(U'2'), is(true));  // 2
		AssertThat(Character::isAlphaOrDecimal(U'3'), is(true));  // 3
		AssertThat(Character::isAlphaOrDecimal(U'4'), is(true));  // 4
		AssertThat(Character::isAlphaOrDecimal(U'5'), is(true));  // 5
		AssertThat(Character::isAlphaOrDecimal(U'6'), is(true));  // 6
		AssertThat(Character::isAlphaOrDecimal(U'7'), is(true));  // 7
		AssertThat(Character::isAlphaOrDecimal(U'8'), is(true));  // 8
		AssertThat(Character::isAlphaOrDecimal(U'9'), is(true));  // 9

		// C1 Controls and Latin-1 Supplement
		// Range: 0080–00FF
		AssertThat(Character::isAlphaOrDecimal(U'\u0080'), is(false)); // XXX
		AssertThat(Character::isAlphaOrDecimal(U'\u00A3'), is(false)); // £
		AssertThat(Character::isAlphaOrDecimal(U'\u00B6'), is(false)); // ¶
		AssertThat(Character::isAlphaOrDecimal(U'\u00C0'), is(true));  // À
		AssertThat(Character::isAlphaOrDecimal(U'\u00E0'), is(true));  // à
		AssertThat(Character::isAlphaOrDecimal(U'\u00E7'), is(true));  // ç
		AssertThat(Character::isAlphaOrDecimal(U'\u00F7'), is(false)); // ÷
		AssertThat(Character::isAlphaOrDecimal(U'\u00FF'), is(true));  // ÿ

		// Latin Extended-A
		// Range: 0100–017F
		AssertThat(Character::isAlphaOrDecimal(U'\u0100'), is(true));  // Ā
		AssertThat(Character::isAlphaOrDecimal(U'\u0150'), is(true));  // Ő
		AssertThat(Character::isAlphaOrDecimal(U'\u0153'), is(true));  // œ
		AssertThat(Character::isAlphaOrDecimal(U'\u015A'), is(true));  // Ś
		AssertThat(Character::isAlphaOrDecimal(U'\u0167'), is(true));  // ŧ
		AssertThat(Character::isAlphaOrDecimal(U'\u016B'), is(true));  // ū
		AssertThat(Character::isAlphaOrDecimal(U'\u016F'), is(true));  // ů
		AssertThat(Character::isAlphaOrDecimal(U'\u017F'), is(true));  // s

		// Greek and Coptic
		// Range: 0370–03FF
		AssertThat(Character::isAlphaOrDecimal(U'\u037E'), is(false)); // ;
		AssertThat(Character::isAlphaOrDecimal(U'\u039E'), is(true));  // Ξ
		AssertThat(Character::isAlphaOrDecimal(U'\u03A0'), is(true));  // Π
		AssertThat(Character::isAlphaOrDecimal(U'\u03B4'), is(true));  // δ
		AssertThat(Character::isAlphaOrDecimal(U'\u03BB'), is(true));  // λ
		AssertThat(Character::isAlphaOrDecimal(U'\u03F4'), is(true));  // Θ
		AssertThat(Character::isAlphaOrDecimal(U'\u03FB'), is(true));  // ϻ
		AssertThat(Character::isAlphaOrDecimal(U'\u03FF'), is(true));  // Ͽ
	}

	void isControlCharacter() {}

	void isSpace() {
		AssertThat(Character::isSpace(U' '),  is(true));
		AssertThat(Character::isSpace(U'\t'), is(false));
		AssertThat(Character::isSpace(U'\n'), is(false));
		AssertThat(Character::isSpace(U'\r'), is(false));
		AssertThat(Character::isSpace(U'a'),  is(false));
		AssertThat(Character::isSpace(U'b'),  is(false));
		AssertThat(Character::isSpace(U'Ξ'),  is(false));
		AssertThat(Character::isSpace(U'Π'),  is(false));
		AssertThat(Character::isSpace(U'δ'),  is(false));
		AssertThat(Character::isSpace(U'λ'),  is(false));
		AssertThat(Character::isSpace(U'Θ'),  is(false));
	}

	void isWhitespace() {
		AssertThat(Character::isWhitespace(U' '),  is(true));
		AssertThat(Character::isWhitespace(U'\t'), is(true));
		AssertThat(Character::isWhitespace(U'\n'), is(true));
		AssertThat(Character::isWhitespace(U'\r'), is(true));
		AssertThat(Character::isWhitespace(U'a'),  is(false));
		AssertThat(Character::isWhitespace(U'b'),  is(false));
		AssertThat(Character::isWhitespace(U'Ξ'),  is(false));
		AssertThat(Character::isWhitespace(U'Π'),  is(false));
		AssertThat(Character::isWhitespace(U'δ'),  is(false));
		AssertThat(Character::isWhitespace(U'λ'),  is(false));
		AssertThat(Character::isWhitespace(U'Θ'),  is(false));
	}

	void isBlank() {
		AssertThat(Character::isBlank(U' '),  is(true));
		AssertThat(Character::isBlank(U'\t'), is(true));
		AssertThat(Character::isBlank(U'\n'), is(false));
		AssertThat(Character::isBlank(U'\r'), is(false));
		AssertThat(Character::isBlank(U'a'),  is(false));
		AssertThat(Character::isBlank(U'b'),  is(false));
		AssertThat(Character::isBlank(U'Ξ'),  is(false));
		AssertThat(Character::isBlank(U'Π'),  is(false));
		AssertThat(Character::isBlank(U'δ'),  is(false));
		AssertThat(Character::isBlank(U'λ'),  is(false));
		AssertThat(Character::isBlank(U'Θ'),  is(false));
	}

	// TODO Complete character tests.

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

	void toLower() {
		constexpr size_t c = 32;
		const char32_t i[c] = { U'0', U'3', U'-', U'(', U'#', U'$', U'.', U'>', U'a', U'A', U'd', U'D', U'z', U'Z', U'φ', U'Φ', U'ξ', U'Ξ', U'δ', U'Δ', U'ā', U'Ā', U'ő', U'Ő', U'œ', U'Œ', U'ś', U'Ś', U'ŧ', U'Ŧ', U'ů', U'Ů' };
		const char32_t e[c] = { U'0', U'3', U'-', U'(', U'#', U'$', U'.', U'>', U'a', U'a', U'd', U'd', U'z', U'z', U'φ', U'φ', U'ξ', U'ξ', U'δ', U'δ', U'ā', U'ā', U'ő', U'ő', U'œ', U'œ', U'ś', U'ś', U'ŧ', U'ŧ', U'ů', U'ů' };

		for (size_t m = 0; m < c; m++) {
			const auto in = i[m];
			const auto ac = Character::toLower(in);
			const auto ex = e[m];
			AssertThat(ac, is(ex));
		}
	}

	void toUpper() {
		constexpr size_t c = 32;
		const char32_t i[c] = { U'0', U'3', U'-', U'(', U'#', U'$', U'.', U'>', U'a', U'A', U'd', U'D', U'z', U'Z', U'φ', U'Φ', U'ξ', U'Ξ', U'δ', U'Δ', U'ā', U'Ā', U'ő', U'Ő', U'œ', U'Œ', U'ś', U'Ś', U'ŧ', U'Ŧ', U'Ů', U'Ů' };
		const char32_t e[c] = { U'0', U'3', U'-', U'(', U'#', U'$', U'.', U'>', U'A', U'A', U'D', U'D', U'Z', U'Z', U'Φ', U'Φ', U'Ξ', U'Ξ', U'Δ', U'Δ', U'Ā', U'Ā', U'Ő', U'Ő', U'Œ', U'Œ', U'Ś', U'Ś', U'Ŧ', U'Ŧ', U'Ů', U'Ů' };

		for (size_t m = 0; m < c; m++) {
			const auto in = i[m];
			const auto ac = Character::toUpper(in);
			const auto ex = e[m];
			AssertThat(ac, is(ex));
		}
	}

	void setUtf8AndAdvanceOffset() {}
};

} // namespace Balau
