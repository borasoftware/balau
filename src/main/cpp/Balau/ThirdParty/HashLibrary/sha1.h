// //////////////////////////////////////////////////////////
// sha1.h
// Copyright (c) 2014,2015 Stephan Brumme. All rights reserved.
// see http://create.stephan-brumme.com/disclaimer.html
//

#ifndef BALAU_TP__HASH_LIBRARY__SHA1_H
#define BALAU_TP__HASH_LIBRARY__SHA1_H

//#include "hash.h"
#include <string>

// define fixed size integer types
#ifdef _MSC_VER
	// Windows
	typedef unsigned __int8  uint8_t;
	typedef unsigned __int32 uint32_t;
	typedef unsigned __int64 uint64_t;
#else
	// GCC
	#include <stdint.h>
#endif

namespace Balau::HashLibrary {

/// compute SHA1 hash
/** Usage:
    SHA1 sha1;
    std::string myHash  = sha1("Hello World");     // std::string
    std::string myHash2 = sha1("How are you", 11); // arbitrary data, 11 bytes

    // or in a streaming fashion:

    SHA1 sha1;
    while (more data available)
      sha1.add(pointer to fresh data, number of new bytes);
    std::string myHash3 = sha1.getHash();
  */
class SHA1 //: public Hash
{
	/// split into 64 byte blocks (=> 512 bits), hash is 20 bytes long
	public: enum {
		BlockSize = 512 / 8, HashBytes = 20
	};

	/// same as reset()
	public: SHA1();

	/// compute SHA1 of a memory block
	public: std::string operator () (const void * data, size_t numBytes);

	/// compute SHA1 of a string, excluding final zero
	public: std::string operator () (const std::string & text);

	/// add arbitrary number of bytes
	public: void add(const void * data, size_t numBytes);

	/// return latest hash as 40 hex characters
	public: std::string getHash();

	/// return latest hash as bytes
	public: void getHash(unsigned char buffer[HashBytes]);

	/// restart
	public: void reset();

	/// process 64 bytes
	private: void processBlock(const void * data);

	/// process everything left in the internal buffer
	private: void processBuffer();

	/// size of processed data in bytes
	private: uint64_t m_numBytes;

	/// valid bytes in m_buffer
	private: size_t m_bufferSize;

	/// bytes not processed yet
	private: uint8_t m_buffer[BlockSize] { 0U };

	private: enum {
		HashValues = HashBytes / 4
	};

	/// hash, stored as integers
	private: uint32_t m_hash[HashValues] { 0U };
};

} // namespace Balau::HashLibrary

#endif // BALAU_TP__HASH_LIBRARY__SHA1_H
