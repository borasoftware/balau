// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#ifndef BALAU_TP__HASH_LIBRARY__HASH_LIBRARY_TEST
#define BALAU_TP__HASH_LIBRARY__HASH_LIBRARY_TEST

#include <Balau/Testing/TestRunner.hpp>

namespace Balau {

struct HashLibraryTest : public Testing::TestGroup<HashLibraryTest> {
    HashLibraryTest() {
      registerTest(&HashLibraryTest::test, "test");
    }

    void test();
};

} // namespace Balau

#endif // BALAU_TP__HASH_LIBRARY__HASH_LIBRARY_TEST
