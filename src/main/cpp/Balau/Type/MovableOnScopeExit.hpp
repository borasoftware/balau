// @formatter:off
//
// Balau core C++ library
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
		: exiter(new ContainerImpl<FunctionT>(function)) {}

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

	private: template <typename FunctionT> struct ContainerImpl : public ContainerBase {
		explicit ContainerImpl(FunctionT function_) : function(function_) {}

		~ContainerImpl() override {
			function();
		}

		ContainerImpl(const ContainerImpl &) = delete;
		ContainerImpl & operator =(const ContainerImpl &) = delete;

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
