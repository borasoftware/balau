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

#ifndef COM_BORA_SOFTWARE__BALAU_LOGGING_IMPL__LOGGER_ALLOCATOR
#define COM_BORA_SOFTWARE__BALAU_LOGGING_IMPL__LOGGER_ALLOCATOR

#include <Balau/Type/ToString.hpp>

#include <cstddef>

namespace Balau::LoggingSystem {

#ifdef BALAU_ENABLE_THREAD_LOCAL_LOGGING_ALLOCATOR

#ifndef BALAU_LOGGING_THREAD_LOCAL_ALLOCATOR_BUFFER_SIZE_KB
	#define BALAU_LOGGING_THREAD_LOCAL_ALLOCATOR_BUFFER_SIZE_KB 10
#endif

/////// Deterministic, thread local allocation ///////

//
// This allocator helper works by allocating memory on a predefined,
// thread local buffer. If the requested allocation size exceeds the
// remaining memory available in the buffer, the standard new/delete
// operators are used instead.
//
// The idea is that the logging system will not use heap based storage
// as long as the total memory used in the logging call does not exceed
// the buffer size. The result is thus a more deterministic execution
// time of logging calls.
//
// Note that this will only result in the log message construction code
// using the custom allocator. It does not extend to the logging stream
// implementations. Due to this, if realtime logging is required, a
// suitable logging stream should be chosen, in addition to turning the
// stack based logging allocator on.
//
class LoggerAllocatorState {
	///
	/// The fixed buffer size per thread.
	///
	/// Each thread will have a permanent buffer of this size.
	///
	public: static const size_t bufferSize = 1024 * 10;

	//
	// Resets the pointer for a new logging message call.
	//
	public: void startLogAllocation() {
		currentFree = buffer;
	}

	public: char * allocate(size_t n) {
		const size_t blockSize = getBlockSize(n);

		if ((ptrdiff_t) blockSize < buffer + bufferSize - currentFree) {
			// The new block fits in the buffer.
			char * ret = currentFree;
			currentFree += blockSize;
			return ret;
		} else {
			// The new block does not fit in the buffer.. allocate on the heap.
			++overAlloc;
			return static_cast<char *>(::operator new(blockSize));
		}
	}

	public: void deallocate(char * block, size_t n) noexcept {
		if (block >= buffer && block < buffer + bufferSize) {
			const size_t blockSize = getBlockSize(n);

			// Only deallocate if this is the last allocated block.
			if (block + blockSize == currentFree)
				currentFree = block;
		} else {
			// The block was allocated on the heap.
			::operator delete(block);
		}
	}

	public: unsigned long long overAllocationCount() const {
		return overAlloc;
	}

	///////////////////////// Private implementation //////////////////////////

	//
	// Blocks are allocated in accordance with the alignment.
	//
	private: static size_t getBlockSize(size_t n) {
		const size_t align = alignof(std::max_align_t);
		const size_t offset = n;
		const size_t aligned = (offset + (align - 1)) & ~(align - 1);
		return aligned;
	}

	private: alignas(std::max_align_t) char buffer[bufferSize];
	private: char * currentFree;
	private: unsigned long long overAlloc = 0;
};

extern thread_local LoggerAllocatorState loggerAllocatorState;

//
// Allocator that uses the thread local allocator state.
//
template <typename T> struct LoggerAllocator {
	using value_type = T;
	using propagate_on_container_move_assignment = std::true_type;
	using is_always_equal = std::true_type;

	LoggerAllocator() noexcept = default;
	LoggerAllocator(const LoggerAllocator &) noexcept = default;
	template <typename U> LoggerAllocator(const LoggerAllocator<U> &) noexcept {}
	LoggerAllocator & operator = (const LoggerAllocator &) noexcept = default;

	T * allocate(size_t n) {
		return reinterpret_cast<T *>(loggerAllocatorState.allocate(n * sizeof(T)));
	}

	void deallocate(T * p, size_t n) {
		loggerAllocatorState.deallocate(reinterpret_cast<char *>(p), n);
	}
};

template <typename T>
inline bool operator == (const LoggerAllocator<T> & lhs, const LoggerAllocator<T> & rhs) noexcept {
	return true;
}

template <typename T>
inline bool operator != (const LoggerAllocator<T> & lhs, const LoggerAllocator<T> & rhs) noexcept {
	return false;
}

using LoggerString = std::basic_string<char, std::char_traits<char>, LoggerAllocator<char>>;
using LoggerStringVector = std::vector<LoggerString, LoggerAllocator<LoggerString>>;
using LoggerOStringStream = std::basic_ostringstream<char, std::char_traits<char>, LoggerAllocator<char>>;

template <typename T> inline LoggerString toLoggerString(const T & value) {
	using ::toString;

	// Compilation error? Ensure that a suitable toString<Allocator> function
	// is available before the logger header file is included.
	return toString<LoggerAllocator<char>>(value);
}

inline void startLogAllocationImpl() {
	loggerAllocatorState.startLogAllocation();
}

inline unsigned long long overAllocationCountImpl() {
	return loggerAllocatorState.overAllocationCount();
}

#else // BALAU_ENABLE_THREAD_LOCAL_LOGGING_ALLOCATOR

/////////////// Default allocation //////////////

using LoggerString = std::string;
using LoggerStringVector = std::vector<LoggerString>;
using LoggerOStringStream = std::ostringstream;

template <typename T>
inline LoggerString toLoggerString(const T & value) {
	using ::toString;

	// Compilation error? Ensure that a suitable toString function
	// is available before the logger header file is included.
	return toString(value);
}

inline void startLogAllocationImpl() {
}

inline unsigned long long overAllocationCountImpl() {
	return 0;
}

#endif // BALAU_ENABLE_THREAD_LOCAL_LOGGING_ALLOCATOR

inline void makeStringVector2(LoggerStringVector & vector) {
}

template <typename ... StringT>
inline void makeStringVector2(LoggerStringVector & vector, LoggerString && first, StringT && ... remaining) {
	vector.emplace_back(std::forward<LoggerString>(first));
	makeStringVector2(vector, std::forward<StringT>(remaining) ...);
}

template <typename T, typename ... MT>
inline LoggerStringVector makeStringVector(const T & first, const MT & ... remaining) {
	LoggerStringVector vector;
	makeStringVector2(vector, toLoggerString(first), toLoggerString(remaining) ...);
	return vector;
}

////////////////////////////////// Public API /////////////////////////////////

///
/// Reset the allocator if the custom allocator is enabled.
///
inline void startLogAllocation() {
	startLogAllocationImpl();
}

///
/// Get the number of times that the allocator ran out of buffer and thus called operator new for the current thread.
///
inline unsigned long long overAllocationCount() {
	return overAllocationCountImpl();
}

} // namespace Balau::LoggingSystem

#endif // COM_BORA_SOFTWARE__BALAU_LOGGING_IMPL__LOGGER_ALLOCATOR
