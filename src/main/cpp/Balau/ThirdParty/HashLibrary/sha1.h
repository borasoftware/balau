// //////////////////////////////////////////////////////////
// sha1.h
// Copyright (c) 2014,2015 Stephan Brumme. All rights reserved.
// see http://create.stephan-brumme.com/disclaimer.html
//

#ifndef BALAU_TP__HASH_LIBRARY__SHA1_H
#define BALAU_TP__HASH_LIBRARY__SHA1_H

#include <string>

// define fixed size integer types
#ifdef _MSC_VER
	// Windows
	typedef unsigned __int8  uint8_t;
	typedef unsigned __int32 uint32_t;
	typedef unsigned __int64 uint64_t;

	#include <endian.h>
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
	public: SHA1()
		: m_numBytes(0)
		, m_bufferSize(0) {
		reset();
	}

	/// compute SHA1 of a memory block
	public: std::string operator () (const void * data, size_t numBytes) {
		reset();
		add(data, numBytes);
		return getHash();
	}

	/// compute SHA1 of a string, excluding final zero
	public: std::string operator () (const std::string & text) {
		reset();
		add(text.c_str(), text.size());
		return getHash();
	}

	/// add arbitrary number of bytes
	public: void add(const void * data, size_t numBytes) {
		const auto * current = (const uint8_t *) data;

		if (m_bufferSize > 0) {
			while (numBytes > 0 && m_bufferSize < BlockSize) {
				m_buffer[m_bufferSize++] = *current++;
				numBytes--;
			}
		}

		// full buffer
		if (m_bufferSize == BlockSize) {
			processBlock((void *) m_buffer);
			m_numBytes += BlockSize;
			m_bufferSize = 0;
		}

		// no more data ?
		if (numBytes == 0) {
			return;
		}

		// process full blocks
		while (numBytes >= BlockSize) {
			processBlock(current);
			current += BlockSize;
			m_numBytes += BlockSize;
			numBytes -= BlockSize;
		}

		// keep remaining bytes in buffer
		while (numBytes > 0) {
			m_buffer[m_bufferSize++] = *current++;
			numBytes--;
		}
	}

	/// return latest hash as 40 hex characters
	public: std::string getHash() {
		// compute hash (as raw bytes)
		unsigned char rawHash[HashBytes];
		getHash(rawHash);

		// convert to hex string
		std::string result;
		result.reserve(2 * HashBytes);

		for (unsigned int i : rawHash) {
			static const char dec2hex[16U + 1U] = "0123456789abcdef";
			result += dec2hex[(i >> 4U) & 15U];
			result += dec2hex[i & 15U];
		}

		return result;
	}

	/// return latest hash as bytes
	public: void getHash(unsigned char buffer[HashBytes]) {
		// save old hash if buffer is partially filled
		uint32_t oldHash[HashValues];

		for (int i = 0; i < HashValues; i++) {
			oldHash[i] = m_hash[i];
		}

		// process remaining bytes
		processBuffer();

		unsigned char * current = buffer;

		for (size_t i = 0; i < HashValues; i++) {
			*current++ = (m_hash[i] >> 24U) & 0xFFU;
			*current++ = (m_hash[i] >> 16U) & 0xFFU;
			*current++ = (m_hash[i] >> 8U) & 0xFFU;
			*current++ = m_hash[i] & 0xFFU;

			// restore old hash
			m_hash[i] = oldHash[i];
		}
	}

	/// restart
	public: void reset() {
		m_numBytes = 0;
		m_bufferSize = 0;

		// according to RFC 1321
		m_hash[0] = 0x67452301;
		m_hash[1] = 0xefcdab89;
		m_hash[2] = 0x98badcfe;
		m_hash[3] = 0x10325476;
		m_hash[4] = 0xc3d2e1f0;
	}

	/// process 64 bytes
	private: void processBlock(const void * data) {
		// get last hash
		uint32_t a = m_hash[0];
		uint32_t b = m_hash[1];
		uint32_t c = m_hash[2];
		uint32_t d = m_hash[3];
		uint32_t e = m_hash[4];

		// data represented as 16x 32-bit words
		const uint32_t * input = (uint32_t *) data;

		// convert to big endian
		uint32_t words[80];

		for (int i = 0; i < 16; i++) {
			#if defined(__BYTE_ORDER) && (__BYTE_ORDER != 0) && (__BYTE_ORDER == __BIG_ENDIAN)
			words[i] = input[i];
			#else
			words[i] = swap(input[i]);
			#endif
		}

		// extend to 80 words
		for (int i = 16; i < 80; i++) {
			words[i] = rotate(words[i - 3] ^ words[i - 8] ^ words[i - 14] ^ words[i - 16], 1);
		}

		// first round
		for (int i = 0; i < 4; i++) {
			int offset = 5 * i;
			e += rotate(a, 5) + f1(b, c, d) + words[offset] + 0x5a827999;
			b = rotate(b, 30);
			d += rotate(e, 5) + f1(a, b, c) + words[offset + 1] + 0x5a827999;
			a = rotate(a, 30);
			c += rotate(d, 5) + f1(e, a, b) + words[offset + 2] + 0x5a827999;
			e = rotate(e, 30);
			b += rotate(c, 5) + f1(d, e, a) + words[offset + 3] + 0x5a827999;
			d = rotate(d, 30);
			a += rotate(b, 5) + f1(c, d, e) + words[offset + 4] + 0x5a827999;
			c = rotate(c, 30);
		}

		// second round
		for (int i = 4; i < 8; i++) {
			int offset = 5 * i;
			e += rotate(a, 5) + f2(b, c, d) + words[offset] + 0x6ed9eba1;
			b = rotate(b, 30);
			d += rotate(e, 5) + f2(a, b, c) + words[offset + 1] + 0x6ed9eba1;
			a = rotate(a, 30);
			c += rotate(d, 5) + f2(e, a, b) + words[offset + 2] + 0x6ed9eba1;
			e = rotate(e, 30);
			b += rotate(c, 5) + f2(d, e, a) + words[offset + 3] + 0x6ed9eba1;
			d = rotate(d, 30);
			a += rotate(b, 5) + f2(c, d, e) + words[offset + 4] + 0x6ed9eba1;
			c = rotate(c, 30);
		}

		// third round
		for (int i = 8; i < 12; i++) {
			int offset = 5 * i;
			e += rotate(a, 5) + f3(b, c, d) + words[offset] + 0x8f1bbcdc;
			b = rotate(b, 30);
			d += rotate(e, 5) + f3(a, b, c) + words[offset + 1] + 0x8f1bbcdc;
			a = rotate(a, 30);
			c += rotate(d, 5) + f3(e, a, b) + words[offset + 2] + 0x8f1bbcdc;
			e = rotate(e, 30);
			b += rotate(c, 5) + f3(d, e, a) + words[offset + 3] + 0x8f1bbcdc;
			d = rotate(d, 30);
			a += rotate(b, 5) + f3(c, d, e) + words[offset + 4] + 0x8f1bbcdc;
			c = rotate(c, 30);
		}

		// fourth round
		for (int i = 12; i < 16; i++) {
			int offset = 5 * i;
			e += rotate(a, 5) + f2(b, c, d) + words[offset] + 0xca62c1d6;
			b = rotate(b, 30);
			d += rotate(e, 5) + f2(a, b, c) + words[offset + 1] + 0xca62c1d6;
			a = rotate(a, 30);
			c += rotate(d, 5) + f2(e, a, b) + words[offset + 2] + 0xca62c1d6;
			e = rotate(e, 30);
			b += rotate(c, 5) + f2(d, e, a) + words[offset + 3] + 0xca62c1d6;
			d = rotate(d, 30);
			a += rotate(b, 5) + f2(c, d, e) + words[offset + 4] + 0xca62c1d6;
			c = rotate(c, 30);
		}

		// update hash
		m_hash[0] += a;
		m_hash[1] += b;
		m_hash[2] += c;
		m_hash[3] += d;
		m_hash[4] += e;
	}

	/// process everything left in the internal buffer
	private: void processBuffer() {
		// the input bytes are considered as bits strings, where the
		// first bit is the most significant bit of the byte

		// - append "1" bit to message
		// - append "0" bits until message length in bit mod 512 is 448
		// - append length as 64 bit integer

		// number of bits
		size_t paddedLength = m_bufferSize * 8U;

		// plus one bit set to 1 (always appended)
		paddedLength++;

		// number of bits must be (numBits % 512) = 448
		size_t lower11Bits = paddedLength & 511U;

		if (lower11Bits <= 448U) {
			paddedLength += 448U - lower11Bits;
		} else {
			paddedLength += 512U + 448U - lower11Bits;
		}

		// convert from bits to bytes
		paddedLength /= 8U;

		// only needed if additional data flows over into a second block
		unsigned char extra[BlockSize];

		// append a "1" bit, 128 => binary 10000000
		if (m_bufferSize < BlockSize) {
			m_buffer[m_bufferSize] = 128U;
		} else {
			extra[0] = 128U;
		}

		size_t i;

		for (i = m_bufferSize + 1; i < BlockSize; i++) {
			m_buffer[i] = 0;
		}

		for (; i < paddedLength; i++) {
			extra[i - BlockSize] = 0;
		}

		// add message length in bits as 64 bit number
		uint64_t msgBits = 8 * (m_numBytes + m_bufferSize);

		// find right position
		unsigned char * addLength;

		if (paddedLength < BlockSize) {
			addLength = m_buffer + paddedLength;
		} else {
			addLength = extra + paddedLength - BlockSize;
		}

		// must be big endian
		*addLength++ = (unsigned char) ((msgBits >> 56U) & 0xFFU);
		*addLength++ = (unsigned char) ((msgBits >> 48U) & 0xFFU);
		*addLength++ = (unsigned char) ((msgBits >> 40U) & 0xFFU);
		*addLength++ = (unsigned char) ((msgBits >> 32U) & 0xFFU);
		*addLength++ = (unsigned char) ((msgBits >> 24U) & 0xFFU);
		*addLength++ = (unsigned char) ((msgBits >> 16U) & 0xFFU);
		*addLength++ = (unsigned char) ((msgBits >> 8U) & 0xFFU);
		*addLength   = (unsigned char) (msgBits & 0xFFU);

		// process blocks
		processBlock(m_buffer);

		// flowed over into a second block ?
		if (paddedLength > BlockSize) {
			processBlock(extra);
		}
	}

	// mix functions for processBlock()
	private: static uint32_t f1(uint32_t b, uint32_t c, uint32_t d) {
		return d ^ (b & (c ^ d)); // original: f = (b & c) | ((~b) & d);
	}

	private: static uint32_t f2(uint32_t b, uint32_t c, uint32_t d) {
		return b ^ c ^ d;
	}

	private: static uint32_t f3(uint32_t b, uint32_t c, uint32_t d) {
		return (b & c) | (b & d) | (c & d);
	}

	private: static uint32_t rotate(uint32_t a, uint32_t c) {
		return (a << c) | (a >> (32 - c));
	}

	private: static uint32_t swap(uint32_t x) {
		#if defined(__GNUC__) || defined(__clang__)
			return __builtin_bswap32(x);
		#elif defined MSC_VER
			return _byteswap_ulong(x);
		#else
			return (x >> 24) |
			       ((x >> 8) & 0x0000FF00) |
			       ((x << 8) & 0x00FF0000) |
			       (x << 24);
		#endif
	}

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
