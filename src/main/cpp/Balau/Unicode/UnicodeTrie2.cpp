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

#include "UnicodeTrie2Impl.hpp"
#include "UTF32.hpp"

namespace {

template<typename T> inline T minValue(T a, T b) {
	return a < b ? a : b;
}

inline uint32_t enumSameValue(const void *, const uint32_t value) {
	return value;
}

} // namespace

namespace Balau::Unicode {

void utrie2Enum(const UTrie2 * trie, UTrie2EnumValue enumValue, UTrie2EnumRange enumRange, const void * context) {
	//
	// Enumerate all ranges of code points with the same relevant values.
	// The values are transformed from the raw trie entries by the enumValue function.
	//
	// Currently requires start<limit and both start and limit must be multiples
	// of UTRIE2_DATA_BLOCK_LENGTH.
	//
	// Optimizations:
	// - Skip a whole block if we know that it is filled with a single value,
	//   and it is the same as we visited just before.
	// - Handle the null block specially because we know a priori that it is filled
	//   with a single value.
	//

	const char32_t start = 0;
	const char32_t limit = 0x110000;

	const uint32_t * data32;
	const uint16_t * idx;

	uint32_t value, prevValue, initialValue;
	char32_t c, prev, highStart;
	int32_t j, i2Block, prevI2Block, index2NullOffset, block, prevBlock, nullBlock;

	if (enumRange == nullptr) {
		return;
	}

	if (enumValue == nullptr) {
		enumValue = enumSameValue;
	}

	if (trie->newTrie == nullptr) {
		// frozen trie
		idx = trie->index;
		data32 = trie->data32;
		index2NullOffset = trie->index2NullOffset;
		nullBlock = trie->dataNullOffset;
	} else {
		// unfrozen, mutable trie
		idx = nullptr;
		data32 = trie->newTrie->data;
		index2NullOffset = trie->newTrie->index2NullOffset;
		nullBlock = trie->newTrie->dataNullOffset;
	}

	highStart = trie->highStart;

	// get the enumeration value that corresponds to an initial-value trie data entry
	initialValue = enumValue(context, trie->initialValue);

	// set variables for previous range
	prevI2Block = -1;
	prevBlock = -1;
	prev = start;
	prevValue = 0;

	// enumerate index-2 blocks
	for (c = start; c < limit && c < highStart;) {
		// Code point limit for iterating inside this i2Block.
		char32_t tempLimit = c + UTRIE2_CP_PER_INDEX_1_ENTRY;

		if (limit < tempLimit) {
			tempLimit = limit;
		}

		if (c <= 0xffff) {
			if (!isSurrogate(c)) {
				i2Block = c >> UTRIE2_SHIFT_2;
			} else if (isSurrogateLead(c)) {
				//
				// Enumerate values for lead surrogate code points, not code units:
				// This special block has half the normal length.
				//
				i2Block = UTRIE2_LSCP_INDEX_2_OFFSET;
				tempLimit = minValue((char32_t) 0xdc00, limit);
			} else {
				//
				// Switch back to the normal part of the index-2 table.
				// Enumerate the second half of the surrogates block.
				//
				i2Block = 0xd800 >> UTRIE2_SHIFT_2;
				tempLimit = minValue((char32_t) 0xe000, limit);
			}
		} else {
			// supplementary code points
			if (idx != nullptr) {
				i2Block = idx[(UTRIE2_INDEX_1_OFFSET - UTRIE2_OMITTED_BMP_INDEX_1_LENGTH) + (c >> UTRIE2_SHIFT_1)];
			} else {
				i2Block = trie->newTrie->index1[c >> UTRIE2_SHIFT_1];
			}

			if (i2Block == prevI2Block && (c - prev) >= UTRIE2_CP_PER_INDEX_1_ENTRY) {
				//
				// The index-2 block is the same as the previous one, and filled with prevValue.
				// Only possible for supplementary code points because the linear-BMP index-2
				// table creates unique i2Block values.
				//
				c += UTRIE2_CP_PER_INDEX_1_ENTRY;
				continue;
			}
		}

		prevI2Block = i2Block;

		if (i2Block == index2NullOffset) {
			// this is the null index-2 block
			if (prevValue != initialValue) {
				if (prev < c && !enumRange(context, prev, c - 1, prevValue)) {
					return;
				}

				prevBlock = nullBlock;
				prev = c;
				prevValue = initialValue;
			}

			c += UTRIE2_CP_PER_INDEX_1_ENTRY;
		} else {
			// enumerate data blocks for one index-2 block
			int32_t i2, i2Limit;
			i2 = (c >> UTRIE2_SHIFT_2) & UTRIE2_INDEX_2_MASK;

			if ((c >> UTRIE2_SHIFT_1) == (tempLimit >> UTRIE2_SHIFT_1)) {
				i2Limit = (tempLimit >> UTRIE2_SHIFT_2) & UTRIE2_INDEX_2_MASK;
			} else {
				i2Limit = UTRIE2_INDEX_2_BLOCK_LENGTH;
			}

			for (; i2 < i2Limit; ++i2) {
				if (idx != nullptr) {
					block = (int32_t) idx[i2Block + i2] << UTRIE2_INDEX_SHIFT;
				} else {
					block = trie->newTrie->index2[i2Block + i2];
				}

				if (block == prevBlock && (c - prev) >= UTRIE2_DATA_BLOCK_LENGTH) {
					// the block is the same as the previous one, and filled with prevValue
					c += UTRIE2_DATA_BLOCK_LENGTH;
					continue;
				}

				prevBlock = block;

				if (block == nullBlock) {
					/* this is the null data block */
					if (prevValue != initialValue) {
						if (prev < c && !enumRange(context, prev, c - 1, prevValue)) {
							return;
						}

						prev = c;
						prevValue = initialValue;
					}

					c += UTRIE2_DATA_BLOCK_LENGTH;
				} else {
					for (j = 0; j < UTRIE2_DATA_BLOCK_LENGTH; ++j) {
						value = enumValue(context, data32 != nullptr ? data32[block + j] : idx[block + j]);

						if (value != prevValue) {
							if (prev < c && !enumRange(context, prev, c - 1, prevValue)) {
								return;
							}

							prev = c;
							prevValue = value;
						}

						++c;
					}
				}
			}
		}
	}

	if (c > limit) {
		c = limit;  // could be higher if in the index2NullOffset
	} else if (c < limit) {
		// c == highStart < limit
		uint32_t highValue;

		if (idx != nullptr) {
			highValue = data32 != nullptr ? data32[trie->highValueIndex] : idx[trie->highValueIndex];
		} else {
			highValue = trie->newTrie->data[trie->newTrie->dataLength - UTRIE2_DATA_GRANULARITY];
		}

		value = enumValue(context, highValue);

		if (value != prevValue) {
			if (prev < c && !enumRange(context, prev, c - 1, prevValue)) {
				return;
			}

			prev = c;
			prevValue = value;
		}

		c = limit;
	}

	// deliver last range
	enumRange(context, prev, c - 1, prevValue);
}

} // namespace Balau::Unicode
