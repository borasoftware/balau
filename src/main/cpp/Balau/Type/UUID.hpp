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

///
/// @file UUID.hpp
///
/// %UUID class, using the Boost uuid implementation.
///

#ifndef COM_BORA_SOFTWARE__BALAU_TYPE__UUID
#define COM_BORA_SOFTWARE__BALAU_TYPE__UUID

#include <Balau/Util/Random.hpp>
#include <Balau/Type/ToString.hpp>

#include <iomanip>
#include <cstring>

namespace Balau {

///
/// %UUID class, using the Boost uuid implementation.
///
class UUID {
	///
	/// Construct a new UUID with a random value.
	///
	public: UUID() : uuid(generateUUID()) {}

	///
	/// Construct a new UUID from the supplied array.
	///
	/// @param bytes exactly 16 bytes of data
	///
	public: UUID(const unsigned char * bytes) : uuid(generateUUID(bytes)) {}

	///
	/// Construct a new UUID by copying the supplied instance.
	///
	public: UUID(const UUID & copy) = default;

	///
	/// Get the UUID as a std::array of bytes.
	///
	/// This allocates a std::array object.
	///
	public: const std::array<unsigned char, 16> & asArray() const {
		return uuid;
	}

	///
	/// Get the UUID as raw bytes.
	///
	/// This returns the bytes directly. No object construction is performed.
	///
	public: const unsigned char * asBytes() const {
		return uuid.data();
	}

	///
	/// Get the UUID as a UTF-8 string.
	///
	/// This allocates a string stream and string object.
	///
	public: template <typename AllocatorT> Balau::U8String<AllocatorT> asString() const;

	///
	/// Get the UUID as a UTF-8 string.
	///
	/// This allocates a string stream and string object.
	///
	public: std::string asString() const;

	///
	/// Are the two UUIDs equal?
	///
	public: bool operator == (const UUID & rhs) const {
		return uuid == rhs.uuid;
	}

	///
	/// Are the two UUIDs not equal?
	///
	public: bool operator != (const UUID & rhs) const {
		return uuid != rhs.uuid;
	}

	///
	/// Is this UUID less than the supplied UUID?
	///
	public: bool operator < (const UUID & rhs) const {
		return uuid < rhs.uuid;
	}

	///
	/// Is this UUID greater than the supplied UUID?
	///
	public: bool operator > (const UUID & rhs) const {
		return uuid > rhs.uuid;
	}

	///
	/// Is this UUID less or equal to the supplied UUID?
	///
	public: bool operator <= (const UUID & rhs) const {
		return uuid <= rhs.uuid;
	}

	///
	/// Is this UUID greater or equal to the supplied UUID?
	///
	public: bool operator >= (const UUID & rhs) const {
		return uuid >= rhs.uuid;
	}

	////////////////////////// Private implementation /////////////////////////

	private: static std::array<unsigned char, 16> generateUUID() {
		Util::UniformInt<unsigned char> r;
		std::array<unsigned char, 16> ret;
		std::generate(ret.begin(), ret.end(), [&r] () { return r(); });
		return ret;
	}

	private: static std::array<unsigned char, 16> generateUUID(const unsigned char * bytes) {
		std::array<unsigned char, 16> ret;
		std::memcpy(ret.data(), bytes, 16);
		return ret;
	}

	private: const std::array<uint8_t, 16> uuid;

	friend std::ostream & operator << (std::ostream &, const UUID &);
	template <typename AllocatorT> friend std::basic_ostringstream<char, std::char_traits<char>, AllocatorT> & operator << (std::basic_ostringstream<char, std::char_traits<char>, AllocatorT> &, const UUID &);
};

template <typename AllocatorT>
inline std::basic_ostringstream<char, std::char_traits<char>, AllocatorT> & operator << (std::basic_ostringstream<char, std::char_traits<char>, AllocatorT> & stream, const UUID & uuid) {
	for (size_t m = 0; m < uuid.uuid.size(); m++) {
		stream << std::hex << std::setw(2) << std::setfill('0') << static_cast<unsigned int>(uuid.uuid[m]);

		if (m == 3 || m == 5 || m == 7 || m == 9) {
			stream << "-";
		}
	}

	return stream;
}

///
/// Print the UUID as a UTF-8 string.
///
/// @return a UTF-8 string representing the UUID
///
template <typename AllocatorT> inline Balau::U8String<AllocatorT> toString(const UUID & uuid) {
	return uuid.asString<AllocatorT>();
}

///
/// Print the UUID as a UTF-8 string.
///
/// @return a UTF-8 string representing the UUID
///
inline std::string toString(const UUID & uuid) {
	return uuid.asString();
}

template <typename AllocatorT> inline Balau::U8String<AllocatorT> UUID::asString() const {
	Balau::U8OStringStream<AllocatorT> stream;
	stream << uuid;
	return stream.str();
}

///
/// Get the UUID as a UTF-8 string.
///
/// This allocates a string stream and string object.
///
inline std::string UUID::asString() const {
	std::ostringstream stream;
	stream << *this;
	return stream.str();
}

} // namespace Balau

#endif // COM_BORA_SOFTWARE__BALAU_TYPE__UUID
