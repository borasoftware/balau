// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include <memory>

struct A {
	explicit A(int data_) : data(data_) {}
	int data;
};

struct B {
	explicit B(int data_) : data(data_) {}
	int data;
};

int main() {
	auto a = std::make_shared<A>(42);
	auto b = std::reinterpret_pointer_cast<B>(a);

	if (a->data != b->data) {
		throw std::exception();
	}

	return 0;
}
