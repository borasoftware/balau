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

#include "LoggingState.hpp"
#include "LoggerPropertyVisitor.hpp"
#include "LoggerConfigurationVisitor.hpp"
#include "../../Application/Impl/PropertyBindingBuilderFactory.hpp"
#include "../../System/SystemClock.hpp"
#include "../../Util/User.hpp"

// Defines for logging system development logging.

// Set to 1 to print out the loggers between each stage.
#define BALAU_LOGGING__PRINT_LOGGERS_IN_BETWEEN_CONFIGURE_STAGES 0

// Set to 1 to print out logging configuration messages to stdout.
#define BALAU_LOGGING__PRINT_CONFIGURATION_MESSAGES 0

namespace Balau {

using namespace Util;

namespace LoggingSystem {

// Singleton container for logging state.
class LoggingStateHolder {
	public: LoggingStateHolder()
		: instance(new LoggingState) {}

	public: ~LoggingStateHolder() {
		std::lock_guard<std::mutex> lock(mutex);

		for (auto logItemVector : instance->logItemVectorPool) {
			delete logItemVector;
		}

		instance->logItemVectorPool.clear();

		for (auto & stream : instance->streamPoolsByUri) {
			delete stream.second;
		}

		instance->streamPoolsByUri.clear();
	}

	public: LoggingState * instance;
	public: std::mutex mutex;
};


LoggingStateHolder & LoggingState::loggingStateHolder() {
	static LoggingStateHolder holder;
	return holder;
}

LoggingState & LoggingState::loggingSystemState() {
	return *loggingStateHolder().instance;
}

LoggingState::LoggingState()
	: locked(printStartupDebugMessageAndReturnFalse())
	, noisy(false)
	, logItemPool()
	, streamFactories(createDefaultStreamFactories())
	, loggerTree(autoConfigure()) {
	printLoggingDebugMessage("finished constructing");
}

std::map<std::string, LoggingStreamFactory> LoggingState::createDefaultStreamFactories() {
	return {
		std::make_pair(
			"stdout"
			, [] (std::string_view) {
				return static_cast<LoggingStream *>(new StdOutLoggingStream);
			}
		)

		, std::make_pair(
			"stderr"
			, [] (std::string_view) {
				return static_cast<LoggingStream *>(new StdErrLoggingStream);
			}
		)

		, std::make_pair(
			"file"
			, [] (std::string_view uri) {
				// Do not throw here, as this call is in the execution path of getLogger.
				// Instead, log to std err and use stdout instead of the file.
				if (Strings::startsWith(uri, "file:///")) {
					return static_cast<LoggingStream *>(
						new FileLoggingStream(std::shared_ptr<System::Clock>(new System::SystemClock()), uri)
					);
				} else {
					std::cerr << "LOGGING CONFIGURATION ERROR: "
					          << "Invalid file scheme uri (uri must start with \"file:///\"): "
					          << uri
					          << ".\n Using stderr as fallback."
					          << std::endl;
					return static_cast<LoggingStream *>(new OStreamLoggingStream(std::cerr));
				}
			}
		)
	};
}

// Logging system logging (for development of the logging system).
#if BALAU_LOGGING__PRINT_CONFIGURATION_MESSAGES

bool LoggingState::printStartupDebugMessageAndReturnFalse() {
	std::cout << "LoggingSystem: starting up\n" << std::flush;
	return false;
}

void LoggingState::printLoggingDebugMessage(const std::string & message) {
	std::cout << (toString("LoggingSystem: ") + message + "\n") << std::flush;
}

#else

bool LoggingState::printStartupDebugMessageAndReturnFalse() {
	return false;
}

void LoggingState::printLoggingDebugMessage(std::string_view) {
}

#endif

std::string LoggingState::generateAbbreviatedNamespace(std::string_view loggerNamespace) {
	auto trimmedLoggerNamespace = Util::Strings::trim(loggerNamespace);

	if (trimmedLoggerNamespace.empty()) {
		return std::string(trimmedLoggerNamespace);
	}

	std::vector<std::string_view> identifiers = Strings::splitAndTrim(trimmedLoggerNamespace, ".");
	std::ostringstream builder;
	std::string prefix;

	if (!identifiers.empty()) {
		for (size_t m = 0; m < identifiers.size() - 1; m++) {
			builder << prefix << identifiers[m][0];
			prefix = ".";
		}

		builder << prefix << identifiers.back();
	}

	return builder.str();
}

Logger & LoggingState::getInstance(std::string_view loggingNamespace) {
	std::lock_guard<std::mutex> lock(loggingStateHolder().mutex);

	std::string nameSpaceStr = std::string(loggingNamespace == "." ? "" : loggingNamespace);

	const std::vector<std::string_view> nameSpace = Util::Containers::concatenate(
		std::vector<std::string_view> { "" }, Util::Strings::splitAndTrim(nameSpaceStr, ".")
	);

	LoggerTreeNode * nearest = loggerTree.findNearest(
		  nameSpace
		, [] (const auto & holder, const auto & identifier) { return holder.logger->identifier == identifier; }
		, false
	);

	if (nearest->value.getLogger()->nameSpace == nameSpaceStr) {
		return *nearest->value.getLogger();
	}

	const std::string remainingNamespace = nearest->value.getLogger()->nameSpace.empty()
		? nameSpaceStr
		: nameSpaceStr.substr(nearest->value.getLogger()->nameSpace.length() + 1);

	std::vector<std::string_view> remainingIdentifiers = Strings::splitAndTrim(remainingNamespace, ".");

	// Instantiate missing descendants.
	LoggerTreeNode * previous;
	LoggerTreeNode * current = nearest;
	std::string loggerNamespace = nearest->value.getLogger()->nameSpace;
	std::string prefix = loggerNamespace.empty() ? "" : ".";

	for (auto & identifier : remainingIdentifiers) {
		loggerNamespace += prefix + std::string(identifier);
		prefix = ".";
		std::string ns = generateAbbreviatedNamespace(loggerNamespace);

		previous = current;

		current = &previous->addAndReturnChild(
			LoggerHolder(
				std::shared_ptr<Logger>(
					new Logger(std::string(identifier), std::string(loggerNamespace), std::move(ns))
				)
			)
		);

		current->value.getLogger()->inheritConfiguration(*previous->value.getLogger());
	}

	return *current->value.getLogger();
}

void LoggingState::flushAll() {
	std::lock_guard<std::mutex> lock(loggingStateHolder().mutex);
	performFlushAll();
}

void LoggingState::configure(std::string_view configurationText,
                             const std::map<std::string, std::string> & placeholders) {
	std::lock_guard<std::mutex> lock(loggingStateHolder().mutex);

	if (locked) {
		if (noisy) {
			ThrowBalauException(Exception::LoggingConfigurationException, "Attempt to reconfigure locked logging system.");
		}

		return;
	}

	performFlushAll();

	// Encase the placeholders.
	std::map<std::string, std::string> encasedPlaceholders;

	std::for_each(
		  placeholders.begin()
		, placeholders.end()
		, [&encasedPlaceholders] (const auto & p) { encasedPlaceholders.insert(std::pair<std::string, std::string>("${" + p.first + "}", p.second)); }
	);

	LoggerTree newLoggers = manualConfigure(configurationText, encasedPlaceholders);

	wipeProperties(loggerTree);
	cascadeAndConfigureLoggers(loggerTree, newLoggers);
}

void LoggingState::configure(const EnvironmentProperties & configuration,
                             const std::map<std::string, std::string> & placeholders) {
	std::lock_guard<std::mutex> lock(loggingStateHolder().mutex);

	if (locked) {
		if (noisy) {
			ThrowBalauException(Exception::LoggingConfigurationException, "Attempt to reconfigure locked logging system.");
		}

		return;
	}

	performFlushAll();

	// Encase the placeholders.
	std::map<std::string, std::string> encasedPlaceholders;

	std::for_each(
		  placeholders.begin()
		, placeholders.end()
		, [&encasedPlaceholders] (const auto & p) { encasedPlaceholders.insert(std::pair<std::string, std::string>("${" + p.first + "}", p.second)); }
	);

	LoggerTree newLoggers = manualConfigure(configuration, encasedPlaceholders);

	wipeProperties(loggerTree);
	cascadeAndConfigureLoggers(loggerTree, newLoggers);
}

void LoggingState::resetConfigure() {
	std::lock_guard<std::mutex> lock(loggingStateHolder().mutex);

	if (locked) {
		if (noisy) {
			ThrowBalauException(Exception::LoggingConfigurationException, "Attempt to reconfigure locked logging system.");
		}

		return;
	}

	performFlushAll();
	LoggerTree newLoggers = autoConfigure();
	wipeProperties(loggerTree);
	cascadeAndConfigureLoggers(loggerTree, newLoggers);
}

void LoggingState::lockConfiguration(bool throwOnReconfigure) {
	std::lock_guard<std::mutex> lock(loggingStateHolder().mutex);
	locked = true;
	noisy |= throwOnReconfigure;
}

void LoggingState::performFlushAll() {
	for (auto & stream : streamPoolsByUri) {
		stream.second->flush();
	}
}

void LoggingState::registerLoggingStreamFactory(const std::string & scheme, LoggingStreamFactory factory) {
	std::lock_guard<std::mutex> lock(loggingStateHolder().mutex);
	streamFactories.insert_or_assign(scheme, factory);
}

std::shared_ptr<LogItem> LoggingState::getOrCreateLogItem(const std::string & text) {
	auto textItem = logItemPool.find(text);

	if (textItem == logItemPool.end()) {
		logItemPool.insert(std::make_pair(text, std::make_shared<StringLogItem>(text)));
		textItem = logItemPool.find(text);
	}

	return textItem->second;
}

LogItemVector * LoggingState::lookupOrCacheItemVector(LogItemVector * newVector) {
	return *logItemVectorPool.insert(newVector).first;
}

LoggingStream * LoggingState::getOrCreateStream(const std::string & uri) {
	auto cachedStream = streamPoolsByUri.find(uri);

	if (cachedStream == streamPoolsByUri.end()) {
		std::string scheme = uri.substr(0, uri.find(':'));
		auto iter = streamFactories.find(scheme);

		if (iter == streamFactories.end()) {
			std::cerr << "LOGGING CONFIGURATION ERROR: Unknown scheme: '" << scheme
			          << "'.\n Using stderr as fallback." << std::endl;

			iter = streamFactories.find("stderr");
		}

		LoggingStreamFactory factory = iter->second;

		// If this throws, then there was no allocation made.
		LoggingStream * const stream = factory(uri);

		// If the insert fails, the heap object must be deleted.
		try {
			streamPoolsByUri.insert(std::make_pair(uri, stream));
		} catch (...) {
			delete stream; // NOP if null
			throw;
		}

		cachedStream = streamPoolsByUri.find(uri);
	}

	return cachedStream->second;
}

using LoggingNamespace = std::vector<std::string>;

void LoggingState::wipeProperties(LoggerTree & theLoggers) {
	printLoggingDebugMessage("wipeProperties called");

	for (LoggerTreeNode & node : theLoggers) {
		node.value.getLogger()->properties.clear();
	}
}

LoggerTree LoggingState::autoConfigure() noexcept {
	printLoggingDebugMessage("autoConfigure called");

	// Attempt normal configuration.
	try {
		const char * configurationFileName = "balau-logging.hconf";
		boost::filesystem::path exeLocation = boost::dll::program_location();
		std::string configurationText;

		if (!boost::filesystem::is_regular_file(exeLocation)) {
			std::cerr << "ERROR: cannot locate executable. "
			          << "Implicit logging system configuration from balau-logging.hconf cannot be performed."
			          << std::endl;
		} else {
			boost::filesystem::path confLocation = exeLocation.parent_path() / configurationFileName;
			if (boost::filesystem::is_regular_file(confLocation)) {
				configurationText = Files::readToString(Resource::File(confLocation));
			}
		}

		return manualConfigure(configurationText, std::map<std::string, std::string>());
	} catch (const std::exception & e) {
		fprintf(stderr, "FATAL: The logging configuration threw an exception:\n");
		fprintf(stderr, "FATAL: %s\n", e.what());
		fprintf(stderr, "FATAL: The logging system is not configured correctly.\n"); // NOLINT
		fprintf(stderr, "FATAL: All log messages log to stdout/stderr.\n"); // NOLINT
	} catch (...) {
		fprintf(stderr, "FATAL: The logging configuration threw an unknown exception.\n"); // NOLINT
		fprintf(stderr, "FATAL: The logging system is not configured correctly.\n"); // NOLINT
		fprintf(stderr, "FATAL: All log messages log to stdout/stderr.\n"); // NOLINT
	}

	// If the normal configuration failed, try to create a fallback default configuration.
	// This should only fail due to fatal out of memory issues.
	try {
		return createDefaultConfiguration();
	} catch (const std::exception & e) {
		fprintf(stderr, "FATAL: Attempt to create default logging configuration failed:\n"); // NOLINT
		fprintf(stderr, "FATAL: %s\n", e.what()); // NOLINT
		fprintf(stderr, "FATAL: The logging system is not configured.\n"); // NOLINT
	} catch (...) {
		fprintf(stderr, "FATAL: Attempt to create default logging configuration failed:\n"); // NOLINT
		fprintf(stderr, "FATAL: The logging system is not configured.\n"); // NOLINT
	}

	// Otherwise the logging system is unconfigured and in a fatal state.
	return LoggerTree(LoggerHolder(std::shared_ptr<Logger>()));
}

LoggerTree LoggingState::manualConfigure(std::string_view configurationText,
                                         const std::map<std::string, std::string> & encasedPlaceholders) {
	printLoggingDebugMessage("manualConfigure called");
	LoggerTree theLoggers = createDefaultConfiguration();

	if (!configurationText.empty()) {
		std::string configurationTextExpanded = expandConfigurationTextMacros(configurationText, encasedPlaceholders);
		LoggerTree newLoggers = parseConfiguration(configurationTextExpanded);
		cascadeAndConfigureLoggers(theLoggers, newLoggers);
	} else {
		configureLoggers(theLoggers);
	}

	return theLoggers;
}

LoggerTree LoggingState::manualConfigure(const EnvironmentProperties & configuration,
                                         const std::map<std::string, std::string> & encasedPlaceholders) {
	printLoggingDebugMessage("manualConfigure called");
	LoggerTree theLoggers = createDefaultConfiguration();

	if (configuration.begin() != configuration.end()) {
		const auto configurationExpanded = expandConfigurationTextMacros(configuration, encasedPlaceholders);
		LoggerTree newLoggers = parseConfiguration(configurationExpanded);
		cascadeAndConfigureLoggers(theLoggers, newLoggers);
	} else {
		configureLoggers(theLoggers);
	}

	return theLoggers;
}

std::string LoggingState::expandConfigurationTextMacros(std::string_view configurationText,
                                                        const std::map<std::string, std::string> & encasedPlaceholders) {
	const boost::filesystem::path exeLocation = boost::dll::program_location();
	const std::string homeDir = User::getHomeDirectory().toUriString();
	const std::string executableName = exeLocation.filename().string();

	return expandConfigurationTextMacros(configurationText, exeLocation, homeDir, executableName, encasedPlaceholders);
}

std::string LoggingState::expandConfigurationTextMacros(std::string_view configurationText,
                                                        const boost::filesystem::path & exeLocation,
                                                        const std::string & homeDir,
                                                        const std::string & executableName,
                                                        const std::map<std::string, std::string> & encasedPlaceholders) {
	bool modded;
	size_t count;
	auto text = std::string(configurationText);

	// The supplied encasedPlaceholders are expanded first.

	do {
		modded = false;

		for (const auto & p : encasedPlaceholders) {
			text = Strings::replaceAll(text, p.first, p.second, &count);
			modded |= count != 0;
		}
	} while (modded);

	// The default encasedPlaceholders are expanded last.
	// This allows the supplied placeholders to take precedence.

	do {
		modded = false;
		text = Strings::replaceAll(text, "${user.home}", homeDir, &count);
		modded |= count != 0;
		text = Strings::replaceAll(text, "${executable}", executableName, &count);
		modded |= count != 0;
	} while (modded);

	return text;
}

std::unique_ptr<EnvironmentProperties> LoggingState::expandConfigurationTextMacros(const EnvironmentProperties & configuration,
                                                                                   const std::map<std::string, std::string> & encasedPlaceholders) {
	const boost::filesystem::path exeLocation = boost::dll::program_location();
	const std::string homeDir = User::getHomeDirectory().toUriString();
	const std::string executableName = exeLocation.filename().string();

	auto bindings = std::make_unique<::Balau::Impl::BindingMap>();

	for (auto item : configuration) {
		std::string_view name = item.getName();

		if (item.isComposite()) {
			auto composite = item.getComposite();

			bindings->put(
				  item.getKey()
				, expandConfigurationValueTextMacros(
					item.getKey(), composite, exeLocation, homeDir, executableName, encasedPlaceholders
				)
			);
		} else {
			fprintf(stderr, "WARN: ignoring root value property in logging configuration: %s\n", std::string(name).c_str());
		}
	}

	return std::make_unique<EnvironmentProperties>(std::move(bindings));
}

std::unique_ptr<::Balau::Impl::AbstractBinding>
LoggingState::expandConfigurationValueTextMacros(::Balau::Impl::BindingKey key,
                                                 const std::shared_ptr<EnvironmentProperties> & configuration,
                                                 const boost::filesystem::path & exeLocation,
                                                 const std::string & homeDir,
                                                 const std::string & executableName,
                                                 const std::map<std::string, std::string> & encasedPlaceholders) {
	auto bindings = std::make_unique<::Balau::Impl::BindingMap>();

	for (auto item : *configuration) {
		std::string_view name = item.getName();

		if (item.isComposite()) {
			fprintf(stderr, "WARN: ignoring descendant composite property in logging configuration: %s\n", std::string(name).c_str());
		} else if (item.isValue<std::string>()) {
			auto value = item.getValue<std::string>();

			value = expandConfigurationTextMacros(value, exeLocation, homeDir, executableName, encasedPlaceholders);

			bindings->put(
				  item.getKey()
				, std::unique_ptr<::Balau::Impl::AbstractBinding>(new ::Balau::Impl::PrototypeBinding<std::string>(item.getKey(), value))
			);
		} else if (item.isValue<bool>()) {
			auto value = item.getValue<bool>();
			bindings->put(
				  item.getKey()
				, std::unique_ptr<::Balau::Impl::AbstractBinding>(new ::Balau::Impl::PrototypeBinding<bool>(item.getKey(), value))
			);
		} else {
			fprintf(stderr, "WARN: unknown logging configuration value property: %s\n", std::string(name).c_str());
		}
	}

	return std::unique_ptr<::Balau::Impl::AbstractBinding>(
		new ::Balau::Impl::ProvidedSingletonBinding<EnvironmentProperties>(
			std::move(key), std::make_shared<EnvironmentProperties>(std::move(bindings))
		)
	);
}

LoggerTree LoggingState::createDefaultConfiguration() {
	return LoggerTree(
		LoggerHolder(
			std::shared_ptr<Logger>(
				new Logger(
					  ""
					, ""
					, ""
					, {
						  "level", "info"
						, "warn-stream", "stdout"
						, "error-stream", "stderr"
						, "format", "%Y-%m-%d %H:%M:%S [%thread] %LEVEL - %namespace - %message"
					}
				)
			)
		)
	);
}

// TODO what changes are required in order to allow move instead of copy?

void LoggingState::cascadeAndConfigureLoggers(LoggerTree & theLoggers, LoggerTree & newLoggers) {
	printLoggingDebugMessage("cascadeAndConfigureLoggers called");

	#if BALAU_LOGGING__PRINT_LOGGERS_IN_BETWEEN_CONFIGURE_STAGES
	std::cout << "\n\n---------------------------------------------------" << std::endl;
		std::cout << "theLoggers: \n" << theLoggers << std::endl;
		std::cout << "newLoggers: \n" << newLoggers << std::endl;
	#endif

	theLoggers.cascade(newLoggers);

	#if BALAU_LOGGING__PRINT_LOGGERS_IN_BETWEEN_CONFIGURE_STAGES
	std::cout << "------------------ after cascade ------------------" << std::endl;
		std::cout << "theLoggers: \n" << theLoggers << std::endl;
	#endif

	configureLoggers(theLoggers);
}

void LoggingState::configureLoggers(LoggerTree & theLoggers) {
	printLoggingDebugMessage("configureLoggers called");

	propagateConfiguration(theLoggers);

	#if BALAU_LOGGING__PRINT_LOGGERS_IN_BETWEEN_CONFIGURE_STAGES
	std::cout << "----------- after propagateConfiguration ----------" << std::endl;
		std::cout << "theLoggers: \n" << theLoggers << std::endl;
	#endif

	setLevels(theLoggers);

	#if BALAU_LOGGING__PRINT_LOGGERS_IN_BETWEEN_CONFIGURE_STAGES
	std::cout << "----------------- after setLevels -----------------" << std::endl;
		std::cout << "theLoggers: \n" << theLoggers << std::endl;
	#endif

	setShouldFlush(theLoggers);

	#if BALAU_LOGGING__PRINT_LOGGERS_IN_BETWEEN_CONFIGURE_STAGES
	std::cout << "----------------- after setShouldFlush -----------------" << std::endl;
		std::cout << "theLoggers: \n" << theLoggers << std::endl;
	#endif

	setStreams(theLoggers);

	#if BALAU_LOGGING__PRINT_LOGGERS_IN_BETWEEN_CONFIGURE_STAGES
	std::cout << "----------------- after setStreams ----------------" << std::endl;
		std::cout << "theLoggers: \n" << theLoggers << std::endl;
	#endif

	setFormats(theLoggers);

	#if BALAU_LOGGING__PRINT_LOGGERS_IN_BETWEEN_CONFIGURE_STAGES
	std::cout << "----------------- after setFormats ----------------" << std::endl;
		std::cout << "theLoggers: \n" << theLoggers << std::endl;
		std::cout << "---------------------------------------------------\n\n" << std::endl;
	#endif
}

void LoggingState::propagateConfiguration(LoggerTree & theLoggers) {
	printLoggingDebugMessage("propagateConfiguration called");
	const std::string streamStr = std::string("stream");

	for (auto loggerIterator = theLoggers.begin(); loggerIterator != theLoggers.end(); ++loggerIterator) {
		LoggerTreeNode & node = *loggerIterator;
		Logger & logger = *node.value.getLogger();
		auto & loggerProps = logger.properties;
		size_t ancestorLevel = 1;
		LoggerTreeNode * parent = loggerIterator.getAncestor(ancestorLevel++);

		while (parent != nullptr) {
			Logger & parentLogger = *parent->value.getLogger();
			auto & parentProps = parentLogger.properties;

			for (auto & parentProp : parentProps) {
				if (Strings::endsWith(parentProp.first, streamStr)) {
					// Special treatment is required for stream properties.
					// Do not propagate the parent's stream properties if
					// there is a single stream property in the child.
					for (auto & loggerProp : loggerProps) {
						if (Strings::endsWith(loggerProp.first, streamStr)) {
							continue;
						}
					}
				}

				if (loggerProps.find(parentProp.first) == loggerProps.end()) {
					loggerProps[parentProp.first] = parentProp.second;
				}
			}

			parent = loggerIterator.getAncestor(ancestorLevel++);
		}
	}
}

inline std::string removeOptionalQuotes(const std::string & text) {
	std::string ret = text;

	if (!ret.empty() && ret[0] == '"') {
		ret = ret.substr(1);
	}

	if (!ret.empty() && ret.back() == '"') {
		ret = ret.substr(0, ret.length() - 1);
	}

	return ret;
}

void LoggingState::setStreams(LoggerTree & theLoggers) {
	printLoggingDebugMessage("setStreams called");

	for (LoggerTreeNode & node : theLoggers) {
		Logger & logger = *node.value.getLogger();
		auto & p = logger.properties;

		const auto global   = p.find("stream");
		const auto trace    = p.find("trace-stream");
		const auto debug    = p.find("debug-stream");
		const auto info     = p.find("info-stream");
		const auto warn     = p.find("warn-stream");
		const auto error    = p.find("error-stream");

		LoggingStream * stream;

		// Trace
		if (trace != p.end()) {
			stream = getOrCreateStream(removeOptionalQuotes(trace->second));
		} else if (global != p.end()) {
			stream = getOrCreateStream(removeOptionalQuotes(global->second));
		} else if (debug != p.end()) {
			stream = getOrCreateStream(removeOptionalQuotes(debug->second));
		} else if (info != p.end()) {
			stream = getOrCreateStream(removeOptionalQuotes(info->second));
		} else if (warn != p.end()) {
			stream = getOrCreateStream(removeOptionalQuotes(warn->second));
		} else if (error != p.end()) {
			stream = getOrCreateStream(removeOptionalQuotes(error->second));
		} else {
			stream = getOrCreateStream("stdout");
		}

		logger.streams[Enums::toUnderlying(LoggingLevel::TRACE)] = stream;

		// Debug
		if (debug != p.end()) {
			stream = getOrCreateStream(removeOptionalQuotes(debug->second));
		} else if (global != p.end()) {
			stream = getOrCreateStream(removeOptionalQuotes(global->second));
		} else if (info != p.end()) {
			stream = getOrCreateStream(removeOptionalQuotes(info->second));
		} else if (warn != p.end()) {
			stream = getOrCreateStream(removeOptionalQuotes(warn->second));
		} else if (error != p.end()) {
			stream = getOrCreateStream(removeOptionalQuotes(error->second));
		} else if (trace != p.end()) {
			stream = getOrCreateStream(removeOptionalQuotes(trace->second));
		} else {
			stream = getOrCreateStream("stdout");
		}

		logger.streams[Enums::toUnderlying(LoggingLevel::DEBUG)] = stream;

		// Info
		if (info != p.end()) {
			stream = getOrCreateStream(removeOptionalQuotes(info->second));
		} else if (global != p.end()) {
			stream = getOrCreateStream(removeOptionalQuotes(global->second));
		} else if (warn != p.end()) {
			stream = getOrCreateStream(removeOptionalQuotes(warn->second));
		} else if (error != p.end()) {
			stream = getOrCreateStream(removeOptionalQuotes(error->second));
		} else if (debug != p.end()) {
			stream = getOrCreateStream(removeOptionalQuotes(debug->second));
		} else if (trace != p.end()) {
			stream = getOrCreateStream(removeOptionalQuotes(trace->second));
		} else {
			stream = getOrCreateStream("stdout");
		}

		logger.streams[Enums::toUnderlying(LoggingLevel::INFO)] = stream;

		// Warn
		if (warn != p.end()) {
			stream = getOrCreateStream(removeOptionalQuotes(warn->second));
		} else if (global != p.end()) {
			stream = getOrCreateStream(removeOptionalQuotes(global->second));
		} else if (error != p.end()) {
			stream = getOrCreateStream(removeOptionalQuotes(error->second));
		} else if (info != p.end()) {
			stream = getOrCreateStream(removeOptionalQuotes(info->second));
		} else if (debug != p.end()) {
			stream = getOrCreateStream(removeOptionalQuotes(debug->second));
		} else if (trace != p.end()) {
			stream = getOrCreateStream(removeOptionalQuotes(trace->second));
		} else {
			stream = getOrCreateStream("stdout");
		}

		logger.streams[Enums::toUnderlying(LoggingLevel::WARN)] = stream;

		// Error
		if (error != p.end()) {
			stream = getOrCreateStream(removeOptionalQuotes(error->second));
		} else if (global != p.end()) {
			stream = getOrCreateStream(removeOptionalQuotes(global->second));
		} else if (warn != p.end()) {
			stream = getOrCreateStream(removeOptionalQuotes(warn->second));
		} else if (info != p.end()) {
			stream = getOrCreateStream(removeOptionalQuotes(info->second));
		} else if (debug != p.end()) {
			stream = getOrCreateStream(removeOptionalQuotes(debug->second));
		} else if (trace != p.end()) {
			stream = getOrCreateStream(removeOptionalQuotes(trace->second));
		} else {
			stream = getOrCreateStream("stdout");
		}

		logger.streams[Enums::toUnderlying(LoggingLevel::ERROR)] = stream;
	}
}

void LoggingState::setFormats(LoggerTree & theLoggers) {
	printLoggingDebugMessage("setFormats called");

	for (LoggerTreeNode & node : theLoggers) {
		Logger & logger = *node.value.getLogger();
		auto & p = logger.properties;

		const auto iter = p.find("format");
		std::string format;

		if (iter == p.end()) {
			std::cerr << "LOGGING BUG: Format option missing from logger. Using default format." << std::endl;
			format = "info";
		} else {
			format = removeOptionalQuotes(iter->second);
		}

		LogItemVector * loggerItems = nullptr;

		// Not using a pointer container here, so explicit error management is required.
		try {
			loggerItems = new LogItemVector;

			static const std::regex re(R"(%message|%Y|%y|%m|%d|%H|%M|%S|%thread|%LEVEL|%level|%filename|%filepath|%namespace|%ns|%%|%\")");
			std::smatch sm;

			while (std::regex_search(format, sm, re)) {
				const auto currentPosition = static_cast<unsigned long long>(sm.position());
				std::string text = sm.str();

				// Grab any unmatched text into a text log item.
				if (currentPosition != 0) {
					std::string extraText = format.substr(0, currentPosition);
					auto textItem = getOrCreateLogItem(extraText);
					loggerItems->emplace_back(textItem);
				}

				if (text == "%Y") {
					loggerItems->emplace_back(allLogItems.fourDigitYearLoggerItem);
				} else if (text == "%y") {
					loggerItems->emplace_back(allLogItems.twoDigitYearLoggerItem);
				} else if (text == "%m") {
					loggerItems->emplace_back(allLogItems.monthLoggerItem);
				} else if (text == "%d") {
					loggerItems->emplace_back(allLogItems.dayLoggerItem);
				} else if (text == "%H") {
					loggerItems->emplace_back(allLogItems.hourLoggerItem);
				} else if (text == "%M") {
					loggerItems->emplace_back(allLogItems.minuteLoggerItem);
				} else if (text == "%S") {
					loggerItems->emplace_back(allLogItems.secondLoggerItem);
				} else if (text == "%thread") {
					loggerItems->emplace_back(allLogItems.threadLoggerItem);
				} else if (text == "%LEVEL") {
					loggerItems->emplace_back(allLogItems.uppercaseLevelLoggerItem);
				} else if (text == "%level") {
					loggerItems->emplace_back(allLogItems.lowercaseLevelLoggerItem);
				} else if (text == "%namespace") {
					loggerItems->emplace_back(allLogItems.namespaceLoggerItem);
				} else if (text == "%ns") {
					loggerItems->emplace_back(allLogItems.nsLoggerItem);
				} else if (text == "%filename") {
					loggerItems->emplace_back(allLogItems.filenameLoggerItem);
				} else if (text == "%filepath") {
					loggerItems->emplace_back(allLogItems.filePathLoggerItem);
				} else if (text == "%message") {
					loggerItems->emplace_back(allLogItems.messageLoggerItem);
				} else if (text == "%%") {
					loggerItems->emplace_back(allLogItems.percentCharacterLoggerItem);
				} else if (text == "%\"") {
					loggerItems->emplace_back(allLogItems.doubleQuotationLoggerItem);
				} else {
					std::cerr << "LOGGING CONFIGURATION ERROR: Unknown logging format specification: "
					          << text << ".\n" << std::endl;
					loggerItems->emplace_back(std::make_shared<StringLogItem>(std::move(text)));
				}

				format = sm.suffix();
			}

			// Grab any remaining unmatched text into a text log item.
			if (!format.empty()) {
				auto textItem = getOrCreateLogItem(format);
				loggerItems->emplace_back(textItem);
			}

			logger.logItems = lookupOrCacheItemVector(loggerItems);
		} catch (...) {
			delete loggerItems;
			throw;
		}
	}
}

LoggerTree LoggingState::parseConfiguration(const std::string & configurationText) {
	printLoggingDebugMessage("parseConfiguration called with text:\n" + configurationText);
	Resource::StringUri input(configurationText);
	Lang::Property::AST::Properties properties = Lang::Property::PropertyParserService::parse(input);
	LoggerPropertyVisitor visitor;
	return visitor.visit(properties);
}

LoggerTree LoggingState::parseConfiguration(const std::unique_ptr<EnvironmentProperties> & configuration) {
	printLoggingDebugMessage("parseConfiguration called with environment properties object\n");
	LoggerConfigurationVisitor visitor;
	return visitor.visit(*configuration);
}

void LoggingState::setLevels(LoggerTree & theLoggers) {
	printLoggingDebugMessage("setLevels called");

	for (LoggerTreeNode & node : theLoggers) {
		auto & level = node.value.getLogger()->level;
		const auto iter = node.value.getLogger()->properties.find("level");

		if (iter == node.value.getLogger()->properties.end()) {
			level.store(LoggingLevel::INFO);
		} else if (Strings::toLower(iter->second) == "trace") {
			level.store(LoggingLevel::TRACE);
		} else if (Strings::toLower(iter->second) == "debug") {
			level.store(LoggingLevel::DEBUG);
		} else if (Strings::toLower(iter->second) == "info") {
			level.store(LoggingLevel::INFO);
		} else if (Strings::toLower(iter->second) == "warn") {
			level.store(LoggingLevel::WARN);
		} else if (Strings::toLower(iter->second) == "error") {
			level.store(LoggingLevel::ERROR);
		} else if (Strings::toLower(iter->second) == "none") {
			level.store(LoggingLevel::NONE);
		} else {
			level.store(LoggingLevel::INFO);
		}
	}
}

void LoggingState::setShouldFlush(LoggerTree & theLoggers) {
	printLoggingDebugMessage("setShouldFlush called");

	for (LoggerTreeNode & node : theLoggers) {
		auto & shouldFlush = node.value.getLogger()->shouldFlush;
		const auto iter = node.value.getLogger()->properties.find("flush");
		const auto sf = iter != node.value.getLogger()->properties.end() && Strings::toLower(iter->second) == "false";
		shouldFlush.store(!sf);
	}
}

} // namespace LoggingSystem

} // namespace Balau
