// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#ifndef COM_BORA_SOFTWARE__BALAU_LANG__SHARED_MEMORY_QUEUE_TEST
#define COM_BORA_SOFTWARE__BALAU_LANG__SHARED_MEMORY_QUEUE_TEST

#include <Balau/Type/ToString.hpp>
#include <Balau/Serialization/SerializationMacros.hpp>

#include <array>

namespace Balau {

namespace Interprocess {

// Test object.
struct SMT {
	double d;
	int i;
	std::array<char, 128> padding;

	SMT() : d(0.0), i(0), padding({}) {}
	SMT(double d_, int i_) : d(d_), i(i_), padding({}) {}

	template <typename Archive> void serialize(Archive & archive, unsigned int ) {
		archive & BoostSerialization(d) & BoostSerialization(i);
	}
};

inline std::string toString(const SMT & object) {
	return "{ " + ::toString(object.d) + ", " + ::toString(object.i) + " }";
}

inline bool operator == (const SMT & lhs, const SMT & rhs) {
	return lhs.d == rhs.d && lhs.i == rhs.i;
}

inline bool operator != (const SMT & lhs, const SMT & rhs) {
	return lhs.d != rhs.d || lhs.i != rhs.i;
}

} // namespace Interprocess

} // namespace Balau

#include <Balau/Testing/TestRunner.hpp>

namespace Balau {

namespace Interprocess {

struct SharedMemoryQueueTest : public Testing::TestGroup<SharedMemoryQueueTest> {
	SharedMemoryQueueTest() {
		registerTest(&SharedMemoryQueueTest::singleBufferSPST,      "singleBufferSPST");
		registerTest(&SharedMemoryQueueTest::singleBufferSISO,      "singleBufferSISO");
		registerTest(&SharedMemoryQueueTest::singleBufferSIMO,      "singleBufferSIMO");
		registerTest(&SharedMemoryQueueTest::singleBufferMISO,      "singleBufferMISO");
		registerTest(&SharedMemoryQueueTest::singleBufferMIMO,      "singleBufferMIMO");
		registerTest(&SharedMemoryQueueTest::multipleBufferSPST,    "multipleBufferSPST");
		registerTest(&SharedMemoryQueueTest::multipleBufferSISO,    "multipleBufferSISO");
		registerTest(&SharedMemoryQueueTest::multipleBufferMISO,    "multipleBufferMISO");
		registerTest(&SharedMemoryQueueTest::multipleBufferPending, "multipleBufferPending");
	}

	void singleBufferSPST();
	void singleBufferSISO();
	void singleBufferSIMO();
	void singleBufferMISO();
	void singleBufferMIMO();
	void multipleBufferSPST();
	void multipleBufferSISO();
	void multipleBufferMISO();
	void multipleBufferPending();
};

} // namespace Interprocess

} // namespace Balau

#endif // COM_BORA_SOFTWARE__BALAU_LANG__SHARED_MEMORY_QUEUE_TEST
