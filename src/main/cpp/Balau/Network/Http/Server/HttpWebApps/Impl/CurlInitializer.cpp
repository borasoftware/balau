// @formatter:off
//
// Balau core C++ library
// Copyright (C) 2017 Bora Software (contact@borasoftware.com)
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
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
