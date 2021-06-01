// @formatter:off
//
// Balau core C++ library
//
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

#ifndef COM_BORA_SOFTWARE__BALAU_LOGGING_IMPL__LOGGING_STREAMS
#define COM_BORA_SOFTWARE__BALAU_LOGGING_IMPL__LOGGING_STREAMS

#include <Balau/Logging/Logger.hpp>
#include <Balau/System/Clock.hpp>
#include <Balau/Util/Files.hpp>

#include <condition_variable>
#include <mutex>
#include <thread>
#include <regex>

namespace Balau::LoggingSystem {

////////////////////////// Built-in logging streams ///////////////////////////

///
/// Built-in logging stream which wraps an external std::ostream.
///
/// The supplied output streams may or may not be thread safe, thus the constructor accepts
/// an optional threadSafe_ argument that can be set to true if mutex locking is not required.
///
class OStreamLoggingStream : public LoggingStream {
	public: explicit OStreamLoggingStream(std::ostream & stream_, bool threadSafe_ = false)
		: stream(stream_)
		, threadSafe(threadSafe_) {}

	public: void write(const LoggingSystem::LoggerString & str) override {
		if (!threadSafe) {
			std::lock_guard<std::mutex> lock(syncMutex);
			stream << str;
		} else {
			stream << str;
		}
	}

	public: void flush() override {
		if (!threadSafe) {
			std::lock_guard<std::mutex> lock(syncMutex);
			stream.flush();
		} else {
			stream.flush();
		}
	}

	private: std::ostream & stream;
	private: const bool threadSafe;
	private: std::mutex syncMutex;
};

///
/// Built-in logging stream which logs to stdout.
///
/// The C++ specification states that stdout is threadsafe, hence no mutex lock is used in this logging stream.
///
class StdOutLoggingStream : public LoggingStream {
	public: void write(const LoggingSystem::LoggerString & str) override {
		std::cout << str;
	}

	public: void flush() override {
		std::cout.flush();
	}
};

///
/// Built-in logging stream which logs to stderr.
///
/// The C++ specification states that stderr is threadsafe, hence no mutex lock is used in this logging stream.
///
class StdErrLoggingStream : public LoggingStream {
	public: void write(const LoggingSystem::LoggerString & str) override {
		std::cerr << str;
	}

	public: void flush() override {
		std::cerr.flush();
	}
};

///
/// Built-in logging stream which logs to a file.
///
/// If the supplied URI contains one or more ${date} placeholders, the logging
/// stream will update at midnight to append to a new file.
///
/// File output streams are not thread safe, thus this logging stream uses a
/// mutex to prevent a race condition.
///
class FileLoggingStream : public LoggingStream {
	private: std::shared_ptr<System::Clock> clock;
	private: std::vector<std::string> pathComponents;
	private: std::atomic_bool running;
	private: std::mutex syncMutex;
	private: std::condition_variable syncCondition;
	private: std::thread updater;
	private: std::string currentPath;

	private: std::shared_ptr<boost::filesystem::ofstream> stream;

	public: FileLoggingStream(std::shared_ptr<System::Clock> clock_, std::string_view uri);
	public: ~FileLoggingStream() override;

	public: void write(const LoggingSystem::LoggerString & str) override {
		std::lock_guard<std::mutex> lock(syncMutex);
		*stream << str;
	}

	public: void flush() override {
		std::lock_guard<std::mutex> lock(syncMutex);
		stream->flush();
	}

	///////////////////////// Private implementation //////////////////////////

	// Parses the path components, builds the current day's file logging path,
	// and creates a new output file stream.
	private: void createNewStream();

	// The update thread function.
	private: static void updateFunction(FileLoggingStream * self);

	private: static std::regex dateRegEx;
	private: static std::regex dateRegExExact;

	friend struct LoggingStreamsTest;
};

} // namespace Balau::LoggingSystem

#endif // COM_BORA_SOFTWARE__BALAU_LOGGING_IMPL__LOGGING_STREAMS
