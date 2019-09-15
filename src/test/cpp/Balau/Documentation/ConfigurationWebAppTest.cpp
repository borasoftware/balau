// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include <Balau/Documentation/ConfigurationWebApp.hpp>
#include <TestResources.hpp>

using namespace Balau::Network;
using namespace Balau::Network::Http;

namespace Balau::Documentation {

struct ConfigurationWebAppTest : public Testing::TestGroup<ConfigurationWebAppTest> {
	ConfigurationWebAppTest() {
		registerTest(&ConfigurationWebAppTest::test, "test");
	}

	void test() {
		// TODO
	}
};

} // namespace Balau::Documentation
