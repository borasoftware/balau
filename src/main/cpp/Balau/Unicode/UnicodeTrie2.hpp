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

#ifndef COM_BORA_SOFTWARE__BALAU_TYPE_IMPL__UNICODE_TRIE2
#define COM_BORA_SOFTWARE__BALAU_TYPE_IMPL__UNICODE_TRIE2

#include <Balau/Unicode/UnicodeErrorCode.hpp>
#include <Balau/Unicode/UTF16.hpp>

namespace Balau::Unicode {

struct UTrie;
struct UTrie2;

///
/// Selectors for the width of a UTrie2 data value.
///
enum UTrie2ValueBits {
	///
	/// 16 bits per UTrie2 data value.
	///
	UTRIE2_16_VALUE_BITS,

	///
	/// 32 bits per UTrie2 data value.
	///
	UTRIE2_32_VALUE_BITS,

	///
	/// Number of selectors for the width of UTrie2 data values.
	///
	UTRIE2_COUNT_VALUE_BITS
};

///
/// Callback from utrie2Enum(), extracts a uint32_t value from a
/// trie value. This value will be passed on to the UTrie2EnumRange function.
///
/// @param context an opaque pointer, as passed into utrie2Enum()
/// @param value a value from the trie
/// @return the value that is to be passed on to the UTrie2EnumRange function
///
typedef uint32_t (* UTrie2EnumValue)(const void * context, uint32_t value);

///
/// Callback from utrie2Enum(), is called for each contiguous range
/// of code points with the same value as retrieved from the trie and
/// transformed by the UTrie2EnumValue function.
///
/// The callback function can stop the enumeration by returning false.
///
/// @param context an opaque pointer, as passed into utrie2Enum()
/// @param start the first code point in a contiguous range with value
/// @param end the last code point in a contiguous range with value (inclusive)
/// @param value the value that is set for all code points in [start..end]
/// @return false to stop the enumeration
///
typedef bool (* UTrie2EnumRange)(const void * context, char32_t start, char32_t end, uint32_t value);

///
/// Enumerate efficiently all values in a trie.
/// Do not modify the trie during the enumeration.
///
/// For each entry in the trie, the value to be delivered is passed through
/// the UTrie2EnumValue function.
/// The value is unchanged if that function pointer is NULL.
///
/// For each contiguous range of code points with a given (transformed) value,
/// the UTrie2EnumRange function is called.
///
/// @param trie a pointer to the trie
///
/// @param enumValue a pointer to a function that may transform the trie entry value,
///                  or NULL if the values from the trie are to be used directly
///
/// @param enumRange a pointer to a function that is called for each contiguous range
///                  of code points with the same (transformed) value
///
/// @param context an opaque pointer that is passed on to the callback functions
///
void utrie2Enum(const UTrie2 * trie, UTrie2EnumValue enumValue, UTrie2EnumRange enumRange, const void * context);

// Use the Forward/Backward subclasses below.
class UTrie2StringIterator {
	public:
	UTrie2StringIterator(const UTrie2 * t, const char16_t * p)
		: trie(t)
		  , codePointStart(p)
		  , codePointLimit(p)
		  , codePoint(UnicodeSentinel) {
	}

	const UTrie2 * trie;
	const char16_t * codePointStart, * codePointLimit;
	char32_t codePoint;
};

struct UNewTrie2;

///
/// Trie structure definition.
///
/// Either the data table is 16 bits wide and accessed via the index
/// pointer, with each index item increased by indexLength;
/// in this case, data32==NULL, and data16 is used for direct ASCII access.
///
/// Or the data table is 32 bits wide and accessed via the data32 pointer.
///
struct UTrie2 {
	//
	// protected: used by macros and functions for reading values
	//
	const uint16_t * index;

	///
	/// for fast UTF-8 ASCII access, if 16b data
	///
	const uint16_t * data16;

	///
	/// NULL if 16b data is used via index
	///
	const uint32_t * data32;

	int32_t indexLength, dataLength;

	///
	/// 0xffff if there is no dedicated index-2 null block
	///
	uint16_t index2NullOffset;

	uint16_t dataNullOffset;

	uint32_t initialValue;

	///
	/// Value returned for out-of-range code points and illegal UTF-8.
	///
	uint32_t errorValue;

	///
	/// Start of the last range which ends at U+10ffff, and its value.
	///
	char32_t highStart;

	int32_t highValueIndex;

	//
	// private: used by builder and unserialization functions
	// serialized bytes; NULL if not frozen yet
	//
	void * memory;

	///
	/// number of serialized bytes at memory; 0 if not frozen yet
	///
	int32_t length;

	///
	/// true if the trie owns the memory
	///
	bool isMemoryOwned;

	int8_t padding1;

	int16_t padding2;

	///
	/// builder object; NULL when frozen
	///
	UNewTrie2 * newTrie;
};

///
/// Trie constants, defining shift widths, index array lengths, etc.
///
/// These are needed for the runtime macros but users can treat these as
/// implementation details and skip to the actual public API further below.
///
enum {
	///
	/// Shift size for getting the index-1 table offset.
	///
	UTRIE2_SHIFT_1 = 6 + 5,

	///
	/// Shift size for getting the index-2 table offset.
	///
	UTRIE2_SHIFT_2 = 5,

	///
	/// Difference between the two shift sizes,
	/// for getting an index-1 offset from an index-2 offset. 6=11-5
	///
	UTRIE2_SHIFT_1_2 = UTRIE2_SHIFT_1 - UTRIE2_SHIFT_2,

	///
	/// Number of index-1 entries for the BMP. 32=0x20
	/// This part of the index-1 table is omitted from the serialized form.
	///
	UTRIE2_OMITTED_BMP_INDEX_1_LENGTH = 0x10000 >> UTRIE2_SHIFT_1,

	///
	/// Number of code points per index-1 table entry. 2048=0x800
	///
	UTRIE2_CP_PER_INDEX_1_ENTRY = 1 << UTRIE2_SHIFT_1,

	///
	/// Number of entries in an index-2 block. 64=0x40
	///
	UTRIE2_INDEX_2_BLOCK_LENGTH = 1 << UTRIE2_SHIFT_1_2,

	///
	/// Mask for getting the lower bits for the in-index-2-block offset.
	///
	UTRIE2_INDEX_2_MASK = UTRIE2_INDEX_2_BLOCK_LENGTH - 1,

	///
	/// Number of entries in a data block. 32=0x20
	///
	UTRIE2_DATA_BLOCK_LENGTH = 1 << UTRIE2_SHIFT_2,

	///
	/// Mask for getting the lower bits for the in-data-block offset.
	///
	UTRIE2_DATA_MASK = UTRIE2_DATA_BLOCK_LENGTH - 1,

	///
	/// Shift size for shifting left the index array values.
	/// Increases possible data size with 16-bit index values at the cost
	/// of compactability.
	/// This requires data blocks to be aligned by UTRIE2_DATA_GRANULARITY.
	///
	UTRIE2_INDEX_SHIFT = 2,

	///
	/// The alignment size of a data block. Also the granularity for compaction.
	///
	UTRIE2_DATA_GRANULARITY = 1 << UTRIE2_INDEX_SHIFT,

	// Fixed layout of the first part of the index array.

	///
	/// The BMP part of the index-2 table is fixed and linear and starts at offset 0.
	/// Length=2048=0x800=0x10000>>UTRIE2_SHIFT_2.
	///
	UTRIE2_INDEX_2_OFFSET = 0,

	///
	/// The part of the index-2 table for U+D800..U+DBFF stores values for
	/// lead surrogate code _units_ not code _points_.
	/// Values for lead surrogate code _points_ are indexed with this portion of the table.
	/// Length=32=0x20=0x400>>UTRIE2_SHIFT_2. (There are 1024=0x400 lead surrogates.)
	///
	UTRIE2_LSCP_INDEX_2_OFFSET = 0x10000 >> UTRIE2_SHIFT_2,

	UTRIE2_LSCP_INDEX_2_LENGTH = 0x400 >> UTRIE2_SHIFT_2,

	///
	/// Count the lengths of both BMP pieces. 2080=0x820
	///
	UTRIE2_INDEX_2_BMP_LENGTH = UTRIE2_LSCP_INDEX_2_OFFSET + UTRIE2_LSCP_INDEX_2_LENGTH,

	///
	/// The 2-byte UTF-8 version of the index-2 table follows at offset 2080=0x820.
	/// Length 32=0x20 for lead bytes C0..DF, regardless of UTRIE2_SHIFT_2.
	///
	UTRIE2_UTF8_2B_INDEX_2_OFFSET = UTRIE2_INDEX_2_BMP_LENGTH,

	UTRIE2_UTF8_2B_INDEX_2_LENGTH = 0x800 >> 6,  // U+0800 is the first code point after 2-byte UTF-8

	///
	/// The index-1 table, only used for supplementary code points, at offset 2112=0x840.
	/// Variable length, for code points up to highStart, where the last single-value range starts.
	/// Maximum length 512=0x200=0x100000>>UTRIE2_SHIFT_1.
	/// (For 0x100000 supplementary code points U+10000..U+10ffff.)
	///
	/// The part of the index-2 table for supplementary code points starts
	/// after this index-1 table.
	///
	/// Both the index-1 table and the following part of the index-2 table
	/// are omitted completely if there is only BMP data.
	///
	UTRIE2_INDEX_1_OFFSET = UTRIE2_UTF8_2B_INDEX_2_OFFSET + UTRIE2_UTF8_2B_INDEX_2_LENGTH,
	UTRIE2_MAX_INDEX_1_LENGTH = 0x100000 >> UTRIE2_SHIFT_1,

	// Fixed layout of the first part of the data array. Starts with 4 blocks (128=0x80 entries) for ASCII.

	///
	/// The illegal-UTF-8 data block follows the ASCII block, at offset 128=0x80.
	/// Used with linear access for single bytes 0..0xbf for simple error handling.
	/// Length 64=0x40, not UTRIE2_DATA_BLOCK_LENGTH.
	///
	UTRIE2_BAD_UTF8_DATA_OFFSET = 0x80,

	///
	/// The start of non-linear-ASCII data blocks, at offset 192=0xc0.
	///
	UTRIE2_DATA_START_OFFSET = 0xc0
};

///
/// Internal low-level trie getter. Returns a data index.
///
inline int32_t uTrie2IndexRaw_(int32_t offset, const uint16_t * trieIndex, char32_t c) {
	return ((int32_t) (trieIndex[offset + (c >> UTRIE2_SHIFT_2)]) << UTRIE2_INDEX_SHIFT) + (c & UTRIE2_DATA_MASK);
}

///
/// Internal trie getter from a supplementary code point below highStart. Returns the data index.
///
inline int32_t uTrie2IndexFromSupp_(const uint16_t * trieIndex, char32_t c) {
	const auto i1 = trieIndex[(UTRIE2_INDEX_1_OFFSET - UTRIE2_OMITTED_BMP_INDEX_1_LENGTH) + (c >> UTRIE2_SHIFT_1)];
	return ((int32_t) (trieIndex[i1 + ((c >> UTRIE2_SHIFT_2) & UTRIE2_INDEX_2_MASK)]) << UTRIE2_INDEX_SHIFT) + (c & UTRIE2_DATA_MASK);
}

///
/// Internal trie getter from a code point, with checking that c is in 0..10FFFF.
/// Returns the data index.
///
inline int32_t uTrie2IndexFromCp_(const UTrie2 * trie, int32_t asciiOffset, char32_t c) {
	const auto cc = (uint32_t) c;

	return cc < 0xd800
		? uTrie2IndexRaw_(0, trie->index, c)
		: cc <= 0xffff
		? uTrie2IndexRaw_(c <= 0xdbff ? UTRIE2_LSCP_INDEX_2_OFFSET - (0xd800 >> UTRIE2_SHIFT_2) : 0, trie->index, c)
		: cc > 0x10ffff
		? asciiOffset + UTRIE2_BAD_UTF8_DATA_OFFSET
		: c >= trie->highStart
		? trie->highValueIndex
		: uTrie2IndexFromSupp_(trie->index, c);
}

///
/// Internal trie getter from a code point, with checking that c is in 0..10FFFF.
/// Returns the data.
///
inline uint16_t uTrie2Get_(const UTrie2 * trie, const uint16_t * data, int32_t asciiOffset, char32_t c) {
	return data[uTrie2IndexFromCp_(trie, asciiOffset, c)];
}

///
/// Return a 16-bit trie value from a code point, with range checking.
/// Returns trie->errorValue if c is not in the range 0..U+10ffff.
///
/// @param trie (const UTrie2 *, in) a frozen trie
/// @param c (char32_t, in) the input code point
/// @return (uint16_t) The code point's trie value.
///
inline uint16_t uTrie2Get16(const UTrie2 * trie, char32_t c) {
	return trie->index[uTrie2IndexFromCp_(trie, trie->indexLength, (uint32_t) c)];
}

} // namespace Balau::Unicode

#endif // COM_BORA_SOFTWARE__BALAU_TYPE_IMPL__UNICODE_TRIE2
