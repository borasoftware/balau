// @formatter:off
//
// Balau core C++ library
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
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
#include <Balau/Network/Http/Server/HttpServer.hpp>
#include <Balau/System/SystemClock.hpp>
#include <Balau/System/ThreadName.hpp>
#include <Balau/Logging/Logger.hpp>

#include <signal.h>

using namespace Balau;
using Balau::Network::Http::HttpServer;

const unsigned short port = 12345;
const Resource::File root = Resource::File(BalauString(BALAU_SOURCE_FOLDER_)) / "doc";
//const Resource::File root = Resource::File(BalauString(BALAU_TARGET_FOLDER_)) / "manual";

std::unique_ptr<HttpServer> server;
Logger & logger = Logger::getLogger("main");

int main() {
	System::ThreadName::setName("Main");

	const Network::Endpoint endpoint = Balau::Network::makeEndpoint("0.0.0.0", port);
	const unsigned int workerCount = std::thread::hardware_concurrency();

	BalauLogInfo(logger, "HTTP server starting up on endpoint {} with {} workers.", endpoint, workerCount);

	auto clock = std::shared_ptr<System::Clock>(new System::SystemClock());

	server = std::make_unique<HttpServer>(clock, "Test server", endpoint, "Server", workerCount, root);
	server->startSync();

	return 0;
}
