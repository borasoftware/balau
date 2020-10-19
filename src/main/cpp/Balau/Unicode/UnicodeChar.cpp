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

#include "UnicodeChar.hpp"
#include "UnicodeCharPropertyData.hpp"
#include "UnicodeProperties.hpp"

namespace Balau::Unicode {

namespace {


//
// U_GC_XX_MASK constants are bit flags corresponding to Unicode
// general category values.
// For each category, the nth bit is set if the numeric value of the
// corresponding UCharCategory constant is n.
//
// There are also some U_GC_Y_MASK constants for groups of general categories
// like L for all letter categories.
//
// @see u_charType
// @see U_GET_GC_MASK
// @see UCharCategory
//
inline constexpr uint32_t gc_cn_mask() noexcept {
	return uMask(Util::Enums::toUnderlying(UCharCategory::U_GENERAL_OTHER_TYPES));
}

//
// Mask constant for a UCharCategory.
//
inline constexpr uint32_t gc_lu_mask() noexcept {
	return uMask(Util::Enums::toUnderlying(UCharCategory::U_UPPERCASE_LETTER));
}

//
// Mask constant for a UCharCategory.
//
inline constexpr uint32_t gc_ll_mask() noexcept {
	return uMask(Util::Enums::toUnderlying(UCharCategory::U_LOWERCASE_LETTER));
}

//
// Mask constant for a UCharCategory.
//
inline constexpr uint32_t gc_lt_mask() noexcept {
	return uMask(Util::Enums::toUnderlying(UCharCategory::U_TITLECASE_LETTER));
}

//
// Mask constant for a UCharCategory.
//
inline constexpr uint32_t gc_lm_mask() noexcept {
	return uMask(Util::Enums::toUnderlying(UCharCategory::U_MODIFIER_LETTER));
}

//
// Mask constant for a UCharCategory.
//
inline constexpr uint32_t gc_lo_mask() noexcept {
	return uMask(Util::Enums::toUnderlying(UCharCategory::U_OTHER_LETTER));
}

//
// Mask constant for a UCharCategory.
//
inline constexpr uint32_t gc_mn_mask() noexcept {
	return uMask(Util::Enums::toUnderlying(UCharCategory::U_NON_SPACING_MARK));
}

//
// Mask constant for a UCharCategory.
//
inline constexpr uint32_t gc_me_mask() noexcept {
	return uMask(Util::Enums::toUnderlying(UCharCategory::U_ENCLOSING_MARK));
}

//
// Mask constant for a UCharCategory.
//
inline constexpr uint32_t gc_mc_mask() noexcept {
	return uMask(Util::Enums::toUnderlying(UCharCategory::U_COMBINING_SPACING_MARK));
}

//
// Mask constant for a UCharCategory.
//
inline constexpr uint32_t gc_nd_mask() noexcept {
	return uMask(Util::Enums::toUnderlying(UCharCategory::U_DECIMAL_DIGIT_NUMBER));
}

//
// Mask constant for a UCharCategory.
//
inline constexpr uint32_t gc_nl_mask() noexcept {
	return uMask(Util::Enums::toUnderlying(UCharCategory::U_LETTER_NUMBER));
}

//
// Mask constant for a UCharCategory.
//
inline constexpr uint32_t gc_no_mask() noexcept {
	return uMask(Util::Enums::toUnderlying(UCharCategory::U_OTHER_NUMBER));
}

//
// Mask constant for a UCharCategory.
//
inline constexpr uint32_t gc_zs_mask() noexcept {
	return uMask(Util::Enums::toUnderlying(UCharCategory::U_SPACE_SEPARATOR));
}

//
// Mask constant for a UCharCategory.
//
inline constexpr uint32_t gc_zl_mask() noexcept {
	return uMask(Util::Enums::toUnderlying(UCharCategory::U_LINE_SEPARATOR));
}

//
// Mask constant for a UCharCategory.
//
inline constexpr uint32_t gc_zp_mask() noexcept {
	return uMask(Util::Enums::toUnderlying(UCharCategory::U_PARAGRAPH_SEPARATOR));
}

//
// Mask constant for a UCharCategory.
//
inline constexpr uint32_t gc_cc_mask() noexcept {
	return uMask(Util::Enums::toUnderlying(UCharCategory::U_CONTROL_CHAR));
}

//
// Mask constant for a UCharCategory.
//
inline constexpr uint32_t gc_cf_mask() noexcept {
	return uMask(Util::Enums::toUnderlying(UCharCategory::U_FORMAT_CHAR));
}

//
// Mask constant for a UCharCategory.
//
inline constexpr uint32_t gc_co_mask() noexcept {
	return uMask(Util::Enums::toUnderlying(UCharCategory::U_PRIVATE_USE_CHAR));
}

//
// Mask constant for a UCharCategory.
//
inline constexpr uint32_t gc_cs_mask() noexcept {
	return uMask(Util::Enums::toUnderlying(UCharCategory::U_SURROGATE));
}

//
// Mask constant for a UCharCategory.
//
inline constexpr uint32_t gc_pd_mask() noexcept {
	return uMask(Util::Enums::toUnderlying(UCharCategory::U_DASH_PUNCTUATION));
}

//
// Mask constant for a UCharCategory.
//
inline constexpr uint32_t gc_ps_mask() noexcept {
	return uMask(Util::Enums::toUnderlying(UCharCategory::U_START_PUNCTUATION));
}

//
// Mask constant for a UCharCategory.
//
inline constexpr uint32_t gc_pe_mask() noexcept {
	return uMask(Util::Enums::toUnderlying(UCharCategory::U_END_PUNCTUATION));
}

//
// Mask constant for a UCharCategory.
//
inline constexpr uint32_t gc_pc_mask() noexcept {
	return uMask(Util::Enums::toUnderlying(UCharCategory::U_CONNECTOR_PUNCTUATION));
}

//
// Mask constant for a UCharCategory.
//
inline constexpr uint32_t gc_po_mask() noexcept {
	return uMask(Util::Enums::toUnderlying(UCharCategory::U_OTHER_PUNCTUATION));
}

//
// Mask constant for a UCharCategory.
//
inline constexpr uint32_t gc_sm_mask() noexcept {
	return uMask(Util::Enums::toUnderlying(UCharCategory::U_MATH_SYMBOL));
}

//
// Mask constant for a UCharCategory.
//
inline constexpr uint32_t gc_sc_mask() noexcept {
	return uMask(Util::Enums::toUnderlying(UCharCategory::U_CURRENCY_SYMBOL));
}

//
// Mask constant for a UCharCategory.
//
inline constexpr uint32_t gc_sk_mask() noexcept {
	return uMask(Util::Enums::toUnderlying(UCharCategory::U_MODIFIER_SYMBOL));
}

//
// Mask constant for a UCharCategory.
//
inline constexpr uint32_t gc_so_mask() noexcept {
	return uMask(Util::Enums::toUnderlying(UCharCategory::U_OTHER_SYMBOL));
}

//
// Mask constant for a UCharCategory.
//
inline constexpr uint32_t gc_pi_mask() noexcept {
	return uMask(Util::Enums::toUnderlying(UCharCategory::U_INITIAL_PUNCTUATION));
}

//
// Mask constant for a UCharCategory.
//
inline constexpr uint32_t gc_pf_mask() noexcept {
	return uMask(Util::Enums::toUnderlying(UCharCategory::U_FINAL_PUNCTUATION));
}

//
// Mask constant for multiple UCharCategory bits (L Letters).
//
inline constexpr uint32_t gc_l_mask() noexcept {
	return gc_lu_mask() | gc_ll_mask() | gc_lt_mask() | gc_lm_mask() | gc_lo_mask();
}

//
// Mask constant for multiple UCharCategory bits (LC Cased Letters).
//
inline constexpr uint32_t gc_lc_mask() noexcept {
	return gc_lu_mask() | gc_ll_mask() | gc_lt_mask();
}

//
// Mask constant for multiple UCharCategory bits (M Marks).
//
inline constexpr uint32_t gc_m_mask() noexcept {
	return gc_mn_mask() | gc_me_mask() | gc_mc_mask();
}

//
// Mask constant for multiple UCharCategory bits (N Numbers).
//
inline constexpr uint32_t gc_n_mask() noexcept {
	return gc_nd_mask() | gc_nl_mask() | gc_no_mask();
}

//
// Mask constant for multiple UCharCategory bits (Z Separators).
//
inline constexpr uint32_t gc_z_mask() noexcept {
	return gc_zs_mask() | gc_zl_mask() | gc_zp_mask();
}

//
// Mask constant for multiple UCharCategory bits (C Others).
//
inline constexpr uint32_t gc_c_mask() noexcept {
	return gc_cn_mask() | gc_cc_mask() | gc_cf_mask() | gc_co_mask() | gc_cs_mask();
}

//
// Mask constant for multiple UCharCategory bits (P Punctuation).
//
inline constexpr uint32_t gc_p_mask() noexcept {
	return gc_pd_mask() | gc_ps_mask() | gc_pe_mask() | gc_pc_mask() | gc_po_mask() | gc_pi_mask() | gc_pf_mask();
}

//
// Mask constant for multiple UCharCategory bits (S Symbols).
//
inline constexpr uint32_t gc_s_mask() noexcept {
	return gc_sm_mask() | gc_sc_mask() | gc_sk_mask() | gc_so_mask();
}

// Getting a uint32_t properties word from the data.
inline void GET_PROPS(const char32_t c, uint32_t & result) {
	result = uTrie2Get16(&propsTrie, c);
}

// Enumerate all code points with their general categories.
struct _EnumTypeCallback {
	UCharEnumTypeRange enumRange;
	const void * context;
};

inline uint32_t _enumTypeValue(const void * context, const uint32_t value) {
	(void) context;
	return GET_CATEGORY(value);
}

inline bool _enumTypeRange(const void * context, const char32_t start, const char32_t end, const uint32_t value) {
	// Just cast the value to UCharCategory.
	return ((_EnumTypeCallback *) context)->enumRange(((_EnumTypeCallback *) context)->context, start, end + 1, (UCharCategory) value);
}

// Some control characters that are used as space.
inline bool IS_THAT_CONTROL_SPACE(const char32_t c) {
	return c <= 0x9f && ((c >= TAB && c <= CR) || (c >= 0x1c && c <= 0x1f) || c == NL);
}

// Java has decided that U+0085 New Line is not whitespace any more.
inline bool IS_THAT_ASCII_CONTROL_SPACE(const char32_t c) {
	return c <= 0x1f && c >= TAB && (c <= CR || c >= 0x1c);
}

} // namespace

void enumCharTypes(UCharEnumTypeRange enumRange, const void * context) {
	struct _EnumTypeCallback callback;

	if (enumRange == nullptr) {
		return;
	}

	callback.enumRange = enumRange;
	callback.context = context;
	utrie2Enum(&propsTrie, _enumTypeValue, _enumTypeRange, &callback);
}

bool isLower(const char32_t c) {
	uint32_t props;
	GET_PROPS(c, props);
	return GET_CATEGORY(props) == Util::Enums::toUnderlying(UCharCategory::U_LOWERCASE_LETTER);
}

bool isUpper(const char32_t c) {
	uint32_t props;
	GET_PROPS(c, props);
	return GET_CATEGORY(props) == Util::Enums::toUnderlying(UCharCategory::U_UPPERCASE_LETTER);
}

bool isTitle(const char32_t c) {
	uint32_t props;
	GET_PROPS(c, props);
	return GET_CATEGORY(props) == Util::Enums::toUnderlying(UCharCategory::U_TITLECASE_LETTER);
}

bool isDigit(const char32_t c) {
	uint32_t props;
	GET_PROPS(c, props);
	return GET_CATEGORY(props) == Util::Enums::toUnderlying(UCharCategory::U_DECIMAL_DIGIT_NUMBER);
}

bool isHexDigit(const char32_t c) {
	uint32_t props;

	// Check ASCII and Fullwidth ASCII a-fA-F.
	if ((c <= 0x66 && c >= 0x41 && (c <= 0x46 || c >= 0x61)) || (c >= 0xff21 && c <= 0xff46 && (c <= 0xff26 || c >= 0xff41))) {
		return true;
	}

	GET_PROPS(c, props);
	return GET_CATEGORY(props) == Util::Enums::toUnderlying(UCharCategory::U_DECIMAL_DIGIT_NUMBER);
}

bool isAlpha(const char32_t c) {
	uint32_t props;
	GET_PROPS(c, props);
	return (CAT_MASK(props) & (gc_l_mask())) != 0;
}

bool isUAlphabetic(const char32_t c) {
	return (getUnicodeProperties(c, 1) & uMask(UPROPS_ALPHABETIC)) != 0;
}

bool isAlphaNum(const char32_t c) {
	uint32_t props;
	GET_PROPS(c, props);
	return (CAT_MASK(props) & (gc_l_mask() | gc_nd_mask())) != 0;
}

bool isAlphaNumPOSIX(const char32_t c) {
	return isUAlphabetic(c) || isDigit(c);
}

bool isDefined(const char32_t c) {
	uint32_t props;
	GET_PROPS(c, props);
	return GET_CATEGORY(props) != 0;
}

bool isBase(const char32_t c) {
	uint32_t props;
	GET_PROPS(c, props);
	return (CAT_MASK(props) & (gc_l_mask() | gc_n_mask() | gc_mc_mask() | gc_me_mask())) != 0;
}

bool isControlCharacter(const char32_t c) {
	uint32_t props;
	GET_PROPS(c, props);
	return (CAT_MASK(props) & (gc_cc_mask() | gc_cf_mask() | gc_zl_mask() | gc_zp_mask())) != 0;
}

bool isISOControlCode(const char32_t c) {
	return c <= 0x9f && (c <= 0x1f || c >= 0x7f);
}

bool isSpace(const char32_t c) {
	uint32_t props;
	GET_PROPS(c, props);
	return (CAT_MASK(props) & gc_z_mask()) != 0 || IS_THAT_CONTROL_SPACE(c);
}

bool isJavaSpaceChar(const char32_t c) {
	uint32_t props;
	GET_PROPS(c, props);
	return (CAT_MASK(props) & gc_z_mask()) != 0;
}

bool isWhitespace(const char32_t c) {
	uint32_t props;
	GET_PROPS(c, props);

	return ((CAT_MASK(props) & gc_z_mask()) != 0 && c != NBSP && c != FIGURESP && c != NNBSP) // exclude no-break spaces
		|| IS_THAT_ASCII_CONTROL_SPACE(c);
}

bool isBlank(const char32_t c) {
	if ((uint32_t) c <= 0x9f) {
		return c == 9 || c == 0x20; // TAB or SPACE
	} else {
		// Zs
		uint32_t props;
		GET_PROPS(c, props);
		return GET_CATEGORY(props) == Util::Enums::toUnderlying(UCharCategory::U_SPACE_SEPARATOR);
	}
}

bool u_isUWhiteSpace(const char32_t c) {
	return (getUnicodeProperties(c, 1) & uMask(UPROPS_WHITE_SPACE)) != 0;
}

bool isPrintable(const char32_t c) {
	uint32_t props;
	GET_PROPS(c, props);
	// Comparing ==0 returns false for the categories mentioned.
	return (CAT_MASK(props) & gc_c_mask()) == 0;
}

bool isPrintPOSIX(const char32_t c) {
	uint32_t props;
	GET_PROPS(c, props);

	//
	// The only cntrl character in graph+blank is TAB (in blank).
	// Here we implement (blank-TAB)=Zs instead of calling u_isblank().
	//
	return (GET_CATEGORY(props) == Util::Enums::toUnderlying(UCharCategory::U_SPACE_SEPARATOR)) || isGraphPOSIX(c);
}

bool isGraph(const char32_t c) {
	uint32_t props;
	GET_PROPS(c, props);
	// Comparing ==0 returns false for the categories mentioned.
	return (CAT_MASK(props) & (gc_cc_mask() | gc_cf_mask() | gc_cs_mask() | gc_cn_mask() | gc_z_mask())) == 0;
}

bool isGraphPOSIX(const char32_t c) {
	uint32_t props;
	GET_PROPS(c, props);
	// \p{space}\p{gc=Control} == \p{gc=Z}\p{Control}
	// comparing ==0 returns false for the categories mentioned
	return (CAT_MASK(props) & (gc_cc_mask() | gc_cs_mask() | gc_cn_mask() | gc_z_mask())) == 0;
}

bool isPunctuation(const char32_t c) {
	uint32_t props;
	GET_PROPS(c, props);
	return (CAT_MASK(props) & gc_p_mask()) != 0;
}

bool isIDStart(const char32_t c) {
	// Same as isAlpha().
	uint32_t props;
	GET_PROPS(c, props);
	return (CAT_MASK(props) & (gc_l_mask() | gc_nl_mask())) != 0;
}

bool isIDPart(const char32_t c) {
	uint32_t props;
	GET_PROPS(c, props);
	return (CAT_MASK(props) & (gc_nd_mask() | gc_nl_mask() | gc_l_mask() | gc_pc_mask() | gc_mc_mask() | gc_mn_mask())) != 0 || isIDIgnorable(c);
}

bool isIDIgnorable(const char32_t c) {
	if (c <= 0x9f) {
		return isISOControlCode(c) && !IS_THAT_ASCII_CONTROL_SPACE(c);
	} else {
		uint32_t props;
		GET_PROPS(c, props);
		return GET_CATEGORY(props) == Util::Enums::toUnderlying(UCharCategory::U_FORMAT_CHAR);
	}
}

bool isJavaIDStart(const char32_t c) {
	uint32_t props;
	GET_PROPS(c, props);
	return (CAT_MASK(props) & (gc_l_mask() | gc_sc_mask() | gc_pc_mask())) != 0;
}

bool isJavaIDPart(const char32_t c) {
	uint32_t props;
	GET_PROPS(c, props);
	return (CAT_MASK(props) & (gc_nd_mask() | gc_nl_mask() | gc_l_mask() | gc_sc_mask() | gc_pc_mask() | gc_mc_mask() | gc_mn_mask())) != 0 || isIDIgnorable(c);
}

int32_t charDigitValue(const char32_t c) {
	uint32_t props;
	int32_t value;
	GET_PROPS(c, props);
	value = (int32_t) GET_NUMERIC_TYPE_VALUE(props) - UPROPS_NTV_DECIMAL_START;

	if (value <= 9) {
		return value;
	} else {
		return -1;
	}
}

int32_t getDigit(const char32_t ch, const int8_t radix) {
	int8_t value;

	if ((uint8_t) (radix - 2) <= (36 - 2)) {
		value = (int8_t) charDigitValue(ch);

		if (value < 0) {
			// ch is not a decimal digit, try latin letters
			if (ch >= 0x61 && ch <= 0x7A) {
				value = (int8_t) (ch - 0x57);  // ch - 'a' + 10
			} else if (ch >= 0x41 && ch <= 0x5A) {
				value = (int8_t) (ch - 0x37);  // ch - 'A' + 10
			} else if (ch >= 0xFF41 && ch <= 0xFF5A) {
				value = (int8_t) (ch - 0xFF37);  // fullwidth ASCII a-z
			} else if (ch >= 0xFF21 && ch <= 0xFF3A) {
				value = (int8_t) (ch - 0xFF17);  // fullwidth ASCII A-Z
			}
		}
	} else {
		value = -1; // invalid radix
	}

	return (int8_t) ((value < radix) ? value : -1);
}

char32_t forDigit(const int32_t digit, const int8_t radix) {
	if ((uint8_t) (radix - 2) > (36 - 2) || (uint32_t) digit >= (uint32_t) radix) {
		return 0;
	} else if (digit < 10) {
		return (char32_t) (0x30 + digit);
	} else {
		return (char32_t) ((0x61 - 10) + digit);
	}
}

uint32_t getMainProperties(const char32_t c) {
	uint32_t props;
	GET_PROPS(c, props);
	return props;
}

uint32_t getUnicodeProperties(const char32_t c, const int32_t column) {
	if (column >= propsVectorsColumns) {
		return 0;
	} else {
		uint16_t vecIndex = uTrie2Get16(&propsVectorsTrie, c);
		return propsVectors[vecIndex + column];
	}
}

int32_t uprv_getMaxValues(const int32_t column) {
	switch (column) {
		case 0: {
			return indexes[UPROPS_MAX_VALUES_INDEX];
		}

		case 2: {
			return indexes[UPROPS_MAX_VALUES_2_INDEX];
		}

		default: {
			return 0;
		}
	}
}

void charAge(const char32_t c, UVersionInfo & versionArray) {
	if (versionArray != nullptr) {
		uint32_t version = getUnicodeProperties(c, 0) >> UPROPS_AGE_SHIFT;
		versionArray[0] = (uint8_t) (version >> 4);
		versionArray[1] = (uint8_t) (version & 0xf);
		versionArray[2] = versionArray[3] = 0;
	}
}

UBlockCode getAllocationBlockCode(const char32_t c) {
	return (UBlockCode) ((getUnicodeProperties(c, 0) & UPROPS_BLOCK_MASK) >> UPROPS_BLOCK_SHIFT);
}

} // namespace Balau::Unicode
