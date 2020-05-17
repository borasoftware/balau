// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#ifndef COM_BORA_SOFTWARE__BALAU_TESTING_IMPL__TEST_METHOD_BASE
#define COM_BORA_SOFTWARE__BALAU_TESTING_IMPL__TEST_METHOD_BASE

namespace Balau::Impl {

// Base interface for the test method container.
class TestMethodBase {
	public: virtual ~TestMethodBase() = default;

	public: virtual void run() = 0;
};

} // namespace Balau::Impl

#endif // COM_BORA_SOFTWARE__BALAU_TESTING_IMPL__TEST_METHOD_BASE
