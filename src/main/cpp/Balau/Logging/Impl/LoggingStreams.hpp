// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
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

// Built-in logging stream which wraps an external std::ostream.
class OStreamLoggingStream : public LoggingStream {
	private: std::ostream & stream;

	public: explicit OStreamLoggingStream(std::ostream & stream_) : stream(stream_) {}

	public: void write(const LoggingSystem::LoggerString & str) override {
		stream << str;
	}

	public: void flush() override {
		stream.flush();
	}
};

//
// Built-in logging stream which logs to a file.
//
// If the supplied URI contains one or more ${date} placeholders, the logging
// stream will update at midnight to append to a new file.
//
class FileLoggingStream : public LoggingStream {
	private: std::shared_ptr<System::Clock> clock;
	private: std::vector<std::string> pathComponents;
	private: std::atomic_bool running;
	private: std::mutex syncMutex;
	private: std::condition_variable syncCondition;
	private: std::thread updater;
	private: std::string currentPath;

	// Shared pointer prevents race condition when changing stream.
	private: std::shared_ptr<boost::filesystem::ofstream> stream;

	public: FileLoggingStream(std::shared_ptr<System::Clock> clock_, std::string_view uri);
	public: ~FileLoggingStream() override;

	public: void write(const LoggingSystem::LoggerString & str) override {
		std::shared_ptr<boost::filesystem::ofstream> s = stream;
		*s << str;
	}

	public: void flush() override {
		std::shared_ptr<boost::filesystem::ofstream> s = stream;
		s->flush();
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
