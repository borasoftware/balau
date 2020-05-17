// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#ifndef COM_BORA_SOFTWARE__BALAU_TESTING__TEST_RUNNER_BASE
#define COM_BORA_SOFTWARE__BALAU_TESTING__TEST_RUNNER_BASE

namespace Balau::Impl {

class TestGroupBase;

class TestRunnerBase {
	friend class TestGroupBase;

	private: virtual unsigned int getGroupIndex() = 0;
};

} // namespace Balau::Impl

#endif // COM_BORA_SOFTWARE__BALAU_TESTING__TEST_RUNNER
