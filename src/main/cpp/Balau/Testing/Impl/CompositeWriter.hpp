// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#ifndef COM_BORA_SOFTWARE__BALAU_TESTING_IMPL__COMPOSITE_WRITER
#define COM_BORA_SOFTWARE__BALAU_TESTING_IMPL__COMPOSITE_WRITER

#include <Balau/Testing/Writers.hpp>
#include <Balau/Util/Memory.hpp>

namespace Balau::Testing::Impl {

// Contains all the writers specified during test runner construction.
class CompositeWriter {
	public: CompositeWriter() {}

	public: template <typename ... WriterItemT> explicit CompositeWriter(const WriterItemT & ... items_)
		: items(Util::Memory::makeShared<TestWriter>([] (const TestWriter & item) { return item.clone(); }, items_ ...)) {}

	public: CompositeWriter & operator = (const CompositeWriter & copy) {
		items = copy.items;
		return *this;
	}

	public: template<typename T> CompositeWriter & operator << (T value) {
		for (auto & item : items) {
			item->write(value);
		}

		return *this;
	}

	private: std::vector<std::shared_ptr<TestWriter>> items;
};

} // namespace Balau::Testing::Impl

#endif // COM_BORA_SOFTWARE__BALAU_TESTING_IMPL__COMPOSITE_WRITER
