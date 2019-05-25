// //////////////////////////////////////////////////////////
// sha3.h
// Copyright (c) 2014,2015 Stephan Brumme. All rights reserved.
// see http://create.stephan-brumme.com/disclaimer.html
//

#ifndef BALAU_TP__HASH_LIBRARY__SHA3_H
#define BALAU_TP__HASH_LIBRARY__SHA3_H

//#include "hash.h"
#include <string>

// define fixed size integer types
#ifdef _MSC_VER
// Windows
typedef unsigned __int8  uint8_t;
typedef unsigned __int64 uint64_t;
#else
// GCC
#include <stdint.h>

#endif

namespace Balau::HashLibrary {

/// compute SHA3 hash
/** Usage:
    SHA3 sha3;
    std::string myHash  = sha3("Hello World");     // std::string
    std::string myHash2 = sha3("How are you", 11); // arbitrary data, 11 bytes

    // or in a streaming fashion:

    SHA3 sha3;
    while (more data available)
      sha3.add(pointer to fresh data, number of new bytes);
    std::string myHash3 = sha3.getHash();
  */
class SHA3 //: public Hash
{
	/// algorithm variants
	public: enum Bits {
		  Bits224 = 224
		, Bits256 = 256
		, Bits384 = 384
		, Bits512 = 512
	};

	/// same as reset()
	public: explicit SHA3(Bits bits = Bits256);

	/// compute hash of a memory block
	public: std::string operator ()(const void * data, size_t numBytes);

	/// compute hash of a string, excluding final zero
	public: std::string operator ()(const std::string & text);

	/// add arbitrary number of bytes
	public: void add(const void * data, size_t numBytes);

	/// return latest hash as hex characters
	public: std::string getHash();

	/// restart
	public: void reset();

	/// process a full block
	private: void processBlock(const void * data);

	/// process everything left in the internal buffer
	private: void processBuffer();

	/// 1600 bits, stored as 25x64 bit, BlockSize is no more than 1152 bits (Keccak224)
	private: enum {
		  StateSize = 1600 / (8 * 8)
		, MaxBlockSize = 200 - 2 * (224 / 8)
	};

	/// hash
	private: uint64_t m_hash[StateSize] { 0 };

	/// size of processed data in bytes
	private: uint64_t m_numBytes;

	/// block size (less or equal to MaxBlockSize)
	private: size_t m_blockSize;

	/// valid bytes in m_buffer
	private: size_t m_bufferSize;

	/// bytes not processed yet
	private: uint8_t m_buffer[MaxBlockSize] { 0 };

	/// variant
	private: Bits m_bits;
};

} // namespace Balau::HashLibrary

#endif // BALAU_TP__HASH_LIBRARY__SHA3_H
