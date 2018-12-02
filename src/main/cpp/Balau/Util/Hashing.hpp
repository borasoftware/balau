// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

///
/// @file Hashing.hpp
///
/// %Hashing functions.
///

#ifndef COM_BORA_SOFTWARE__BALAU_UTIL__HASHING
#define COM_BORA_SOFTWARE__BALAU_UTIL__HASHING

#include <Balau/ThirdParty/HashLibrary/crc32.h>
#include <Balau/ThirdParty/HashLibrary/md5.h>
#include <Balau/ThirdParty/HashLibrary/sha1.h>
#include <Balau/ThirdParty/HashLibrary/sha256.h>
#include <Balau/ThirdParty/HashLibrary/sha3.h>
#include <Balau/ThirdParty/HashLibrary/keccak.h>
#include <Balau/Exception/ResourceExceptions.hpp>
#include <Balau/Resource/File.hpp>
#include <Balau/Util/Files.hpp>

namespace Balau::Util {

///
/// %Hashing utilities.
///
struct Hashing final {
	///
	/// Calculate the SHA-256 hash of the specified file.
	///
	/// @throw FileNotFoundException if the file does not exist or is not a regular file
	/// @throw CouldNotOpenException if a stream onto the file could not be opened
	///
	static std::string sha256(const Resource::File & file) {
		auto fileStream = getFileStream(file);
		HashLibrary::SHA256 d;
		return computeHash(*fileStream, d);
	}

	///
	/// Calculate the SHA-256 hash of the data supplied by the stream.
	///
	static std::string sha256(std::istream & stream) {
		HashLibrary::SHA256 d;
		return computeHash(stream, d);
	}

	///
	/// Calculate the SHA-256 hash of the supplied string.
	///
	static std::string sha256(std::string & text) {
		HashLibrary::SHA256 d;
		return computeHash(text, d);
	}

	///
	/// Calculate the SHA-3 hash of the specified file.
	///
	/// @throw FileNotFoundException if the file does not exist or is not a regular file
	/// @throw CouldNotOpenException if a stream onto the file could not be opened
	///
	static std::string sha3(const Resource::File & file) {
		auto fileStream = getFileStream(file);
		HashLibrary::SHA3 d(HashLibrary::SHA3::Bits256);
		return computeHash(*fileStream, d);
	}

	///
	/// Calculate the SHA-3 hash of the data supplied by the stream.
	///
	static std::string sha3(std::istream & stream) {
		HashLibrary::SHA3 d(HashLibrary::SHA3::Bits256);
		return computeHash(stream, d);
	}

	///
	/// Calculate the SHA-3 hash of the supplied string.
	///
	static std::string sha3(std::string & text) {
		HashLibrary::SHA3 d;
		return computeHash(text, d);
	}

	///
	/// Calculate the SHA-1 hash of the specified file.
	///
	/// @throw FileNotFoundException if the file does not exist or is not a regular file
	/// @throw CouldNotOpenException if a stream onto the file could not be opened
	///
	static std::string sha1(const Resource::File & file) {
		auto fileStream = getFileStream(file);
		HashLibrary::SHA1 d;
		return computeHash(*fileStream, d);
	}

	///
	/// Calculate the SHA-1 hash of the data supplied by the stream.
	///
	static std::string sha1(std::istream & stream) {
		HashLibrary::SHA1 d;
		return computeHash(stream, d);
	}

	///
	/// Calculate the SHA-1 hash of the supplied string.
	///
	static std::string sha1(std::string & text) {
		HashLibrary::SHA1 d;
		return computeHash(text, d);
	}

	///
	/// Calculate the Keccak hash of the specified file.
	///
	/// @throw FileNotFoundException if the file does not exist or is not a regular file
	/// @throw CouldNotOpenException if a stream onto the file could not be opened
	///
	static std::string keccak(const Resource::File & file) {
		auto fileStream = getFileStream(file);
		HashLibrary::Keccak d(HashLibrary::Keccak::Keccak256);
		return computeHash(*fileStream, d);
	}

	///
	/// Calculate the Keccak hash of the data supplied by the stream.
	///
	static std::string keccak(std::istream & stream) {
		HashLibrary::Keccak d(HashLibrary::Keccak::Keccak256);
		return computeHash(stream, d);
	}

	///
	/// Calculate the Keccak hash of the supplied string.
	///
	static std::string keccak(std::string & text) {
		HashLibrary::Keccak d(HashLibrary::Keccak::Keccak256);
		return computeHash(text, d);
	}

	///
	/// Calculate the MD5 hash of the specified file.
	///
	/// @throw FileNotFoundException if the file does not exist or is not a regular file
	/// @throw CouldNotOpenException if a stream onto the file could not be opened
	///
	static std::string md5(const Resource::File & file) {
		auto fileStream = getFileStream(file);
		HashLibrary::MD5 d;
		return computeHash(*fileStream, d);
	}

	///
	/// Calculate the MD5 hash of the data supplied by the stream.
	///
	static std::string md5(std::istream & stream) {
		HashLibrary::MD5 d;
		return computeHash(stream, d);
	}

	///
	/// Calculate the MD5 hash of the supplied string.
	///
	static std::string md5(std::string & text) {
		HashLibrary::MD5 d;
		return computeHash(text, d);
	}

	///
	/// Calculate the CRC32 hash of the specified file.
	///
	/// @throw FileNotFoundException if the file does not exist or is not a regular file
	/// @throw CouldNotOpenException if a stream onto the file could not be opened
	///
	static std::string crc32(const Resource::File & file) {
		auto fileStream = getFileStream(file);
		HashLibrary::CRC32 d;
		return computeHash(*fileStream, d);
	}

	///
	/// Calculate the CRC32 hash of the data supplied by the stream.
	///
	static std::string crc32(std::istream & stream) {
		HashLibrary::CRC32 d;
		return computeHash(stream, d);
	}

	///
	/// Calculate the CRC32 hash of the supplied string.
	///
	static std::string crc32(std::string & text) {
		HashLibrary::CRC32 d;
		return computeHash(text, d);
	}

	///////////////////////////////////////////////////////////////////////////

	Hashing() = delete;
	Hashing(const Hashing &) = delete;
	Hashing & operator = (const Hashing &) = delete;

	////////////////////////// Private implementation /////////////////////////

	private: static std::unique_ptr<boost::filesystem::ifstream> getFileStream(const Resource::File & file) {
		if (!file.exists()) {
			ThrowBalauException(Exception::FileNotFoundException, file);
		}

		auto stream = std::make_unique<boost::filesystem::ifstream>(file.getEntry(), std::ios::in | std::ios::binary);

		if (!stream.get()) {
			ThrowBalauException(Exception::CouldNotOpenException, file);
		}

		return stream;
	}

	private: template <typename DigestT> static std::string computeHash(std::istream & input, DigestT & d) {
		std::array<char, 144 * 7 * 1024> buffer {};

		while (input) {
			input.read(buffer.data(), buffer.size());
			auto numBytesRead = size_t(input.gcount());
			d.add(buffer.data(), numBytesRead);
		}

		return d.getHash();
	}

	private: template <typename DigestT> static std::string computeHash(std::string & string, DigestT & d) {
		std::istringstream input(string);
		return computeHash(input, d);
	}
};

} // namespace Balau::Util

#endif // COM_BORA_SOFTWARE__BALAU_UTIL__HASHING
