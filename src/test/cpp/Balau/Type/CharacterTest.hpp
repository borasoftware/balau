// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#ifndef COM_BORA_SOFTWARE__BALAU_TYPE__CHARACTER_TEST
#define COM_BORA_SOFTWARE__BALAU_TYPE__CHARACTER_TEST

#include <Balau/Testing/TestRunner.hpp>

namespace Balau {

struct CharacterTest : public Testing::TestGroup<CharacterTest> {
	explicit CharacterTest(Testing::TestRunner & runner) : TestGroup(runner) {
		//// Classification ////

		registerTest(&CharacterTest::isLower,                       "isLower");
		registerTest(&CharacterTest::isUpper,                       "isUpper");
		registerTest(&CharacterTest::isDigit,                       "isDigit");
		registerTest(&CharacterTest::isHexDigit,                    "isHexDigit");
		registerTest(&CharacterTest::isOctalDigit,                  "isOctalDigit");
		registerTest(&CharacterTest::isBinaryDigit,                 "isBinaryDigit");
		registerTest(&CharacterTest::isAlpha,                       "isAlpha");
		registerTest(&CharacterTest::isAlphaOrDecimal,              "isAlphaOrDecimal");
		registerTest(&CharacterTest::isControlCharacter,            "isControlCharacter");
		registerTest(&CharacterTest::isSpace,                       "isSpace");
		registerTest(&CharacterTest::isSpaceExcludingCRLF,          "isSpaceExcludingCRLF");
		registerTest(&CharacterTest::isWhitespace,                  "isWhitespace");
		registerTest(&CharacterTest::isBlank,                       "isBlank");
		registerTest(&CharacterTest::isPrintable,                   "isPrintable");
		registerTest(&CharacterTest::isPunctuation,                 "isPunctuation");
		registerTest(&CharacterTest::isIdStart,                     "isIdStart");
		registerTest(&CharacterTest::isIdPart,                      "isIdPart");
		registerTest(&CharacterTest::isBreakableCharacter,          "isBreakableCharacter");
		registerTest(&CharacterTest::isInclusiveBreakableCharacter, "isInclusiveBreakableCharacter");

		//// Iteration ////

		registerTest(&CharacterTest::getNextUtf8,                   "getNextUtf8");
		registerTest(&CharacterTest::advanceUtf8,                   "advanceUtf8");
		registerTest(&CharacterTest::retreatUtf8,                   "retreatUtf8");

		//// Mutation ////

		registerTest(&CharacterTest::toUpper,                       "toUpper");
		registerTest(&CharacterTest::toLower,                       "toLower");
		registerTest(&CharacterTest::setUtf8AndAdvanceOffset,       "setUtf8AndAdvanceOffset");
		registerTest(&CharacterTest::setUtf8AndAdvanceOffset,       "setUtf8AndAdvanceOffset");
	}

	//// Classification ////

	void isLower();
	void isUpper();
	void isDigit();
	void isHexDigit();
	void isOctalDigit();
	void isBinaryDigit();
	void isAlpha();
	void isAlphaOrDecimal();
	void isControlCharacter();
	void isSpace();
	void isSpaceExcludingCRLF();
	void isWhitespace();
	void isBlank();
	void isPrintable();
	void isPunctuation();
	void isIdStart();
	void isIdPart();
	void isBreakableCharacter();
	void isInclusiveBreakableCharacter();

	//// Iteration ////

	void getNextUtf8();
	void advanceUtf8();
	void retreatUtf8();

	//// Mutation ////

	void toUpper();
	void toLower();
	void setUtf8AndAdvanceOffset();
};

} // namespace Balau

#endif // COM_BORA_SOFTWARE__BALAU_TYPE__CHARACTER_TEST
