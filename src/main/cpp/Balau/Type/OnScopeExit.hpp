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
/// @file OnScopeExit.hpp
///
/// Run a function on scope exit.
///

#ifndef COM_BORA_SOFTWARE__BALAU_TYPE__ON_SCOPE_EXIT
#define COM_BORA_SOFTWARE__BALAU_TYPE__ON_SCOPE_EXIT

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

#endif // COM_BORA_SOFTWARE__BALAU_TYPE__ON_SCOPE_EXIT
