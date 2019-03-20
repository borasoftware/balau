// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2017 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#ifndef COM_BORA_SOFTWARE__BALAU_NETWORK__FILE_SERVING_HTTP_WEB_APP_TEST
#define COM_BORA_SOFTWARE__BALAU_NETWORK__FILE_SERVING_HTTP_WEB_APP_TEST

#include <Balau/Network/Http/Server/NetworkTypes.hpp>
#include <Balau/Testing/TestRunner.hpp>

namespace Balau {

namespace Network {

namespace Http {

namespace HttpWebApps {

struct FileServingHttpWebAppTest : public Testing::TestGroup<FileServingHttpWebAppTest> {
	FileServingHttpWebAppTest() {
		registerTest(&FileServingHttpWebAppTest::getFile,             "getFile");
		registerTest(&FileServingHttpWebAppTest::getNotFoundFile,     "getNotFoundFile");
		registerTest(&FileServingHttpWebAppTest::headFile,            "headFile");
		registerTest(&FileServingHttpWebAppTest::headNotFoundFile,    "headNotFoundFile");
		registerTest(&FileServingHttpWebAppTest::getFile1000Serial,   "getFile1000Serial");
		registerTest(&FileServingHttpWebAppTest::getFile1000Parallel, "getFile1000Parallel");
		registerTest(&FileServingHttpWebAppTest::getFiles,            "getFiles");
	}

	void getFile();
	void getNotFoundFile();
	void headFile();
	void headNotFoundFile();
	void getFile1000Serial();
	void getFile1000Parallel();
	void getFiles();
};

} // namespace HttpWebApps

} // namespace Http

} // namespace Network

} // namespace Balau

#endif // COM_BORA_SOFTWARE__BALAU_NETWORK__FILE_SERVING_HTTP_WEB_APP_TEST
