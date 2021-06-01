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

///
/// @file ThreadName.hpp
///
/// A thread-local name (used by the logging system).
///

#ifndef COM_BORA_SOFTWARE__BALAU_SYSTEM__THREAD_NAME
#define COM_BORA_SOFTWARE__BALAU_SYSTEM__THREAD_NAME

#include <Balau/Util/Strings.hpp>

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
	public: static const std::string & getName() {
		return threadName();
	}

	///
	/// Set the name of the calling thread.
	///
	public: static void setName(const std::string & name) {
		threadName() = ::toString(Util::Strings::trim(name));
	}

	///////////////////////// Private implementation //////////////////////////

	private: static std::string & threadName() {
		thread_local std::string instance;
		return instance;
	}
};

} // namespace Balau::System

#endif // COM_BORA_SOFTWARE__BALAU_SYSTEM__THREAD_NAME
