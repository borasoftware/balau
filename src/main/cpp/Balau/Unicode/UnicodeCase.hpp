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
// ////////////////////////////////////////////////////////////////////////////
//
// The source code in this file was adapted from the ICU project.
//
// Copyright © 1991-2020 Unicode, Inc. All rights reserved.
// Distributed under the Terms of Use in https://www.unicode.org/copyright.html.
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of the Unicode data files and any associated documentation
// (the "Data Files") or Unicode software and any associated documentation
// (the "Software") to deal in the Data Files or Software
// without restriction, including without limitation the rights to use,
// copy, modify, merge, publish, distribute, and/or sell copies of
// the Data Files or Software, and to permit persons to whom the Data Files
// or Software are furnished to do so, provided that either
// (a) this copyright and permission notice appear with all copies
// of the Data Files or Software, or
// (b) this copyright and permission notice appear in associated
// Documentation.
//
// THE DATA FILES AND SOFTWARE ARE PROVIDED "AS IS", WITHOUT WARRANTY OF
// ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
// WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT OF THIRD PARTY RIGHTS.
// IN NO EVENT SHALL THE COPYRIGHT HOLDER OR HOLDERS INCLUDED IN THIS
// NOTICE BE LIABLE FOR ANY CLAIM, OR ANY SPECIAL INDIRECT OR CONSEQUENTIAL
// DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
// DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
// TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
// PERFORMANCE OF THE DATA FILES OR SOFTWARE.
//
// Except as contained in this notice, the name of a copyright holder
// shall not be used in advertising or otherwise to promote the sale,
// use or other dealings in these Data Files or Software without prior
// written authorization of the copyright holder.
//

#ifndef COM_BORA_SOFTWARE__BALAU_TYPE_IMPL__UNICODE_CASE
#define COM_BORA_SOFTWARE__BALAU_TYPE_IMPL__UNICODE_CASE

#include <Balau/Util/Enums.hpp>
#include <Balau/Unicode/UnicodeProperties.hpp>
#include <Balau/Unicode/UnicodeTrie2.hpp>

namespace Balau::Unicode {

enum class UCase {
	None,
	Lower,
	Upper,
	Title
};

enum class UCaseExc {
	Lower,
	Fold,
	Upper,
	Title,
	Reserved4,  // reserved
	Reserved5,  // reserved
	Closure,
	Mappings,
	AllSlots    // one past the last slot
};

enum class UCaseIx {
	IndexTop,
	Length,
	TrieSize,
	ExcLength,
	UnfoldLength,
	MaxFullLength = 15,
	Top = 16
};

// each slot is 2 uint16_t instead of 1
constexpr int UCaseExcDoubleSlots = 0x100;
constexpr int UCaseException = 0x10;
constexpr int UCaseTypeMask = 3;
constexpr int UCaseDeltaShift = 7;
constexpr int UCaseDeltaMask = 0xff80;
constexpr int UCaseMaxDelta = 0xff;
constexpr int UCaseMinDelta = -UCaseMaxDelta - 1;
constexpr int UCaseExcShift = 5;
constexpr int UCaseExcMask = 0xffe0;
constexpr int UCaseMaxExceptions = (UCaseExcMask >> UCaseExcShift) + 1;

extern const UVersionInfo uCasePropsDataVersion;
extern const int32_t uCasePropsIndexes[Util::Enums::toUnderlying(UCaseIx::Top)];
extern const uint16_t uCasePropsTrieIndex[11720];
extern const uint16_t uCasePropsExceptions[1948];
extern const uint16_t uCasePropsUnfold[370];

struct UDataMemory;

struct UCaseProps {
	UDataMemory * mem;
	const int32_t * indexes;
	const uint16_t * exceptions;
	const uint16_t * unfold;

	UTrie2 trie;
	uint8_t formatVersion[4];
};

extern const UCaseProps uCasePropsSingleton;

inline uint32_t uCaseGetType(uint16_t props) {
	return props & Balau::Unicode::UCaseTypeMask;
}

inline int32_t uCaseGetType(char32_t c) {
	uint16_t props = uTrie2Get16(&Balau::Unicode::uCasePropsSingleton.trie, c);
	return uCaseGetType(props);
}

//
// Nearly all CPUs & compilers implement a right-shift of a signed integer
// as an Arithmetic Shift Right (with sign extension).
//
#if 1

inline int16_t uCaseGetDelta(uint16_t props) {
	return (int16_t) (
		(props & 0x8000)
		? (props >> Balau::Unicode::UCaseDeltaShift) | 0xfe00
		: (uint16_t) props >> Balau::Unicode::UCaseDeltaShift
	);
}

#else

inline int16_t uCaseGetDelta(uint16_t props) {
	return (int16_t) props >> UCaseDeltaShift;
}

#endif

inline const uint16_t * getExceptions(const Balau::Unicode::UCaseProps * csp, const uint16_t props) noexcept {
	return csp->exceptions + (props >> Balau::Unicode::UCaseExcShift);
}

inline bool propsHasException(const uint16_t props) {
	return props & Balau::Unicode::UCaseException;
}

inline bool hasSlot(const uint16_t flags, const unsigned int idx) {
	return flags & (1 << idx);
}

//
// Number of bits in an 8-bit integer value.
//
extern const uint8_t flagsOffset[256];

inline int slotOffset(const uint16_t flags, const unsigned int idx) {
	return flagsOffset[flags & ((1 << idx) - 1)];
}

//
// Get the value of an optional-value slot where hasSlot(excWord, idx).
//
// @param excWord (in) initial exceptions word
// @param idx (in) desired slot index
// @param pExc16 (in/out) const uint16_t * after excWord=*pExc16++;
//               moved to the last uint16_t of the value, use +1 for beginning of next slot
// @param value (out) int32_t or uint32_t output if hasSlot, otherwise not modified
//
inline void getSlotValue(const uint16_t excWord, const unsigned int idx, const uint16_t *& pExc16, uint32_t & value) {
	if ((excWord & Balau::Unicode::UCaseExcDoubleSlots) == 0) {
		pExc16 += slotOffset(excWord, idx);
		value = *pExc16;
	} else {
		pExc16 += 2 * slotOffset(excWord, idx);
		value = *pExc16++;
		value = (value << 16) | *pExc16;
	}
}

///
/// Check if a code point has the Lowercase Unicode property.
///
/// Same as hasBinaryProperty(c, UCHAR_LOWERCASE).
/// This is different from islower!
///
/// @param c Code point to test
/// @return true if the code point has the Lowercase Unicode property, false otherwise
///
/// @see UCHAR_LOWERCASE
/// @see islower
/// @see hasBinaryProperty
///
inline bool isULowercase(char32_t c) {
	return Util::Enums::toUnderlying(UCase::Lower) == uCaseGetType(c);
}

///
/// Check if a code point has the Uppercase Unicode property.
///
/// Same as hasBinaryProperty(c, UCHAR_UPPERCASE).
/// This is different from isupper!
///
/// @param c Code point to test
/// @return true if the code point has the Uppercase Unicode property, false otherwise
///
/// @see UCHAR_UPPERCASE
/// @see isupper
/// @see hasBinaryProperty
///
inline bool isUUppercase(char32_t c) {
	return Util::Enums::toUnderlying(UCase::Upper) == uCaseGetType(c);
}

///
/// The given character is mapped to its lowercase equivalent according to
/// UnicodeData.txt; if the character has no lowercase equivalent, the character
/// itself is returned.
///
/// Same as java.lang.Character.toLowerCase().
///
/// This function only returns the simple, single-code point case mapping.
/// Full case mappings should be used whenever possible because they produce
/// better results by working on whole strings.
/// They take into account the string context and the language and can map
/// to a result string with a different length as appropriate.
/// Full case mappings are applied by the string case mapping functions,
/// see ustring.h and the UnicodeString class.
/// See also the User Guide chapter on C/POSIX migration:
/// http://icu-project.org/userguide/posix.html#case_mappings
///
/// @param c the code point to be mapped
/// @return the Simple_Lowercase_Mapping of the code point, if any; otherwise the code point itself.
///
inline char32_t toLower(const char32_t c) {
	uint16_t props = uTrie2Get16(&uCasePropsSingleton.trie, c);
	auto cc = (uint32_t) c;

	if (!propsHasException(props)) {
		if (uCaseGetType(props) >= Util::Enums::toUnderlying(UCase::Upper)) {
			cc += uCaseGetDelta(props);
		}
	} else {
		const uint16_t * pe = getExceptions(&uCasePropsSingleton, props);
		uint16_t excWord = *pe++;

		if (hasSlot(excWord, Util::Enums::toUnderlying(UCaseExc::Lower))) {
			getSlotValue(excWord, Util::Enums::toUnderlying(UCaseExc::Lower), pe, cc);
		}
	}

	return (char32_t) cc;
}

///
/// The given character is mapped to its uppercase equivalent according to UnicodeData.txt;
/// if the character has no uppercase equivalent, the character itself is
/// returned.
///
/// Same as java.lang.Character.toUpperCase().
///
/// This function only returns the simple, single-code point case mapping.
/// Full case mappings should be used whenever possible because they produce
/// better results by working on whole strings.
/// They take into account the string context and the language and can map
/// to a result string with a different length as appropriate.
/// Full case mappings are applied by the string case mapping functions,
/// see ustring.h and the UnicodeString class.
/// See also the User Guide chapter on C/POSIX migration:
/// http://icu-project.org/userguide/posix.html#case_mappings
///
/// @param c the code point to be mapped
/// @return the Simple_Uppercase_Mapping of the code point, if any; otherwise the code point itself.
///
inline char32_t toUpper(const char32_t c) {
	uint16_t props = uTrie2Get16(&uCasePropsSingleton.trie, c);
	auto cc = (uint32_t) c;

	if (!propsHasException(props)) {
		if (uCaseGetType(props) == Util::Enums::toUnderlying(UCase::Lower)) {
			cc += uCaseGetDelta(props);
		}
	} else {
		const uint16_t * pe = getExceptions(&uCasePropsSingleton, props);
		uint16_t excWord = *pe++;

		if (hasSlot(excWord, Util::Enums::toUnderlying(UCaseExc::Upper))) {
			getSlotValue(excWord, Util::Enums::toUnderlying(UCaseExc::Upper), pe, cc);
		}
	}

	return (char32_t) cc;
}

} // namespace Balau::Unicode

#pragma clang diagnostic pop

#endif // COM_BORA_SOFTWARE__BALAU_TYPE_IMPL__UNICODE_CASE
