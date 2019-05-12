// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#ifndef COM_BORA_SOFTWARE__BALAU_COLLECTION__ARRAY_BLOCKING_QUEUE_TEST
#define COM_BORA_SOFTWARE__BALAU_COLLECTION__ARRAY_BLOCKING_QUEUE_TEST

#include <Balau/Testing/TestRunner.hpp>

namespace Balau::Container {

struct ArrayBlockingQueueTest : public Testing::TestGroup<ArrayBlockingQueueTest> {
	ArrayBlockingQueueTest() {
		registerTest(&ArrayBlockingQueueTest::fullQueue,  "fullQueue");
	}

	void fullQueue();
};

} // namespace Balau::Container

#endif // COM_BORA_SOFTWARE__BALAU_COLLECTION__ARRAY_BLOCKING_QUEUE_TEST
