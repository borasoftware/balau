// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include <TestResources.hpp>

#ifdef BALAU_ENABLE_HTTP
	#include "../../../../main/cpp/Balau/Resource/Http.hpp"
#endif

namespace Balau {

using Testing::isGreaterThan;
using Testing::isLessThan;
using Testing::is;

namespace Resource {

struct FileTest : public Testing::TestGroup<FileTest> {
	FileTest() {
		registerTest(&FileTest::iteration, "iteration");
		registerTest(&FileTest::resolve,   "resolve");
	}

	void iteration() {
		File root(TestResources::SourceFolder / "doc" / "manual");
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

		AssertThat(regularFileCount, isGreaterThan(38U));
		AssertThat(directoryFileCount, isGreaterThan(10U));

		AssertThat(regularFileCount, isLessThan(100U));
		AssertThat(directoryFileCount, isLessThan(100U));

		AssertThat(regularFileCumulativeSize, isGreaterThan(100000U));
		AssertThat(regularFileCumulativeSize, isLessThan(100000000U));
	}

	void resolve() {
		auto source = TestResources::SourceFolder.toAbsolutePath().toRawString();
		auto target = TestResources::TargetFolder;

		auto fileTestDirectory = TestResources::TestResultsFolder.getParentDirectory() / "fileTestDirectory";
		fileTestDirectory.createDirectories();

		// Prefixed with file schema - absolute base folder, relative path
		AssertThat(*target.resolve("file:../d/e.html"), is(File(target.getParentDirectory() / "d" / "e.html")));

		// Prefixed with file schema - relative base folder, relative path
		AssertThat(*File("../fileTestDirectory").resolve("file:../d/e.html"), is(File("..") / "d" / "e.html"));

		// Prefixed with file schema - absolute base folder, absolute path
		AssertThat(*target.resolve("file://" + source), is(File(source)));

		// Prefixed with file schema - relative base folder, absolute path
		AssertThat(*File("../fileTestDirectory").resolve("file://" + source), is(File(source)));

		// Prefixed with file schema - absolute base folder, empty path
		AssertThat(*target.resolve("file://"), is(target));

		// Prefixed with file schema - relative base folder, empty path
		AssertThat(*File("../fileTestDirectory").resolve("file://"), is(File("../fileTestDirectory")));

		/////////////////////////

		// Prefixed with file schema - absolute base file, relative path
		AssertThat(*File("/a/b/c.html").resolve("file:../d/e.html"), is(File("/a/d/e.html")));
		AssertThat(*File("/a/b/c.html").resolve("file:d/e.html"), is(File("/a/b/d/e.html")));

		// Prefixed with file schema - relative base file, relative path
		AssertThat(*File("a/b/c.html").resolve("file:../d/e.html"), is(File("a/d/e.html")));
		AssertThat(*File("a/b/c.html").resolve("file:d/e.html"), is(File("a/b/d/e.html")));

		// Prefixed with file schema - absolute base file, absolute path
		AssertThat(*File("/a/b/c.html").resolve("file:///d/e.html"), is(File("/d/e.html")));

		// Prefixed with file schema - relative base file, absolute path
		AssertThat(*File("a/b/c.html").resolve("file:///d/e.html"), is(File("/d/e.html")));

		// Prefixed with file schema - absolute base file, empty path
		AssertThat(*File("/a/b/c.html").resolve("file://"), is(File("/a/b")));

		// Prefixed with file schema - relative base file, empty path
		AssertThat(*File("a/b/c.html").resolve("file://"), is(File("a/b")));

		#ifdef BALAU_ENABLE_HTTP
			// Starts with non-file schema.
			AssertThat(*File("a/b/c.html").resolve("http://d/e/f"), is(Http("http://d/e/f")));
		#endif

		/////////////////////////

		// No scheme prefix - absolute base folder, relative path
		AssertThat(*target.resolve("../d/e.html"), is(File(target.getParentDirectory() / "d" / "e.html")));

		// No scheme prefix - relative base folder, relative path
		AssertThat(*File("../bin").resolve("../d/e.html"), is(File("..") / "d" / "e.html"));

		// No scheme prefix - absolute base folder, absolute path
		AssertThat(*target.resolve(source), is(File(source)));

		// No scheme prefix - relative base folder, absolute path
		AssertThat(*File("../bin").resolve(source), is(File(source)));

		// No scheme prefix - absolute base folder, empty path
		AssertThat(*target.resolve(""), is(File(target)));

		// No scheme prefix - relative base folder, empty path
		AssertThat(*File("../bin").resolve(""), is(File("../bin")));

		/////////////////////////

		// No scheme prefix - absolute base file, relative path
		AssertThat(*File("/a/b/c.html").resolve("../d/e.html"), is(File("/a/d/e.html")));
		AssertThat(*File("/a/b/c.html").resolve("d/e.html"), is(File("/a/b/d/e.html")));

		// No scheme prefix - relative base file, relative path
		AssertThat(*File("a/b/c.html").resolve("../d/e.html"), is(File("a/d/e.html")));
		AssertThat(*File("a/b/c.html").resolve("d/e.html"), is(File("a/b/d/e.html")));

		// No scheme prefix - absolute base file, absolute path
		AssertThat(*File("/a/b/c.html").resolve("///d/e.html"), is(File("/d/e.html")));

		// No scheme prefix - relative base file, absolute path
		AssertThat(*File("a/b/c.html").resolve("/d/e.html"), is(File("/d/e.html")));

		// No scheme prefix - absolute base file, empty path
		AssertThat(*File("/a/b/c.html").resolve(""), is(File("/a/b")));

		// No scheme prefix - relative base file, empty path
		AssertThat(*File("a/b/c.html").resolve(""), is(File("a/b")));
	}
};

} // namespace Resource

} // namespace Balau
