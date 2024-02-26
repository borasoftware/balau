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
/// @file Memory.hpp
///
/// Utilities for memory.
///

#ifndef COM_BORA_SOFTWARE_BALAU_UTIL_MEMORY
#define COM_BORA_SOFTWARE_BALAU_UTIL_MEMORY

#include <Balau/Type/ToString.hpp>

#include <cstring>
#include <memory>
#include <vector>

namespace Balau::Util {

///
/// Utilities for memory.
///
struct Memory final {
	////////////////// Make unique - scalar transform version /////////////////

	///
	/// Create a vector of unique pointers after transforming the arguments (scalar transform version).
	///
	/// Given the variadic input reference arguments, create a vector of unique
	/// pointers after transforming the arguments via the supplied function.
	///
	/// @tparam T the common base type of the input arguments
	/// @tparam R the return type of which the vector will hold shared pointers
	/// @tparam P types of the remaining arguments, compatible with the base type T
	///
	/// @param transform the transform function called during emplacement
	/// @param first the first argument to emplace
	/// @param remaining the remaining arguments to emplace
	///
	template <typename T, typename R = T, typename F, typename ... P>
	static std::vector<std::unique_ptr<R>> makeUnique(F transform, const T & first, const P & ... remaining) {
		std::vector<std::unique_ptr<R>> allItems;
		allItems.emplace_back(transform(first));
		std::vector<std::unique_ptr<R>> remainingItems = makeUnique<T, R, F>(transform, remaining ...);
		std::move(remainingItems.begin(), remainingItems.end(), std::back_inserter(allItems));
		return allItems;
	}

	///
	/// [End case] Create a vector of unique pointers after transforming the arguments (scalar transform version).
	///
	/// Given no variadic input reference arguments, create an empty unique
	/// pointer vector.
	///
	/// @tparam T the common base type of the input arguments
	/// @tparam R the return type of which the vector will hold shared pointers
	///
	template <typename T, typename R = T, typename F>
	static std::vector<std::unique_ptr<R>> makeUnique(F ) {
		return std::vector<std::unique_ptr<R>>();
	}

	////////////////// Make unique - vector transform version /////////////////

	///
	/// Create a vector of unique pointers after transforming the arguments (vector transform version).
	///
	/// Given the variadic input reference arguments, create a vector of unique
	/// pointers after transforming the arguments via the supplied function
	/// into a vector.
	///
	/// @tparam T the common base type of the input arguments
	/// @tparam R the return type of which the vector will hold unique pointers
	/// @tparam P types of the remaining arguments, compatible with the base type T
	///
	/// @param transform the scalar to vector transform function called during emplacement
	/// @param first the first argument to emplace
	/// @param remaining the remaining arguments to emplace
	///
	template <typename T, typename R = T, typename F, typename ... P>
	static std::vector<std::unique_ptr<R>> makeUniqueV(F transform, const T & first, const P & ... remaining) {
		std::vector<std::unique_ptr<R>> allItems = transform(first);
		std::vector<std::unique_ptr<R>> remainingItems = makeUniqueV<T, R, F>(transform, remaining ...);
		std::move(remainingItems.begin(), remainingItems.end(), std::back_inserter(allItems));
		return allItems;
	}

	///
	/// [End case] Create a vector of unique pointers after transforming the arguments (vector transform version).
	///
	/// Given no variadic input references, create an empty shared pointer vector.
	///
	/// @tparam T the common base type of the input arguments
	/// @tparam R the return type of which the vector will hold shared pointers
	///
	template <typename T, typename R = T, typename F>
	static std::vector<std::shared_ptr<R>> makeUniqueV(F ) {
		return std::vector<std::shared_ptr<R>>();
	}

	////////////////// Make shared - scalar transform version /////////////////

	///
	/// Create a vector of shared pointers after transforming the arguments (scalar transform version).
	///
	/// Given the variadic input reference arguments, create a vector of shared
	/// pointers after transforming the arguments via the supplied function.
	///
	/// @tparam T the common base type of the input arguments
	/// @tparam R the return type of which the vector will hold shared pointers
	/// @tparam P types of the remaining arguments, compatible with the base type T
	///
	/// @param transform the transform function called during emplacement
	/// @param first the first argument to emplace
	/// @param remaining the remaining arguments to emplace
	///
	template <typename T, typename R = T, typename F, typename ... P>
	static std::vector<std::shared_ptr<R>> makeShared(F transform, const T & first, const P & ... remaining) {
		std::vector<std::shared_ptr<R>> allItems;
		allItems.emplace_back(transform(first));
		std::vector<std::shared_ptr<R>> remainingItems = makeShared<T, R, F>(transform, remaining ...);
		std::move(remainingItems.begin(), remainingItems.end(), std::back_inserter(allItems));
		return allItems;
	}

	///
	/// [End case] Create a vector of shared pointers after transforming the arguments (scalar transform version).
	///
	/// Given no variadic input references, create an empty shared pointer vector.
	///
	/// @tparam T the common base type of the input arguments
	/// @tparam R the return type of which the vector will hold shared pointers
	///
	template <typename T, typename R = T, typename F>
	static std::vector<std::shared_ptr<R>> makeShared(F ) {
		return std::vector<std::shared_ptr<R>>();
	}

	////////////////// Make shared - vector transform version /////////////////

	///
	/// Create a vector of shared pointers after transforming the arguments (vector transform version).
	///
	/// Given the variadic input reference arguments, create a vector of shared
	/// pointers after transforming the arguments via the supplied function
	/// into a vector.
	///
	/// @tparam T the common base type of the input arguments
	/// @tparam R the return type of which the vector will hold shared pointers
	/// @tparam P types of the remaining arguments, compatible with the base type T
	///
	/// @param transform the scalar to vector transform function called during emplacement
	/// @param first the first argument to emplace
	/// @param remaining the remaining arguments to emplace
	///
	template <typename T, typename R = T, typename F, typename ... P>
	static std::vector<std::shared_ptr<R>> makeSharedV(F transform, const T & first, const P & ... remaining) {
		std::vector<std::shared_ptr<R>> allItems = transform(first);
		std::vector<std::shared_ptr<R>> remainingItems = makeSharedV<T, R, F>(transform, remaining ...);
		std::move(remainingItems.begin(), remainingItems.end(), std::back_inserter(allItems));
		return allItems;
	}

	///
	/// [End case] Create a vector of shared pointers after transforming the arguments (vector transform version).
	///
	/// Given no variadic input references, create an empty shared pointer vector.
	///
	/// @tparam T the common base type of the input arguments
	/// @tparam R the return type of which the vector will hold shared pointers
	///
	template <typename T, typename R = T, typename F>
	static std::vector<std::shared_ptr<R>> makeSharedV(F ) {
		return std::vector<std::shared_ptr<R>>();
	}

	///////////////////////////////////////////////////////////////////////////

	Memory() = delete;
	Memory(const Memory &) = delete;
	Memory & operator = (const Memory &) = delete;
};

} // namespace Balau::Util

#endif // COM_BORA_SOFTWARE_BALAU_UTIL_MEMORY
