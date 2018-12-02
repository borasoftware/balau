// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include "StringUtf8To32ReadResourceTest.hpp"
#include "../../TestResources.hpp"

#include <Balau/Resource/StringUri.hpp>
#include <Balau/Util/Streams.hpp>

namespace Balau {

using Testing::assertThat;
using Testing::is;

namespace Resource {

void StringUtf8To32ReadResourceTest::test() {
	const std::string text = "some random text";
	const std::u32string expected = ::toString32("some random text");

	StringUri uri("string: " + text);

	auto stringReadResource = uri.getUtf8To32ReadResource();
	auto uriReadResource = uri.utf8To32ReadResource();

	std::u32istream & stringReadStream = stringReadResource.readStream();
	std::u32istream & uriReadStream = uriReadResource->readStream();

	auto actualStringData = ::toString32(stringReadStream);
	auto actualUriData = ::toString32(uriReadStream);

	assertThat(actualStringData, is(expected));
	assertThat(actualUriData, is(expected));
}

} // namespace Resource

} // namespace Balau
