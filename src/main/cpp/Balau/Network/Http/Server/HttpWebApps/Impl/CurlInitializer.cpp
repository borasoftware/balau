// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2017 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include "CurlInitializer.hpp"

#include "../../../../../Concurrent/SingleTimeExecutor.hpp"
#include "../../../../../Logging/Logger.hpp"

#include <curl/curl.h>

#include <atomic>
#include <mutex>

namespace Balau::Network::Http::HttpWebApps::Impl {

Logger & log = Logger::getLogger("balau.network.curl"); // NOLINT

class CurlInitializerImpl {
	public: CurlInitializerImpl() : initializer(
		[] () {
			log.info("Initializing Curl.");
			curl_global_init(CURL_GLOBAL_ALL);
			log.info("Finished initializing Curl.");
		}
		, [] () {
			curl_global_cleanup();
		}
	) {}

	public: CurlInitializerImpl(const CurlInitializerImpl &) = delete;
	public: CurlInitializerImpl & operator = (const CurlInitializerImpl &) = delete;

	public: void ensureCurlInitialization() {
		initializer();
	}

	public: ~CurlInitializerImpl() = default;

	private: Concurrent::SingleTimeExecutor initializer;
};

CurlInitializerImpl curlInitializerImplInstance; // NOLINT

void ensureCurlInitialization() {
	curlInitializerImplInstance.ensureCurlInitialization();
}

} // namespace Balau::Network::Http::HttpWebApps::Impl
