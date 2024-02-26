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

#ifndef COM_BORA_SOFTWARE_BALAU_LOGGING_IMPL_LOGGING_STATE
#define COM_BORA_SOFTWARE_BALAU_LOGGING_IMPL_LOGGING_STATE

#include <Balau/Exception/LoggingExceptions.hpp>
#include <Balau/Logging/Impl/LoggerHolder.hpp>
#include <Balau/Logging/Impl/LoggerItems.hpp>
#include <Balau/Logging/Impl/LoggingStreams.hpp>
#include <Balau/Util/Files.hpp>

#include <boost/dll/runtime_symbol_info.hpp>

#include <mutex>

namespace Balau {

namespace Impl {

class AbstractBinding;

} // namespace Impl

namespace LoggingSystem {

class LoggingStateHolder;

// The global logging state. All fields and methods are private.
// A single instance of this class is instantiated lazily.
class LoggingState final {
	// The global logging state.
	static LoggingStateHolder & loggingStateHolder();
	static LoggingState & loggingSystemState();

	static std::map<std::string, LoggingStreamFactory> createDefaultStreamFactories();

	static bool printStartupDebugMessageAndReturnFalse();
	static void printLoggingDebugMessage(std::string_view);

	LoggingState();

	std::string generateAbbreviatedNamespace(std::string_view loggerNamespace);

	// Get the logger which corresponds to the specified namespace.
	Logger & getInstance(std::string_view loggingNamespace);

	// Flush all logging streams.
	void flushAll();

	// Reconfigure the logging system from the supplied configuration text.
	void configure(std::string_view configurationText, const std::map<std::string, std::string> & placeholders);

	// Reconfigure the logging system from the supplied configuration.
	void configure(const EnvironmentProperties & configuration, const std::map<std::string, std::string> & placeholders);

	// Reconfigure the logging system to the base configuration.
	void resetConfigure();

	// Lock the logging system configuration, preventing reconfiguration.
	void lockConfiguration(bool throwOnReconfigure = false);

	// Flush all logging streams - lock already acquired.
	void performFlushAll();

	void registerLoggingStreamFactory(const std::string & scheme, LoggingStreamFactory factory);

	// Lookup a log item from the shared pool, creating it if it does not already exist.
	std::shared_ptr<LogItem> getOrCreateLogItem(const std::string & text);

	// Lookup a log item vector from the shared pool, caching the supplied
	// one if a match does not already exist.
	LogItemVector * lookupOrCacheItemVector(LogItemVector * newVector);

	// Lookup a logging stream from the shared pool, creating it if it does not already exist.
	LoggingStream * getOrCreateStream(const std::string & uri);

	// Wipe all the loggers' properties, pending a reconfiguration.
	void wipeProperties(LoggerTree & theLoggers);

	// Try to configure implicitly from the standard logging configuration
	// file and initialise the configuration state.
	LoggerTree autoConfigure() noexcept;

	// Configure explicitly from the supplied configuration text.
	LoggerTree manualConfigure(std::string_view configurationText,
	                           const std::map<std::string, std::string> & encasedPlaceholders);

	// Configure explicitly from the supplied configuration.
	LoggerTree manualConfigure(const EnvironmentProperties & configuration,
	                           const std::map<std::string, std::string> & encasedPlaceholders);

	// Expands the placeholders.
	std::string expandConfigurationTextMacros(std::string_view configurationText,
	                                          const std::map<std::string, std::string> & encasedPlaceholders);

	// Utility method for placeholder expansion.
	std::string expandConfigurationTextMacros(std::string_view configurationText,
	                                          const boost::filesystem::path & exeLocation,
	                                          const std::string & homeDir,
	                                          const std::string & executableName,
	                                          const std::map<std::string, std::string> & encasedPlaceholders);

	// Expands the ${user.home} and ${executable} placeholders.
	std::unique_ptr<EnvironmentProperties> expandConfigurationTextMacros(const EnvironmentProperties & configuration,
	                                                                     const std::map<std::string, std::string> & encasedPlaceholders);

	// Expands the ${user.home} and ${executable} placeholders.
	std::unique_ptr<Impl::AbstractBinding> expandConfigurationValueTextMacros(Impl::BindingKey key,
	                                                                          const std::shared_ptr<EnvironmentProperties> & configuration,
	                                                                          const boost::filesystem::path & exeLocation,
	                                                                          const std::string & homeDir,
	                                                                          const std::string & executableName,
	                                                                          const std::map<std::string, std::string> & encasedPlaceholders);

	// Creates a tree containing the default properties.
	LoggerTree createDefaultConfiguration();

	// Cascade the supplied new loggers onto the current ones and configure the resulting loggers.
	void cascadeAndConfigureLoggers(LoggerTree & theLoggers, LoggerTree & newLoggers);

	// Just configure the supplied new loggers.
	void configureLoggers(LoggerTree & theLoggers);

	// Propagate all qualifying ancestor properties to descendants.
	void propagateConfiguration(LoggerTree & theLoggers);

	// Assigns the streams to the loggers. Streams are pooled and shared between loggers.
	void setStreams(LoggerTree & theLoggers);

	//
	// Sets up the formatters in each logger.
	//
	//  - %Y         - the year as four digits
	//  - %y         - the year as two digits
	//  - %m         - the month as two digits
	//  - %d         - the day of the month
	//  - %H         - the hour as two digits
	//  - %M         - the minute as two digits
	//  - %S         - the seconds as two digits followed by six digits
	//                 representing the microsecond remainder
	//  - %thread    - the thread id or name if one is registered
	//  - %level     - the logging level in lowercase
	//  - %LEVEL     - the logging level in uppercase
	//  - %namespace - the logger's logging namespace
	//  - %ns        - an abbreviation of the logger's logging namespace, created by
	//                 printing each identifier's first letter only apart from the last
	//                 identifier which is printed in its entirety
	//  - %message   - the message, after stringification and combination of all arguments
	//  - %%         - the percent character
	//  - %"         - the double quotation character
	//
	// Example format specification:
	//
	//     "%Y-%m-%d %H:%M:%S [%thread] %LEVEL %namespace - %message"
	//
	void setFormats(LoggerTree & theLoggers);

	// Parse and visit the supplied configuration text and create a configured logger tree.
	LoggerTree parseConfiguration(const std::string & configurationText);

	// Visit the supplied configuration and create a configured logger tree.
	LoggerTree parseConfiguration(const std::unique_ptr<EnvironmentProperties> & configuration);

	// Sets the level variables in the loggers.
	void setLevels(LoggerTree & theLoggers);

	// Sets the shouldFlush variables in the loggers.
	void setShouldFlush(LoggerTree & theLoggers);

	friend class LoggingStateHolder;
	friend class ::Balau::Logger;

	// Is the logging system locked, preventing reconfiguration.
	bool locked = false;

	// Should an exception be thrown if an attempt to reconfigure
	// a locked logging system is made.
	bool noisy = false;

	// All the stateless log items.
	struct AllLogItems {
		std::shared_ptr<LogItem> fourDigitYearLoggerItem    = std::make_shared<FourDigitYearLoggerItem>();
		std::shared_ptr<LogItem> twoDigitYearLoggerItem     = std::make_shared<TwoDigitYearLoggerItem>();
		std::shared_ptr<LogItem> monthLoggerItem            = std::make_shared<MonthLoggerItem>();
		std::shared_ptr<LogItem> dayLoggerItem              = std::make_shared<DayLoggerItem>();
		std::shared_ptr<LogItem> hourLoggerItem             = std::make_shared<HourLoggerItem>();
		std::shared_ptr<LogItem> minuteLoggerItem           = std::make_shared<MinuteLoggerItem>();
		std::shared_ptr<LogItem> secondLoggerItem           = std::make_shared<SecondLoggerItem>();
		std::shared_ptr<LogItem> threadLoggerItem           = std::make_shared<ThreadLoggerItem>();
		std::shared_ptr<LogItem> lowercaseLevelLoggerItem   = std::make_shared<LowercaseLevelLoggerItem>();
		std::shared_ptr<LogItem> uppercaseLevelLoggerItem   = std::make_shared<UppercaseLevelLoggerItem>();
		std::shared_ptr<LogItem> namespaceLoggerItem        = std::make_shared<NamespaceLoggerItem>();
		std::shared_ptr<LogItem> nsLoggerItem               = std::make_shared<NsLoggerItem>();
		std::shared_ptr<LogItem> filenameLoggerItem         = std::make_shared<FilenameLoggerItem>();
		std::shared_ptr<LogItem> filePathLoggerItem         = std::make_shared<FilePathLoggerItem>();
		std::shared_ptr<LogItem> messageLoggerItem          = std::make_shared<MessageLoggerItem>();
		std::shared_ptr<LogItem> percentCharacterLoggerItem = std::make_shared<PercentCharacterLoggerItem>();
		std::shared_ptr<LogItem> doubleQuotationLoggerItem  = std::make_shared<DoubleQuotationLoggerItem>();
	} allLogItems;

	// The shared log items.
	std::map<std::string, std::shared_ptr<LogItem>> logItemPool;

	// The shared pool of log item vectors.
	std::set<LogItemVector *> logItemVectorPool;

	// The shared pool of logging streams.
	std::map<std::string, LoggingStream *> streamPoolsByUri;

	// Logging stream factories.
	std::map<std::string, LoggingStreamFactory> streamFactories;

	// The tree of loggers, arranged according to namespace components.
	LoggerTree loggerTree;
};

} // namespace LoggingSystem

} // namespace Balau

#endif // COM_BORA_SOFTWARE_BALAU_LOGGING_IMPL_LOGGING_STATE
