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
/// @file OnScopeExit.hpp
///
/// Run a function on scope exit.
///

#ifndef COM_BORA_SOFTWARE_BALAU_TYPE_ON_SCOPE_EXIT
#define COM_BORA_SOFTWARE_BALAU_TYPE_ON_SCOPE_EXIT

#include <functional>

namespace Balau {

///
/// Run a function on scope exit.
///
/// This class should be used as a stack based instance. The template constructor
/// takes a function pointer or lambda expression. This function is run when the
/// scope within which the instance is defined exits.
///
/// This version of on-scope-exit cannot be moved, other than via C++17 guaranteed
/// copy ellison.
///
class OnScopeExit final {
	///
	/// Run the function on scope exit.
	///
	public: explicit OnScopeExit(std::function<void ()> function_)
		: function(std::move(function_)) {}

	///
	/// Run the scope exit function and destroy the OnScopeExit instance.
	///
	public: ~OnScopeExit() {
		function();
	}

	///
	/// Set to the function to NOP (cancels scheduled execution).
	///
	public: void clear() {
		function = [] () {};
	}

	///
	/// Set to the function to a different function (cancels previously scheduled execution).
	///
	public: void rest(std::function<void ()> function_) {
		function = function_;
	}

	///
	/// Execute the function now and then clear it.
	///
	public: void executeNow() {
		function();
		clear();
	}

	////////////////////////// Private implementation /////////////////////////

	public: OnScopeExit() = delete;
	public: OnScopeExit(const OnScopeExit &) = delete;
	public: OnScopeExit & operator = (const OnScopeExit &) = delete;

	private: std::function<void ()> function;
};

} // namespace Balau

#endif // COM_BORA_SOFTWARE_BALAU_TYPE_ON_SCOPE_EXIT
