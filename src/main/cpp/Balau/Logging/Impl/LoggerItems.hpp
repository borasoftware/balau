// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#ifndef COM_BORA_SOFTWARE__BALAU_LOGGING_IMPL__LOGGER_ITEMS
#define COM_BORA_SOFTWARE__BALAU_LOGGING_IMPL__LOGGER_ITEMS

#include <Balau/Logging/Impl/LoggerItemParameters.hpp>
#include <Balau/System/ThreadName.hpp>
#include <Balau/Util/DateTime.hpp>
#include <Balau/Util/Enums.hpp>
#include <Balau/Util/Files.hpp>

#include <thread>

namespace Balau::LoggingSystem {

// Base class of classes which provide predefined logging message text types.
class LogItem {
	public: virtual void write(LoggerItemParameters & parameters) const = 0;
	public: virtual ~LogItem() = default;
};

using LogItemVector = std::vector<std::shared_ptr<LogItem>>;

//
// /////// Log item placeholders ////////
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
//  - %filename  - the source file name
//  - %filepath  - the full path to the source file
//  - %line      - the line number in the source file
//  - %message   - the message, after stringification and combination of all arguments
//  - %%         - the percent character
//  - %"         - the double quotation character
//
// //// Example format specification ////
//
//     "%Y-%m-%d %H:%M:%S [%thread] %LEVEL - %namespace - %message"
//

// Text contained within the format string.
class StringLogItem : public LogItem {
	public: const std::string text;

	public: explicit StringLogItem(std::string text_) : text(std::move(text_)) {}

	public: void write(LoggerItemParameters & parameters) const override {
		parameters.builder << text;
	}
};

// TODO add this class to allow the combined date time formatter to be used.
// The current date.
class DateTimeLoggerItem : public LogItem {
	private: const std::string format;

	public: explicit DateTimeLoggerItem(std::string && format_) : format(std::move(format_)) {}

	public: void write(LoggerItemParameters & parameters) const override {
		Util::DateTime::toString(parameters.builder, format.c_str(), parameters.timePoint);
	}
};

// The current year as four digits.
class FourDigitYearLoggerItem : public LogItem {
	public: void write(LoggerItemParameters & parameters) const override {
		Util::DateTime::toString(parameters.builder, "%Y", parameters.timePoint);
	}
};

// The current year as two digits.
class TwoDigitYearLoggerItem : public LogItem {
	public: void write(LoggerItemParameters & parameters) const override {
		Util::DateTime::toString(parameters.builder, "%y", parameters.timePoint);
	}
};

// The current month.
class MonthLoggerItem : public LogItem {
	public: void write(LoggerItemParameters & parameters) const override {
		Util::DateTime::toString(parameters.builder, "%m", parameters.timePoint);
	}
};

// The current day.
class DayLoggerItem : public LogItem {
	public: void write(LoggerItemParameters & parameters) const override {
		Util::DateTime::toString(parameters.builder, "%d", parameters.timePoint);
	}
};

// The current hour.
class HourLoggerItem : public LogItem {
	public: void write(LoggerItemParameters & parameters) const override {
		Util::DateTime::toString(parameters.builder, "%H", parameters.timePoint);
	}
};

// The current minute.
class MinuteLoggerItem : public LogItem {
	public: void write(LoggerItemParameters & parameters) const override {
		Util::DateTime::toString(parameters.builder, "%M", parameters.timePoint);
	}
};

// The current second and fraction of second.
class SecondLoggerItem : public LogItem {
	public: void write(LoggerItemParameters & parameters) const override {
		Util::DateTime::toString(parameters.builder, "%S", parameters.timePoint);
	}
};

// The thread name if one has been set or the thread id otherwise.
class ThreadLoggerItem : public LogItem {
	public: void write(LoggerItemParameters & parameters) const override {
		const std::string name = System::ThreadName::getName();

		if (!name.empty()) {
			parameters.builder << name;
		} else {
			parameters.builder << std::this_thread::get_id();
		}
	}
};

// The logging level in lowercase.
class LowercaseLevelLoggerItem : public LogItem {
	static const char * lowercaseLevelText[];

	public: void write(LoggerItemParameters & parameters) const override {
		parameters.builder << lowercaseLevelText[Util::Enums::toUnderlying(parameters.level)];
	}
};

// The logging level in uppercase.
class UppercaseLevelLoggerItem : public LogItem {
	static const char * uppercaseLevelText[];

	public: void write(LoggerItemParameters & parameters) const override {
		parameters.builder << uppercaseLevelText[Util::Enums::toUnderlying(parameters.level)];
	}
};

// The logger's logging namespace.
class NamespaceLoggerItem : public LogItem {
	public: void write(LoggerItemParameters & parameters) const override {
		parameters.builder << parameters.nameSpace;
	}
};

// An abbreviation of the logger's logging namespace, created by printing each identifier's
// first letter only apart from the last identifier which is printed in its entirety.
class NsLoggerItem : public LogItem {
	public: void write(LoggerItemParameters & parameters) const override {
		parameters.builder << parameters.ns;
	}
};

// The name of the file in which the log message is found.
class FilenameLoggerItem : public LogItem {
	public: void write(LoggerItemParameters & parameters) const override {
		if (parameters.filename != nullptr) {
			boost::filesystem::path p(parameters.filename);
			parameters.builder << p.filename().string();
		}
	}
};

// The full path of the file in which the log message is found.
class FilePathLoggerItem : public LogItem {
	public: void write(LoggerItemParameters & parameters) const override {
		parameters.builder << parameters.filename;
	}
};

// The line number in the source file where the log message is found.
class LineNumberLoggerItem : public LogItem {
	public: void write(LoggerItemParameters & parameters) const override {
		if (parameters.line != 0) {
			parameters.builder << parameters.line;
		}
	}
};

// The message, after stringification and combination of all arguments.
class MessageLoggerItem : public LogItem {
	public: void write(LoggerItemParameters & parameters) const override {
		parameters.builder << std::string(parameters.message);
	}
};

// The percent character.
class PercentCharacterLoggerItem : public LogItem {
	public: void write(LoggerItemParameters & parameters) const override {
		parameters.builder << "%";
	}
};

// The double quotation character.
class DoubleQuotationLoggerItem : public LogItem {
	public: void write(LoggerItemParameters & parameters) const override {
		parameters.builder << "\"";
	}
};

} // namespace Balau::LoggingSystem

#endif // COM_BORA_SOFTWARE__BALAU_LOGGING_IMPL__LOGGER_ITEMS
