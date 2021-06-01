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

#include <TestResources.hpp>

#include "../../../../main/cpp/Balau/Resource/UriResolve.hpp"

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
		RegisterTestCase(recursiveIteration);
		RegisterTestCase(nonRecursiveIteration);
		RegisterTestCase(resolve);
		RegisterTestCase(fileAppend);
		RegisterTestCase(uriAppend);
	}

	void recursiveIteration() {
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

	void nonRecursiveIteration() {
		File root(TestResources::SourceFolder / "doc" / "manual");
		auto iterator = root.fileIterator();

		size_t regularFileCount = 0;
		size_t directoryFileCount = 0;

		while (iterator.hasNext()) {
			auto file = iterator.next();

			if (file.isRegularFile()) {
				++regularFileCount;
			} else if (file.isRegularDirectory()) {
				++directoryFileCount;
			}
		}

		AssertThat(regularFileCount, isGreaterThan(0U));
		AssertThat(directoryFileCount, isGreaterThan(10U));
		AssertThat(regularFileCount, isLessThan(10U));
		AssertThat(directoryFileCount, isLessThan(30U));
	}

	void resolve() {
		auto source = TestResources::SourceFolder.toAbsolutePath().toRawString();
		auto target = TestResources::TargetFolder;

		auto fileTestDirectory = TestResources::TestResultsFolder.getParentDirectory() / "fileTestDirectory";
		fileTestDirectory.createDirectories();

		// Prefixed with file schema - absolute base folder, relative path
		AssertThat(*UriResolve::resolve(target, "file:../d/e.html"), is(File(target.getParentDirectory() / "d" / "e.html")));

		// Prefixed with file schema - relative base folder, relative path
		AssertThat(*UriResolve::resolve(File("../fileTestDirectory"), "file:../d/e.html"), is(File("..") / "d" / "e.html"));

		// Prefixed with file schema - absolute base folder, absolute path
		AssertThat(*UriResolve::resolve(target, "file://" + source), is(File(source)));

		// Prefixed with file schema - relative base folder, absolute path
		AssertThat(*UriResolve::resolve(File("../fileTestDirectory"), "file://" + source), is(File(source)));

		// Prefixed with file schema - absolute base folder, empty path
		AssertThat(*UriResolve::resolve(target, "file://"), is(target));

		// Prefixed with file schema - relative base folder, empty path
		AssertThat(*UriResolve::resolve(File("../fileTestDirectory"), "file://"), is(File("../fileTestDirectory")));

		/////////////////////////

		// Prefixed with file schema - absolute base file, relative path
		AssertThat(*UriResolve::resolve(File("/a/b/c.html"), "file:../d/e.html"), is(File("/a/d/e.html")));
		AssertThat(*UriResolve::resolve(File("/a/b/c.html"), "file:d/e.html"), is(File("/a/b/d/e.html")));

		// Prefixed with file schema - relative base file, relative path
		AssertThat(*UriResolve::resolve(File("a/b/c.html"), "file:../d/e.html"), is(File("a/d/e.html")));
		AssertThat(*UriResolve::resolve(File("a/b/c.html"), "file:d/e.html"), is(File("a/b/d/e.html")));

		// Prefixed with file schema - absolute base file, absolute path
		AssertThat(*UriResolve::resolve(File("/a/b/c.html"), "file:///d/e.html"), is(File("/d/e.html")));

		// Prefixed with file schema - relative base file, absolute path
		AssertThat(*UriResolve::resolve(File("a/b/c.html"), "file:///d/e.html"), is(File("/d/e.html")));

		// Prefixed with file schema - absolute base file, empty path
		AssertThat(*UriResolve::resolve(File("/a/b/c.html"), "file://"), is(File("/a/b")));

		// Prefixed with file schema - relative base file, empty path
		AssertThat(*UriResolve::resolve(File("a/b/c.html"), "file://"), is(File("a/b")));

		#ifdef BALAU_ENABLE_HTTP
			// Starts with non-file schema.
			AssertThat(*UriResolve::resolve(File("a/b/c.html"), "http://d/e/f"), is(Http("http://d/e/f")));
		#endif

		/////////////////////////

		// No scheme prefix - absolute base folder, relative path
		AssertThat(*UriResolve::resolve(target, "../d/e.html"), is(File(target.getParentDirectory() / "d" / "e.html")));

		// No scheme prefix - relative base folder, relative path
		AssertThat(*UriResolve::resolve(File("../bin"), "../d/e.html"), is(File("..") / "d" / "e.html"));

		// No scheme prefix - absolute base folder, absolute path
		AssertThat(*UriResolve::resolve(target, source), is(File(source)));

		// No scheme prefix - relative base folder, absolute path
		AssertThat(*UriResolve::resolve(File("../bin"), source), is(File(source)));

		// No scheme prefix - absolute base folder, empty path
		AssertThat(*UriResolve::resolve(target, ""), is(File(target)));

		// No scheme prefix - relative base folder, empty path
		AssertThat(*UriResolve::resolve(File("../bin"), ""), is(File("../bin")));

		/////////////////////////

		// No scheme prefix - absolute base file, relative path
		AssertThat(*UriResolve::resolve(File("/a/b/c.html"), "../d/e.html"), is(File("/a/d/e.html")));
		AssertThat(*UriResolve::resolve(File("/a/b/c.html"), "d/e.html"), is(File("/a/b/d/e.html")));

		// No scheme prefix - relative base file, relative path
		AssertThat(*UriResolve::resolve(File("a/b/c.html"), "../d/e.html"), is(File("a/d/e.html")));
		AssertThat(*UriResolve::resolve(File("a/b/c.html"), "d/e.html"), is(File("a/b/d/e.html")));

		// No scheme prefix - absolute base file, absolute path
		AssertThat(*UriResolve::resolve(File("/a/b/c.html"), "///d/e.html"), is(File("/d/e.html")));

		// No scheme prefix - relative base file, absolute path
		AssertThat(*UriResolve::resolve(File("a/b/c.html"), "/d/e.html"), is(File("/d/e.html")));

		// No scheme prefix - absolute base file, empty path
		AssertThat(*UriResolve::resolve(File("/a/b/c.html"), ""), is(File("/a/b")));

		// No scheme prefix - relative base file, empty path
		AssertThat(*UriResolve::resolve(File("a/b/c.html"), ""), is(File("a/b")));
	}

	void fileAppend() {
		File file("/a/b/c");
		auto actual = file / "d" / "e" / "f";
		auto expected = File("/a/b/c/d/e/f");

		AssertThat(actual, is(expected));
	}

	void uriAppend() {
		auto uri = std::unique_ptr<Uri>(new File("/a/b/c"));
		auto actual = uri / "d" / "e" / "f";
		auto expected = File("/a/b/c/d/e/f");

		AssertThat(*actual, is(expected));
	}
};

} // namespace Resource

} // namespace Balau
