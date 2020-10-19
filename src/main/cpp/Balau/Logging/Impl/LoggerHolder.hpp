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
	friend class LoggerPropertyVisitor;
	friend class LoggerConfigurationVisitor;
	friend class ::Balau::Logger;
	friend class ::Balau::Container::ObjectTrie<LoggerHolder>;
	friend class ::Balau::Container::ObjectTrieNode<LoggerHolder>;
};

using LoggerTree = Container::ObjectTrie<LoggerHolder>;
using LoggerTreeNode = Container::ObjectTrieNode<LoggerHolder>;

} // namespace Balau::LoggingSystem

#endif // COM_BORA_SOFTWARE__BALAU_LOGGING_IMPL__LOGGER_HOLDER
