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

#ifndef COM_BORA_SOFTWARE_BALAU_TESTING_IMPL_COMPOSITE_WRITER
#define COM_BORA_SOFTWARE_BALAU_TESTING_IMPL_COMPOSITE_WRITER

#include <Balau/Testing/Writers/TestWriter.hpp>
#include <Balau/Util/Memory.hpp>

namespace Balau::Testing::Impl {

// Contains all the writers specified during test runner construction.
class CompositeWriter {
	public: template <typename ... WriterItemT> explicit CompositeWriter(const WriterItemT & ... items_)
		: items(Util::Memory::makeShared<TestWriter>([] (const TestWriter & item) { return item.clone(); }, items_ ...)) {}

	public: CompositeWriter & operator = (const CompositeWriter & copy) = default;

	public: template<typename T> CompositeWriter & operator << (T value) {
		for (auto & item : items) {
			item->write(value);
		}

		return *this;
	}

	private: std::vector<std::shared_ptr<TestWriter>> items;
};

} // namespace Balau::Testing::Impl

#endif // COM_BORA_SOFTWARE_BALAU_TESTING_IMPL_COMPOSITE_WRITER
