// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include "FileTest.hpp"
#include "../../TestResources.hpp"

#include "../../../../main/cpp/Balau/Resource/Http.hpp"

namespace Balau {

using Testing::assertThat;
using Testing::isGreaterThan;
using Testing::isLessThan;
using Testing::is;

namespace Resource {

void FileTest::iteration() {
	File root(TestResources::BalauSourceFolder / "doc" / "manual");
	auto iterator = root.recursiveFileIterator();

	size_t regularFileCount = 0;
	size_t regularFileCumulativeSize = 0;
	size_t directoryFileCount = 0;

	while (iterator.hasNext()) {
		auto file = iterator.next();

		if (file.isRegularFile()) {
			++regularFileCount;
			regularFileCumulativeSize += file.size();
		} else if (file.isRegularDirectory()) {
			++directoryFileCount;
		}
	}

	assertThat(regularFileCount, isGreaterThan(38U));
	assertThat(directoryFileCount, isGreaterThan(10U));

	assertThat(regularFileCount, isLessThan(100U));
	assertThat(directoryFileCount, isLessThan(100U));

	assertThat(regularFileCumulativeSize, isGreaterThan(100000U));
	assertThat(regularFileCumulativeSize, isLessThan(100000000U));
}

void FileTest::resolve() {
	auto source = TestResources::BalauSourceFolder.toAbsolutePath().toRawString();
	auto target = TestResources::BalauTargetFolder;

	// Prefixed with file schema - absolute base folder, relative path
	assertThat(*target.resolve("file:../d/e.html"), is(File(target.getParentDirectory() / "d" / "e.html")));

	// Prefixed with file schema - relative base folder, relative path
	assertThat(*File("../bin").resolve("file:../d/e.html"), is(File("..") / "d" / "e.html"));

	// Prefixed with file schema - absolute base folder, absolute path
	assertThat(*target.resolve("file://" + source), is(File(source)));

	// Prefixed with file schema - relative base folder, absolute path
	assertThat(*File("../bin").resolve("file://" + source), is(File(source)));

	// Prefixed with file schema - absolute base folder, empty path
	assertThat(*target.resolve("file://"), is(target));

	// Prefixed with file schema - relative base folder, empty path
	assertThat(*File("../bin").resolve("file://"), is(File("../bin")));

	/////////////////////////

	// Prefixed with file schema - absolute base file, relative path
	assertThat(*File("/a/b/c.html").resolve("file:../d/e.html"), is(File("/a/d/e.html")));
	assertThat(*File("/a/b/c.html").resolve("file:d/e.html"), is(File("/a/b/d/e.html")));

	// Prefixed with file schema - relative base file, relative path
	assertThat(*File("a/b/c.html").resolve("file:../d/e.html"), is(File("a/d/e.html")));
	assertThat(*File("a/b/c.html").resolve("file:d/e.html"), is(File("a/b/d/e.html")));

	// Prefixed with file schema - absolute base file, absolute path
	assertThat(*File("/a/b/c.html").resolve("file:///d/e.html"), is(File("/d/e.html")));

	// Prefixed with file schema - relative base file, absolute path
	assertThat(*File("a/b/c.html").resolve("file:///d/e.html"), is(File("/d/e.html")));

	// Prefixed with file schema - absolute base file, empty path
	assertThat(*File("/a/b/c.html").resolve("file://"), is(File("/a/b")));

	// Prefixed with file schema - relative base file, empty path
	assertThat(*File("a/b/c.html").resolve("file://"), is(File("a/b")));

	// Starts with non-file schema.
	assertThat(*File("a/b/c.html").resolve("http://d/e/f"), is(Http("http://d/e/f")));

	/////////////////////////

	// No scheme prefix - absolute base folder, relative path
	assertThat(*target.resolve("../d/e.html"), is(File(target.getParentDirectory() / "d" / "e.html")));

	// No scheme prefix - relative base folder, relative path
	assertThat(*File("../bin").resolve("../d/e.html"), is(File("..") / "d" / "e.html"));

	// No scheme prefix - absolute base folder, absolute path
	assertThat(*target.resolve(source), is(File(source)));

	// No scheme prefix - relative base folder, absolute path
	assertThat(*File("../bin").resolve(source), is(File(source)));

	// No scheme prefix - absolute base folder, empty path
	assertThat(*target.resolve(""), is(File(target)));

	// No scheme prefix - relative base folder, empty path
	assertThat(*File("../bin").resolve(""), is(File("../bin")));

	/////////////////////////

	// No scheme prefix - absolute base file, relative path
	assertThat(*File("/a/b/c.html").resolve("../d/e.html"), is(File("/a/d/e.html")));
	assertThat(*File("/a/b/c.html").resolve("d/e.html"), is(File("/a/b/d/e.html")));

	// No scheme prefix - relative base file, relative path
	assertThat(*File("a/b/c.html").resolve("../d/e.html"), is(File("a/d/e.html")));
	assertThat(*File("a/b/c.html").resolve("d/e.html"), is(File("a/b/d/e.html")));

	// No scheme prefix - absolute base file, absolute path
	assertThat(*File("/a/b/c.html").resolve("///d/e.html"), is(File("/d/e.html")));

	// No scheme prefix - relative base file, absolute path
	assertThat(*File("a/b/c.html").resolve("/d/e.html"), is(File("/d/e.html")));

	// No scheme prefix - absolute base file, empty path
	assertThat(*File("/a/b/c.html").resolve(""), is(File("/a/b")));

	// No scheme prefix - relative base file, empty path
	assertThat(*File("a/b/c.html").resolve(""), is(File("a/b")));
}

} // namespace Resource

} // namespace Balau
