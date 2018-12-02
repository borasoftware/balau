// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

///
/// @file BalauException.hpp
///
/// Base %Balau exception classes.
///

#ifndef COM_BORA_SOFTWARE__BALAU_EXCEPTION__BALAU_EXCEPTION
#define COM_BORA_SOFTWARE__BALAU_EXCEPTION__BALAU_EXCEPTION

#include <Balau/Type/ToString.hpp>

#include <exception>

///
/// Throw a %Balau style exception, with implicit file and line number.
///
#define ThrowBalauException(ExceptionClass, ...) throw ExceptionClass(__FILE__, __LINE__, __VA_ARGS__)

namespace Balau::Exception {

///
/// Base class of all %Balau exceptions.
///
class BalauException : public std::exception {
	///
	/// The message.
	///
	public: const std::string message;

	///
	/// The full message (includes file and line information).
	///
	public: const std::string fullMessage;

	public: const char * what() const noexcept override {
		return fullMessage.c_str();
	}

	protected: BalauException(const char * file, int line, const std::string & name, const std::string & text)
		: message(::toString(name, " - ", text))
		  , fullMessage(::toString(stripFilePath(file), ":", line, " - ", name, " - ", text)) {}

	// This may need to be reimplemented for other platforms.
	private: static std::string stripFilePath(const char * file) {
		const char * lastForward = strrchr(file, '/');

		if (lastForward != nullptr) {
			return std::string(lastForward + 1);
		} else {
			return file;
		}
	}
};

///
/// Base class comparison function for Balau exceptions.
///
/// This function does not compare file/line numbers.
///
/// This should be overloaded for exceptions that have data to compare.
///
inline bool operator == (const BalauException & lhs, const BalauException & rhs) {
	return typeid(lhs) == typeid(rhs) && lhs.message == rhs.message;
}

///
/// Base class toString function for Balau exceptions.
///
/// This should be overloaded for exceptions that have data to print.
///
inline std::string toString(const BalauException & e) {
	return e.fullMessage;
}

///
/// Thrown when an illegal argument is passed to a function or method.
///
class IllegalArgumentException : public BalauException {
	public: IllegalArgumentException(const char * file, int line, const std::string & text)
		: BalauException(file, line, "IllegalArgument", text) {}
};

///
/// Thrown when a feature is not yet implemented.
///
class NotImplementedException : public BalauException {
	public: NotImplementedException(const char * file, int line, const std::string & text)
		: BalauException(file, line, "NotImplemented", text) {}
};

///
/// Thrown when a conversion fails.
///
class ConversionException : public BalauException {
	public: ConversionException(const char * file, int line, const std::string & text)
		: BalauException(file, line, "Conversion", text) {}
};

///
/// Thrown when a bug is encountered (mainly unimplemented switch cases).
///
class BugException : public BalauException {
	public: BugException(const char * file, int line, const std::string & text)
		: BalauException(file, line, "Bug", text) {}
};

} // namespace Balau::Exception

///
/// Base class toString function for Balau exceptions.
///
/// This should be overloaded for exceptions that have data to print.
///
inline std::string toString(const Balau::Exception::BalauException & e) {
	return e.what();
}

inline std::string toString(const Balau::Exception::IllegalArgumentException & e) {
	return e.what();
}

inline std::string toString(const Balau::Exception::NotImplementedException & e) {
	return e.what();
}

inline std::string toString(const Balau::Exception::BugException & e) {
	return e.what();
}

#endif // COM_BORA_SOFTWARE__BALAU_EXCEPTION__BALAU_EXCEPTION
