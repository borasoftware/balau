// //////////////////////////////////////////////////////////
// keccak.h
// Copyright (c) 2014,2015 Stephan Brumme. All rights reserved.
// see http://create.stephan-brumme.com/disclaimer.html
//

#ifndef BALAU_TP__HASH_LIBRARY__KECCAK_H
#define BALAU_TP__HASH_LIBRARY__KECCAK_H

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

/// compute Keccak hash (designated SHA3)
/** Usage:
    Keccak keccak;
    std::string myHash  = keccak("Hello World");     // std::string
    std::string myHash2 = keccak("How are you", 11); // arbitrary data, 11 bytes

    // or in a streaming fashion:

    Keccak keccak;
    while (more data available)
      keccak.add(pointer to fresh data, number of new bytes);
    std::string myHash3 = keccak.getHash();
  */
class Keccak //: public Hash
{
	/// algorithm variants
	public: enum Bits {
		Keccak224 = 224, Keccak256 = 256, Keccak384 = 384, Keccak512 = 512
	};

	/// same as reset()
	public: explicit Keccak(Bits bits = Keccak256);

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
		StateSize = 1600 / (8 * 8), MaxBlockSize = 200 - 2 * (224 / 8)
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

#endif // BALAU_TP__HASH_LIBRARY__KECCAK_H
