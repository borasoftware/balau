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
/// @file ThreadName.hpp
///
/// A thread-local name (used by the logging system).
///

#ifndef COM_BORA_SOFTWARE__BALAU_SYSTEM__THREAD_NAME
#define COM_BORA_SOFTWARE__BALAU_SYSTEM__THREAD_NAME

#include <mutex>
#include <string>
#include <unordered_set>

namespace Balau::System {

///
/// A thread-local name.
///
/// This implementation is very simple and only allows access to the thread
/// name from the thread itself. It exists mainly to support logging, but
/// may be used for other applications that do not need to access the names
/// of other threads.
///
/// It is the responsibility of the end developer to ensure the same name is
/// not used for multiple threads.
///
class ThreadName {
	///
	/// Get the name of the calling thread.
	///
	public: static const std::string & getName();

	///
	/// Set the name of the calling thread.
	///
	public: static void setName(const std::string & name);

	///////////////////////// Private implementation //////////////////////////

	private: static thread_local std::string threadName;
};

} // namespace Balau::System

#endif // COM_BORA_SOFTWARE__BALAU_SYSTEM__THREAD_NAME
