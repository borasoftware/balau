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

#include <Balau/Type/SourceCodeLocation.hpp>
#include <Balau/Util/Macros.hpp>

#define BalauSourceCodeLocation(F, L) ::Balau::SourceCodeLocation(F ":" _BalauStringExpand(L))

#ifdef BALAU_ENABLE_STACKTRACES
	#include <boost/stacktrace.hpp>
	#include <exception>
	#include <Balau/Util/Impl/StringsImpl.hpp>

	#define _ThrowBalauException_generateStackTrace                              \
		std::ostringstream stStream;                                             \
		stStream << boost::stacktrace::stacktrace();                             \
		std::string st = stStream.str();                                         \
		std::string match = std::string(BalauString(BALAU_BASE_FOLDER_)) + "/";  \
		std::string replacement;                                                 \
		::Balau::Util::Impl::replaceAllImpl(st, match, replacement, nullptr);
#else
	#define _ThrowBalauException_generateStackTrace std::string st;
#endif

///
/// Throw a %Balau style exception, with implicit file and line number, and optional stacktrace.
///
/// If you do not wish to see the ThrowBalau prefix on each of your throw
/// statements in your code, define an alias macro for this macro.
///
#define ThrowBalauException(ExceptionClass, ...) {            \
	_ThrowBalauException_generateStackTrace                   \
	throw ExceptionClass(BalauSourceCodeLocation(__FILE__, __LINE__), st, __VA_ARGS__); \
} _Balau_SwallowSemiColon_()

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

	protected: BalauException(SourceCodeLocation location,
	                          const std::string & st,
	                          const std::string & name,
	                          const std::string & text)
		: message(::toString(name, " - ", text))
		, fullMessage(generateFullMessage(location, st, name, text)) {}

	protected: BalauException(const std::string & st,
	                          const std::string & name,
	                          const std::string & text)
		: message(::toString(name, " - ", text))
		, fullMessage(generateFullMessage(SourceCodeLocation(), st, name, text)) {}

	private: static std::string generateFullMessage(SourceCodeLocation location,
	                                                const std::string & st,
	                                                const std::string & name,
	                                                const std::string & text) {
		std::ostringstream stream;

		if (location.location) {
			stream << stripFilePath(location.location) << " - ";
		}

		stream << name << " - " << text;

		if (!st.empty()) {
			stream << "\n" << st;
		}

		return stream.str();
	}

	// This will need to be reimplemented for the Windows platform.
	private: static std::string stripFilePath(const char * location) {
		const char * lastForward = strrchr(location, '/');

		if (lastForward != nullptr) {
			return std::string(lastForward + 1);
		} else {
			return location;
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
/// Base class toString<AllocatorT> function for Balau exceptions.
///
/// This should be overloaded for exceptions that have data to print.
///
template <typename AllocatorT>
inline Balau::U8String<AllocatorT> toString(const BalauException & e) {
	return toString<AllocatorT>(e.fullMessage);
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
	public: IllegalArgumentException(SourceCodeLocation location, const std::string & st, const std::string & text)
		: BalauException(location, st, "IllegalArgument", text) {}

	public: IllegalArgumentException(const std::string & st, const std::string & text)
		: BalauException(st, "IllegalArgument", text) {}
};

///
/// Thrown when a variable is not in a valid state or when a section of code has been executed at an inappropriate time.
///
class IllegalStateException : public BalauException {
	public: IllegalStateException(SourceCodeLocation location, const std::string & st, const std::string & text)
		: BalauException(location, st, "IllegalState", text) {}

	public: IllegalStateException(const std::string & st, const std::string & text)
		: BalauException(st, "IllegalState", text) {}
};

///
/// Thrown when an operation is deliberately not implemented.
///
class UnsupportedOperationException : public BalauException {
	public: UnsupportedOperationException(SourceCodeLocation location, const std::string & st, const std::string & text)
		: BalauException(location, st, "UnsupportedOperation", text) {}

	public: UnsupportedOperationException(const std::string & st, const std::string & text)
		: BalauException(st, "UnsupportedOperation", text) {}
};

///
/// Thrown when a feature is not yet implemented.
///
class NotImplementedException : public BalauException {
	public: NotImplementedException(SourceCodeLocation location, const std::string & st, const std::string & text)
		: BalauException(location, st, "NotImplemented", text) {}

	public: NotImplementedException(const std::string & st, const std::string & text)
		: BalauException(st, "NotImplemented", text) {}
};

///
/// Thrown when a conversion fails.
///
class ConversionException : public BalauException {
	public: ConversionException(SourceCodeLocation location, const std::string & st, const std::string & text)
		: BalauException(location, st, "Conversion", text) {}

	public: ConversionException(const std::string & st, const std::string & text)
		: BalauException(st, "Conversion", text) {}
};

///
/// Thrown when a bug is encountered (mainly unimplemented switch cases).
///
class BugException : public BalauException {
	public: BugException(SourceCodeLocation location, const std::string & st, const std::string & text)
		: BalauException(location, st, "Bug", text) {}

	public: BugException(const std::string & st, const std::string & text)
		: BalauException(st, "Bug", text) {}
};

} // namespace Balau::Exception

///
/// Catch all to-string function for std::exception.
///
inline std::string toString(const std::exception & e) {
	return e.what();
}

///
/// Catch all to-string function for std::exception.
///
template <typename AllocatorT>
inline Balau::U8String<AllocatorT> toString(const std::exception & e) {
	return toString<AllocatorT>(e.what());
}

#endif // COM_BORA_SOFTWARE__BALAU_EXCEPTION__BALAU_EXCEPTION
