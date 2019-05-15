// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include "TestGroupBase.hpp"
#include "../TestRunner.hpp"

namespace Balau::Testing::Impl {

TestGroupBase::TestGroupBase() : groupIndex(Testing::TestRunner::getGroupIndex()) {}

bool TestGroupBase::currentIsIgnored() {
	return currentIgnored;
}

void TestGroupBase::resetIgnoreCurrent() {
	currentIgnored = false;
}

void TestGroupBase::ignoreCurrent() {
	currentIgnored = true;
}

} // namespace Balau::Testing::Impl
