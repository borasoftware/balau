// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#ifndef COM_BORA_SOFTWARE__BALAU__TEST_RESOURCES
#define COM_BORA_SOFTWARE__BALAU__TEST_RESOURCES

#include <Balau/Resource/File.hpp>

namespace Balau {

struct TestResources final {
	const static Resource::File BalauBaseFolder;
	const static Resource::File BalauSourceFolder;
	const static Resource::File BalauSourceMainFolder;
	const static Resource::File BalauSourceTestFolder;
	const static Resource::File BalauSourceTestResourcesFolder;
	const static Resource::File BalauTargetFolder;
	const static Resource::File BalauTestResultsFolder;

	const static std::string BalauTestEmailHost;
	const static std::string BalauTestEmailPort;
	const static std::string BalauTestEmailSenderUser;
	const static std::string BalauTestEmailSenderPassword;
	const static std::string BalauTestEmailTo;

	///
	/// Asserts that the test resource file exists and returns a file pointing to it.
	///
	static Resource::File assertTestResourceFileExists(const std::string & relativePath);

	static std::string getTestResourceAsString(const std::string & relativePath);

	///////////////////////////////////////////////////////////////////////////

	TestResources() = delete;
	TestResources(const TestResources &) = delete;
	TestResources & operator = (const TestResources &) = delete;
};

} // namespace Balau

#endif // COM_BORA_SOFTWARE__BALAU__TEST_RESOURCES
