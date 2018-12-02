// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#ifndef COM_BORA_SOFTWARE__BALAU_LOGGING_IMPL__LOGGER_HOLDER
#define COM_BORA_SOFTWARE__BALAU_LOGGING_IMPL__LOGGER_HOLDER

#include <Balau/Container/ObjectTrie.hpp>
#include <Balau/Logging/Logger.hpp>

namespace Balau::LoggingSystem {

//
// Allows the logger tree to be changed whilst moving existing loggers around
// and without invalidating external references of the loggers.
//
// The LoggerHolder class wraps a pointer container (which provides lifecycle
// management), allowing it to work correctly within the ObjectTrie implementation
// with regard to operators used during finding and cascading.
//
class LoggerHolder {
	private: static Logger backupLogger;

	private: std::shared_ptr<Logger> logger;

	private: std::shared_ptr<Logger> getLogger() {
		return logger;
	}

	private: const std::shared_ptr<Logger> getLogger() const {
		return logger;
	}

	private: explicit LoggerHolder(std::shared_ptr<Logger> logger_) : logger(std::move(logger_)) {}

	public: LoggerHolder(LoggerHolder && rhs) noexcept : logger(std::move(rhs.logger)) {}

	private: LoggerHolder() = default;
	private: LoggerHolder(const LoggerHolder & copy) = default;

	private: LoggerHolder & operator = (LoggerHolder && rhs) noexcept {
		logger = rhs.logger;
		return *this;
	}

	private: LoggerHolder & operator = (const LoggerHolder & rhs) {
		logger->inheritConfiguration(*rhs.logger);
		return *this;
	}

	private: bool operator == (const LoggerHolder & loggerHolder) const {
		return *logger == *loggerHolder.logger;
	}

	friend class LoggingState;
	friend class ::Balau::Logger;
	friend class ::Balau::Container::ObjectTrie<LoggerHolder>;
	friend class ::Balau::Container::ObjectTrieNode<LoggerHolder>;
};

using LoggerTree = Container::ObjectTrie<LoggerHolder>;
using LoggerTreeNode = Container::ObjectTrieNode<LoggerHolder>;

} // namespace Balau::LoggingSystem

#endif // COM_BORA_SOFTWARE__BALAU_LOGGING_IMPL__LOGGER_HOLDER
