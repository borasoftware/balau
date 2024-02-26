// @formatter:off
//
// Balau core C++ library
//
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

#ifndef COM_BORA_SOFTWARE_BALAU_TYPE_UUID
#define COM_BORA_SOFTWARE_BALAU_TYPE_UUID

#include <Balau/Type/ToString.hpp>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <sstream>

namespace Balau {

///
/// %UUID class, using the Boost uuid implementation.
///
class UUID {
	///
	/// Construct a new UUID with a random value.
	///
	public: UUID() : uuid(boost::uuids::random_generator()()) {}

	///
	/// Construct a new UUID by copying the supplied instance.
	///
	public: UUID(const UUID & copy) = default;

	///
	/// Get the UUID as a std::array of bytes.
	///
	/// This allocates a std::array object.
	///
	public: std::array<unsigned char, 16> asArray() const {
		std::array<unsigned char, 16> a {};
		memcpy(a.data(), uuid.data, 16);
		return a;
	}

	///
	/// Get the UUID as raw bytes.
	///
	/// This returns the bytes directly. No object construction is performed.
	///
	public: const unsigned char * asBytes() const {
		return uuid.data;
	}

	///
	/// Get the UUID as a UTF-8 string.
	///
	/// This allocates a string stream and string object.
	///
	public: template <typename AllocatorT> Balau::U8String<AllocatorT> asString() const {
		Balau::U8OStringStream<AllocatorT> stream;
		stream << uuid;
		return stream.str();
	}

	///
	/// Get the UUID as a UTF-8 string.
	///
	/// This allocates a string stream and string object.
	///
	public: std::string asString() const {
		std::ostringstream stream;
		stream << uuid;
		return stream.str();
	}

	///
	/// Get the UUID as the underlying Boost UUID.
	///
	/// No object construction is performed.
	///
	public: const boost::uuids::uuid & asUUID() const {
		return uuid;
	}

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

	private: const boost::uuids::uuid uuid;
};

///
/// Print the UUID as a UTF-8 string.
///
/// @return a UTF-8 string representing the UUID
///
template <typename AllocatorT>
inline Balau::U8String<AllocatorT> toString(const UUID & uuid) {
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

} // namespace Balau

#endif // COM_BORA_SOFTWARE_BALAU_TYPE_UUID
