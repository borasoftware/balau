// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include "TestResources.hpp"

#include <Balau/Util/Macros.hpp>
#include <Balau/Util/Files.hpp>
#include <Balau/Testing/Assertions.hpp>

namespace Balau {

using Testing::assertThat;
using Testing::is;

const Resource::File TestResources::BalauBaseFolder                = Resource::File(BalauString(BALAU_BASE_FOLDER_));
const Resource::File TestResources::BalauSourceFolder              = Resource::File(BalauString(BALAU_SOURCE_FOLDER_));
const Resource::File TestResources::BalauSourceMainFolder          = Resource::File(BalauString(BALAU_SOURCE_MAIN_FOLDER_)); // NOLINT
const Resource::File TestResources::BalauSourceTestFolder          = Resource::File(BalauString(BALAU_SOURCE_TEST_FOLDER_)); // NOLINT
const Resource::File TestResources::BalauSourceTestResourcesFolder = Resource::File(BalauString(BALAU_SOURCE_TEST_RESOURCES_FOLDER_)); // NOLINT
const Resource::File TestResources::BalauTargetFolder              = Resource::File(BalauString(BALAU_TARGET_FOLDER_)); // NOLINT
const Resource::File TestResources::BalauTestResultsFolder         = Resource::File(BalauString(BALAU_TEST_RESULTS_FOLDER_)); // NOLINT


const std::string TestResources::BalauTestEmailHost                = BalauString(BALAU_TEST_EMAIL_HOST_);
const std::string TestResources::BalauTestEmailPort                = BalauString(BALAU_TEST_EMAIL_PORT_);
const std::string TestResources::BalauTestEmailSenderUser          = BalauString(BALAU_TEST_EMAIL_SENDER_USER_);
const std::string TestResources::BalauTestEmailSenderPassword      = BalauString(BALAU_TEST_EMAIL_SENDER_PASSWORD_);
const std::string TestResources::BalauTestEmailTo                  = BalauString(BALAU_TEST_EMAIL_TO_);

Resource::File TestResources::assertTestResourceFileExists(const std::string & relativePath) {
	Resource::File path = BalauSourceTestResourcesFolder / relativePath;
	assertThat(path.exists(), is(true));
	return path;
}

std::string TestResources::getTestResourceAsString(const std::string & relativePath) {
	return Util::Files::readToString(BalauSourceTestResourcesFolder / relativePath);
}

} // namespace Balau
