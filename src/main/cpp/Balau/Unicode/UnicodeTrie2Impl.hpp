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

#ifndef COM_BORA_SOFTWARE__BALAU_TYPE_IMPL__UNICODE_TRIE2_IMPL
#define COM_BORA_SOFTWARE__BALAU_TYPE_IMPL__UNICODE_TRIE2_IMPL

#include <Balau/Unicode/UnicodeTrie2.hpp>

namespace Balau::Unicode {

enum {
	///
	/// At build time, leave a gap in the index-2 table,
	/// at least as long as the maximum lengths of the 2-byte UTF-8 index-2 table
	/// and the supplementary index-1 table.
	/// Round up to UTRIE2_INDEX_2_BLOCK_LENGTH for proper compacting.
	///
	UNEWTRIE2_INDEX_GAP_OFFSET = UTRIE2_INDEX_2_BMP_LENGTH,
	UNEWTRIE2_INDEX_GAP_LENGTH = ((UTRIE2_UTF8_2B_INDEX_2_LENGTH + UTRIE2_MAX_INDEX_1_LENGTH) + UTRIE2_INDEX_2_MASK) & ~UTRIE2_INDEX_2_MASK,

	///
	/// Maximum length of the build-time index-2 array.
	/// Maximum number of Unicode code points (0x110000) shifted right by UTRIE2_SHIFT_2,
	/// plus the part of the index-2 table for lead surrogate code points,
	/// plus the build-time index gap,
	/// plus the null index-2 block.
	///
	UNEWTRIE2_MAX_INDEX_2_LENGTH = (0x110000 >> UTRIE2_SHIFT_2) + UTRIE2_LSCP_INDEX_2_LENGTH + UNEWTRIE2_INDEX_GAP_LENGTH + UTRIE2_INDEX_2_BLOCK_LENGTH,

	UNEWTRIE2_INDEX_1_LENGTH = 0x110000 >> UTRIE2_SHIFT_1
};

///
/// Maximum length of the build-time data array.
/// One entry per 0x110000 code points, plus the illegal-UTF-8 block and the null block,
/// plus values for the 0x400 surrogate code units.
///
constexpr unsigned int UNEWTRIE2_MAX_DATA_LENGTH = 0x110000 + 0x40 + 0x40 + 0x400;

///
/// Build-time trie structure.
///
/// Just using a boolean flag for "repeat use" could lead to data array overflow
/// because we would not be able to detect when a data block becomes unused.
/// It also leads to orphan data blocks that are kept through serialization.
///
/// Need to use reference counting for data blocks,
/// and allocDataBlock() needs to look for a free block before increasing dataLength.
///
/// This scheme seems like overkill for index-2 blocks since the whole index array is
/// preallocated anyway (unlike the growable data array).
/// Just allocating multiple index-2 blocks as needed.
///
struct UNewTrie2 {
	int32_t index1[UNEWTRIE2_INDEX_1_LENGTH];
	int32_t index2[UNEWTRIE2_MAX_INDEX_2_LENGTH];
	uint32_t * data;

	uint32_t initialValue, errorValue;
	int32_t index2Length, dataCapacity, dataLength;
	int32_t firstFreeBlock;
	int32_t index2NullOffset, dataNullOffset;
	char32_t highStart;
	bool isCompacted;

	///
	/// Multi-purpose per-data-block table.
	///
	/// Before compacting:
	///
	/// Per-data-block reference counters/free-block list.
	///  0: unused
	/// >0: reference counter (number of index-2 entries pointing here)
	/// <0: next free data block in free-block list
	///
	/// While compacting:
	///
	/// Map of adjusted indexes, used in compactData() and compactIndex2().
	/// Maps from original indexes to new ones.
	///
	int32_t map[UNEWTRIE2_MAX_DATA_LENGTH >> UTRIE2_SHIFT_2];
};

} // namespace Balau::Unicode

#endif // COM_BORA_SOFTWARE__BALAU_TYPE_IMPL__UNICODE_TRIE2_IMPL
