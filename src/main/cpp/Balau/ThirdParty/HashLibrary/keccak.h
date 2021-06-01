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
	public: explicit Keccak(Bits bits = Keccak256)
		: m_numBytes(0U)
		, m_blockSize(200 - 2 * (bits / 8))
		, m_bufferSize(0U)
		, m_bits(bits) {
		reset();
	}

	/// compute hash of a memory block
	public: std::string operator ()(const void * data, size_t numBytes) {
		reset();
		add(data, numBytes);
		return getHash();
	}

	/// compute hash of a string, excluding final zero
	public: std::string operator ()(const std::string & text) {
		reset();
		add(text.c_str(), text.size());
		return getHash();
	}

	/// add arbitrary number of bytes
	public: void add(const void * data, size_t numBytes) {
		const auto * current = (const uint8_t *) data;

		if (m_bufferSize > 0U) {
			while (numBytes > 0U && m_bufferSize < m_blockSize) {
				m_buffer[m_bufferSize++] = *current++;
				numBytes--;
			}
		}

		// full buffer
		if (m_bufferSize == m_blockSize) {
			processBlock((void *) m_buffer);
			m_numBytes += m_blockSize;
			m_bufferSize = 0U;
		}

		// no more data ?
		if (numBytes == 0U) {
			return;
		}

		// process full blocks
		while (numBytes >= m_blockSize) {
			processBlock(current);
			current += m_blockSize;
			m_numBytes += m_blockSize;
			numBytes -= m_blockSize;
		}

		// keep remaining bytes in buffer
		while (numBytes > 0) {
			m_buffer[m_bufferSize++] = *current++;
			numBytes--;
		}
	}

	/// return latest hash as hex characters
	public: std::string getHash() {
		// process remaining bytes
		processBuffer();

		// convert hash to string
		static const char dec2hex[16U + 1U] = "0123456789abcdef";

		// number of significant elements in hash (uint64_t)
		unsigned int hashLength = m_bits / 64U;

		std::string result;

		for (unsigned int i = 0U; i < hashLength; i++) {
			for (unsigned int j = 0U; j < 8U; j++) { // 64 bits => 8 bytes
				// convert a byte to hex
				auto oneByte = (unsigned char) (m_hash[i] >> (8U * j));
				result += dec2hex[oneByte >> 4U];
				result += dec2hex[oneByte & 15U];
			}
		}

		// Keccak224's last entry in m_hash provides only 32 bits instead of 64 bits
		unsigned int remainder = m_bits - hashLength * 64U;
		unsigned int processed = 0U;

		while (processed < remainder) {
			// convert a byte to hex
			auto oneByte = (unsigned char) (m_hash[hashLength] >> processed);
			result += dec2hex[oneByte >> 4U];
			result += dec2hex[oneByte & 15U];

			processed += 8;
		}

		return result;
	}

	/// restart
	public: void reset() {
		for (unsigned long & i : m_hash) {
			i = 0;
		}

		m_numBytes = 0;
		m_bufferSize = 0;
	}

	private: static const unsigned int KeccakRounds = 24;

	private: static const auto & xorMasks() {
		const static uint64_t masks[KeccakRounds] = {
			0x0000000000000001ULL, 0x0000000000008082ULL, 0x800000000000808aULL,
			0x8000000080008000ULL, 0x000000000000808bULL, 0x0000000080000001ULL,
			0x8000000080008081ULL, 0x8000000000008009ULL, 0x000000000000008aULL,
			0x0000000000000088ULL, 0x0000000080008009ULL, 0x000000008000000aULL,
			0x000000008000808bULL, 0x800000000000008bULL, 0x8000000000008089ULL,
			0x8000000000008003ULL, 0x8000000000008002ULL, 0x8000000000000080ULL,
			0x000000000000800aULL, 0x800000008000000aULL, 0x8000000080008081ULL,
			0x8000000000008080ULL, 0x0000000080000001ULL, 0x8000000080008008ULL
		};

		return masks;
	}

	/// process a full block
	private: void processBlock(const void * data) {
		#if defined(__BYTE_ORDER) && (__BYTE_ORDER != 0) && (__BYTE_ORDER == __BIG_ENDIAN)
		#define LITTLEENDIAN(x) swap(x)
		#else
		#define LITTLEENDIAN(x) (x)
		#endif

		const auto * data64 = (const uint64_t *) data;
		const auto & masks = xorMasks();

		// mix data into state
		for (unsigned int i = 0; i < m_blockSize / 8; i++) {
			m_hash[i] ^= LITTLEENDIAN(data64[i]);
		}

		// re-compute state
		for (unsigned long XorMask : masks) {
			// Theta
			uint64_t coefficients[5];
			for (unsigned int i = 0; i < 5; i++) {
				coefficients[i] = m_hash[i] ^ m_hash[i + 5] ^ m_hash[i + 10] ^ m_hash[i + 15] ^ m_hash[i + 20];
			}

			for (unsigned int i = 0; i < 5; i++) {
				uint64_t one = coefficients[mod5(i + 4)] ^rotateLeft(coefficients[mod5(i + 1)], 1);
				m_hash[i] ^= one;
				m_hash[i + 5] ^= one;
				m_hash[i + 10] ^= one;
				m_hash[i + 15] ^= one;
				m_hash[i + 20] ^= one;
			}

			// temporary
			uint64_t one;

			// Rho Pi
			uint64_t last = m_hash[1];
			one = m_hash[10];
			m_hash[10] = rotateLeft(last, 1);

			last = one;
			one = m_hash[7];
			m_hash[7] = rotateLeft(last, 3);

			last = one;
			one = m_hash[11];
			m_hash[11] = rotateLeft(last, 6);

			last = one;
			one = m_hash[17];
			m_hash[17] = rotateLeft(last, 10);

			last = one;
			one = m_hash[18];
			m_hash[18] = rotateLeft(last, 15);

			last = one;
			one = m_hash[3];
			m_hash[3] = rotateLeft(last, 21);

			last = one;
			one = m_hash[5];
			m_hash[5] = rotateLeft(last, 28);

			last = one;
			one = m_hash[16];
			m_hash[16] = rotateLeft(last, 36);

			last = one;
			one = m_hash[8];
			m_hash[8] = rotateLeft(last, 45);

			last = one;
			one = m_hash[21];
			m_hash[21] = rotateLeft(last, 55);

			last = one;
			one = m_hash[24];
			m_hash[24] = rotateLeft(last, 2);

			last = one;
			one = m_hash[4];
			m_hash[4] = rotateLeft(last, 14);

			last = one;
			one = m_hash[15];
			m_hash[15] = rotateLeft(last, 27);

			last = one;
			one = m_hash[23];
			m_hash[23] = rotateLeft(last, 41);

			last = one;
			one = m_hash[19];
			m_hash[19] = rotateLeft(last, 56);

			last = one;
			one = m_hash[13];
			m_hash[13] = rotateLeft(last, 8);

			last = one;
			one = m_hash[12];
			m_hash[12] = rotateLeft(last, 25);

			last = one;
			one = m_hash[2];
			m_hash[2] = rotateLeft(last, 43);

			last = one;
			one = m_hash[20];
			m_hash[20] = rotateLeft(last, 62);

			last = one;
			one = m_hash[14];
			m_hash[14] = rotateLeft(last, 18);

			last = one;
			one = m_hash[22];
			m_hash[22] = rotateLeft(last, 39);

			last = one;
			one = m_hash[9];
			m_hash[9] = rotateLeft(last, 61);

			last = one;
			one = m_hash[6];
			m_hash[6] = rotateLeft(last, 20);

			last = one;
			m_hash[1] = rotateLeft(last, 44);

			// Chi
			for (unsigned int j = 0; j < 25; j += 5) {
				// temporaries
				uint64_t one = m_hash[j];
				uint64_t two = m_hash[j + 1];

				m_hash[j] ^= m_hash[j + 2] & ~two;
				m_hash[j + 1] ^= m_hash[j + 3] & ~m_hash[j + 2];
				m_hash[j + 2] ^= m_hash[j + 4] & ~m_hash[j + 3];
				m_hash[j + 3] ^= one & ~m_hash[j + 4];
				m_hash[j + 4] ^= two & ~one;
			}

			// Iota
			m_hash[0] ^= XorMask;
		}
	}

	/// process everything left in the internal buffer
	private: void processBuffer() {
		unsigned int blockSize = 200U - 2U * (m_bits / 8U);

		// add padding
		size_t offset = m_bufferSize;
		// add a "1" byte
		m_buffer[offset++] = 1U;
		// fill with zeros
		while (offset < blockSize) {
			m_buffer[offset++] = 0U;
		}

		// and add a single set bit
		m_buffer[blockSize - 1] |= 0x80U;

		processBlock(m_buffer);
	}

	/// rotate left and wrap around to the right
	private: static uint64_t rotateLeft(uint64_t x, uint8_t numBits) {
		return (x << numBits) | (x >> (64 - numBits));
	}

	/// convert litte vs big endian
	private: static uint64_t swap(uint64_t x) {
		#if defined(__GNUC__) || defined(__clang__)
		return __builtin_bswap64(x);
		#elif defined _MSC_VER
		return _byteswap_uint64(x);
	#else
		return (x >> 56) |
		       ((x >> 40) & 0x000000000000FF00ULL) |
		       ((x >> 24) & 0x0000000000FF0000ULL) |
		       ((x >> 8) & 0x00000000FF000000ULL) |
		       ((x << 8) & 0x000000FF00000000ULL) |
		       ((x << 24) & 0x0000FF0000000000ULL) |
		       ((x << 40) & 0x00FF000000000000ULL) |
		       (x << 56);
		#endif
	}

	/// return x % 5 for 0 <= x <= 9
	private: static unsigned int mod5(unsigned int x) {
		if (x < 5) {
			return x;
		}

		return x - 5;
	}

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
