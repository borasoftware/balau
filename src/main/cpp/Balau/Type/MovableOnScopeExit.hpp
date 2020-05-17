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
/// @file MovableOnScopeExit.hpp
///
/// Run a function on scope exit (movable version).
///

#ifndef COM_BORA_SOFTWARE__BALAU_TYPE__MOVEABLE_ON_SCOPE_EXIT
#define COM_BORA_SOFTWARE__BALAU_TYPE__MOVEABLE_ON_SCOPE_EXIT

#include <functional>
#include <memory>

namespace Balau {

///
/// Run a function on scope exit (movable version).
///
/// This class should be used as a stack based instance. The template constructor
/// takes a function pointer or lambda expression. This function is run when the
/// scope within which the instance is defined exits, unless the instance is moved
/// out of the scope.
///
/// If the instance is moved, the old instance will not run the function on destruction.
/// This allows the instance to be set up in an inner scope and moved to the calling
/// scope without the function running at the end of the inner scope.
///
class MovableOnScopeExit final {
	///
	/// Run the function on scope exit.
	///
	public: template <typename FunctionT> explicit MovableOnScopeExit(FunctionT function)
		: exiter(new Impl<FunctionT>(function)) {}

	///
	/// Move the scope exit running to a different instance.
	///
	public: MovableOnScopeExit(MovableOnScopeExit && rhs) noexcept
		: exiter(std::move(rhs.exiter)) {}

	///
	/// Set to the function to NOP (cancels scheduled execution).
	///
	public: void clear() {
		exiter->clear();
	}

	///
	/// Execute the function now and then clear it.
	///
	public: void executeNow() {
		exiter->executeNow();
	}

	////////////////////////// Private implementation /////////////////////////

	public: MovableOnScopeExit(const MovableOnScopeExit &) = delete;
	public: MovableOnScopeExit & operator = (const MovableOnScopeExit &) = delete;

	private: struct ContainerBase {
		virtual ~ContainerBase() = default;
		public: virtual void clear() = 0;
		public: virtual void reset(std::function<void ()> function_) = 0;
		public: virtual void executeNow() = 0;
	};

	private: template <typename FunctionT> struct Impl : public ContainerBase {
		explicit Impl(FunctionT function_) : function(function_) {}

		~Impl() override {
			function();
		}

		Impl(const Impl &) = delete;
		Impl & operator =(const Impl &) = delete;

		public: void clear() override {
			function = [] () {};
		}

		public: void executeNow() override {
			function();
			clear();
		}

		friend class MovableOnScopeExit;
		private: FunctionT function;
	};

	private: std::unique_ptr<ContainerBase> exiter;
};

} // namespace Balau

#endif // COM_BORA_SOFTWARE__BALAU_TYPE__MOVEABLE_ON_SCOPE_EXIT
