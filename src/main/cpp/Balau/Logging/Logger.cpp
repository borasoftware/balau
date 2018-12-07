// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include "Logger.hpp"
#include "../Logging/Impl/LoggingState.hpp"
#include "../System/SystemClock.hpp"
#include "Impl/LoggerMessageItems.hpp"

namespace Balau {

using namespace Util;
using namespace LoggingSystem;

Logger & globalLogger = Logger::getLogger("");

Logger & Logger::getLogger(std::string_view loggingNamespace) noexcept {
	try {
		return LoggingState::loggingSystemState().getInstance(loggingNamespace);
	} catch (const std::exception & e) {
		try {
			fprintf(stderr, "FATAL: an exception was thrown when attempting to obtain the logger for namespace"); // NOLINT
			fprintf(stderr, "FATAL: %s\n", loggingNamespace.data()); // NOLINT
			fprintf(stderr, "FATAL: %s\n", e.what()); // NOLINT
		} catch (...) {
			// Nothing left to do.
		}
	} catch (...) {
		try {
			fprintf(stderr, "FATAL: an unknown error occurred when attempting to obtain the logger for namespace"); // NOLINT
			fprintf(stderr, "FATAL: %s\n", loggingNamespace.data()); // NOLINT
		} catch (...) {
			// Nothing left to do.
		}
	}

	return LoggerHolder::backupLogger;
}

void Logger::flushAll() {
	LoggingState::loggingSystemState().flushAll();
}

void Logger::configure(const std::string & configurationText) {
	LoggingState::loggingSystemState().configure(configurationText);
}

void Logger::resetConfiguration() {
	LoggingState::loggingSystemState().resetConfigure();
}

void Logger::lockConfiguration(bool throwOnReconfigure) {
	LoggingState::loggingSystemState().lockConfiguration(throwOnReconfigure);
}

std::string Logger::getConfiguration() const {
	std::ostringstream stream;
	stream << nameSpace << " = ";
	const auto padLength = (unsigned int) nameSpace.length() + 3U;
	std::string prefix;

	for (const auto & property : properties) {
		stream << prefix << property.first << ": " << property.second << "\n";

		if (prefix.empty()) {
			prefix = Util::Strings::padLeft(", ", padLength, ' ');
		}
	}

	return stream.str();
}

void Logger::registerLoggingStreamFactory(const std::string & scheme, LoggingStreamFactory factory) {
	LoggingState::loggingSystemState().registerLoggingStreamFactory(scheme, factory);
}

inline void copyStreamPointers(std::array<std::atomic<LoggingStream *>, _BalauLoggingLevelCount> & dst,
                               const std::array<std::atomic<LoggingStream *>, _BalauLoggingLevelCount> & src) {
	for (size_t m = 0; m < dst.size(); m++) {
		dst[m].store(src[m].load());
	}
}

void Logger::flush() const {
	std::array<std::atomic<LoggingStream *>, _BalauLoggingLevelCount> currentStreams {};
	copyStreamPointers(currentStreams, streams);
	std::array<LoggingStream *, _BalauLoggingLevelCount> flushedStreams {};
	size_t index = 0;

	for (const auto & currentStream : currentStreams) {
		if (std::find(flushedStreams.begin(), flushedStreams.end(), currentStream) == flushedStreams.end()) {
			LoggingStream * s = currentStream.load();
			s->flush();
			flushedStreams[index++] = s;
		}
	}
}

// Convert the supplied message to a vector of message items.
// TODO can this be achieved at compile time?
std::vector<std::unique_ptr<MessageItem>> textToMessageItems(std::string_view message, size_t & parameterCount) {
	std::vector<std::unique_ptr<MessageItem>> ret {};
	size_t textStart = 0;
	size_t textEnd = 0;

	auto iter = message.begin();
	auto end = message.end();

	while (iter != end) {
		if (*iter == '{') {
			++iter;

			if (iter == end) {
				++textEnd;
				break;
			} else if (*iter == '}') {
				if (textEnd != textStart) {
					// A pair of {} after some text.. construct text item first.
					ret.emplace_back(std::unique_ptr<MessageItem>(new StringMessageItem(textStart, textEnd)));
					textStart = textEnd;
				}

				// Construct parameter item.
				ret.emplace_back(std::unique_ptr<MessageItem>(new ParameterMessageItem));
				++parameterCount;

				// Jump the "{}" and reset.
				textStart += 2;
				textEnd = textStart;
			} else {
				textEnd += 2;
			}
		} else {
			++textEnd;
		}

		++iter;
	}

	if (textEnd != textStart) {
		ret.emplace_back(std::unique_ptr<MessageItem>(new StringMessageItem(textStart, textEnd)));
	}

	return ret;
}

Logger::Logger(std::string identifier_,
               std::string nameSpace_,
               std::string ns_,
               const std::initializer_list<std::string> & propertyPairs)
	: identifier(std::move(identifier_))
	, nameSpace(std::move(nameSpace_))
	, ns(std::move(ns_))
	, level(LoggingLevel::INFO)
	, shouldFlush(true) {
	auto iter = propertyPairs.begin();
	while (iter != propertyPairs.end()) {
		auto & key = *(iter++);
		auto & value = *(iter++);
		properties[key] = value;
	}
}

Logger::Logger(Logger && rhs) noexcept
	: identifier(std::move(rhs.identifier))
	, nameSpace(std::move(rhs.nameSpace))
	, ns(std::move(rhs.ns))
	, level(rhs.level.load())
	, shouldFlush(rhs.shouldFlush.load())
	, properties(std::move(rhs.properties)) {}

Logger::Logger(std::string && identifier_, std::string && nameSpace_, std::string && ns_) noexcept
	: identifier(std::move(identifier_))
	, nameSpace(std::move(nameSpace_))
	, ns(std::move(ns_))
	, level(LoggingLevel::INFO)
	, shouldFlush(true) {}

void Logger::inheritConfiguration(const Logger & copy) {
	level.store(copy.level);
	shouldFlush.store(copy.shouldFlush);
	logItems.store(copy.logItems.load());
	copyStreamPointers(streams, copy.streams);
	const std::string streamStr = std::string("stream");

	// Special treatment for stream properties.
	// If the source properties contains one or more stream properties,
	// then all current stream properties should be deleted first.
	for (const auto & property : copy.properties) {
		if (Strings::endsWith(property.first, streamStr)) {
			auto iter = properties.begin();
			while (iter != properties.end()) {
				if (Strings::endsWith(iter->first, streamStr)) {
					iter = properties.erase(iter);
				} else {
					++iter;
				}
			}

			break;
		}
	}

	// Copy across the properties.
	for (const auto & property : copy.properties) {
		properties[property.first] = property.second;
	}
}

void Logger::logMessage(const SourceCodeLocation & location,
                        LoggingLevel level,
                        const Logger & logger,
                        std::string_view message) {
	// Apart from compiler ordering restrictions, these atomic reads are free on x86/x64.
	const LogItemVector * const loggerItems = logger.logItems.load(std::memory_order_acquire);
	LoggingStream * const stream = logger.streams[Enums::toUnderlying(level)].load(std::memory_order_relaxed);

	if (loggerItems == nullptr || stream == nullptr) {
		return; // The logging system has not yet configured the logger.
	}

	auto timePoint = System::SystemClock().now();
	std::ostringstream fullTextStream;

	LoggerItemParameters loggerItemParameters(
		  fullTextStream
		, logger.nameSpace
		, logger.ns
		, level
		, location.file
		, location.line
		, message
		, timePoint
	);

	for (const std::shared_ptr<LogItem> & item : *loggerItems) {
		item->write(loggerItemParameters);
	}

	fullTextStream << "\n";
	stream->write(fullTextStream.str());

	if (logger.shouldFlush) {
		stream->flush();
	}
}

void Logger::logMessage(const SourceCodeLocation & location,
                        LoggingLevel level,
                        const Logger & logger,
                        std::string_view message,
                        const std::vector<std::string> & parameters) {
	// Apart from compiler ordering restrictions, these atomic reads are free on x86/x64.
	const LogItemVector * const loggerItems = logger.logItems.load(std::memory_order_acquire);
	LoggingStream * const stream = logger.streams[Enums::toUnderlying(level)].load(std::memory_order_relaxed);

	if (loggerItems == nullptr || stream == nullptr) {
		return; // The logging system has not yet configured the logger.
	}

	size_t parameterCount = 0;
	auto timePoint = System::SystemClock().now();
	auto messageItems = textToMessageItems(message, parameterCount);
	std::ostringstream messageTextStream;
	const std::vector<std::string> * finalParameters;

	if (parameters.size() < parameterCount) {
		// The logging message does not have enough parameters.
		std::vector<std::string> replacementParameters = parameters;

		while (parameterCount < parameters.size()) {
			replacementParameters.emplace_back("????");
		}

		finalParameters = &replacementParameters;
	} else {
		finalParameters = &parameters;
	}

	auto parameterIterator = finalParameters->begin();

	for (const std::unique_ptr<MessageItem> & item : messageItems) {
		messageTextStream << item->get(message, parameterIterator);
	}

	std::string messageText = messageTextStream.str();
	std::ostringstream fullTextStream;

	LoggerItemParameters loggerItemParameters(
		  fullTextStream
		, logger.nameSpace
		, logger.ns
		, level
		, location.file
		, location.line
		, messageText.c_str()
		, timePoint
	);

	for (const std::shared_ptr<LogItem> & item : *loggerItems) {
		item->write(loggerItemParameters);
	}

	fullTextStream << "\n";
	stream->write(fullTextStream.str());

	if (logger.shouldFlush) {
		stream->flush();
	}
}

} // namespace Balau
