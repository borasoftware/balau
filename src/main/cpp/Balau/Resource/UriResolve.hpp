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
/// @file UriResolve.hpp
///
/// Resolve URIs from an existing URI.
///

#ifndef COM_BORA_SOFTWARE__BALAU_RESOURCE__URI_RESOLVE
#define COM_BORA_SOFTWARE__BALAU_RESOURCE__URI_RESOLVE

#include <Balau/Resource/UriFromString.hpp>

namespace Balau::Resource {

///
/// Resolve URIs from an existing URI.
///
class UriResolve {
	///
	/// Resolve the relative or absolute path, in reference to the current URI.
	///
	/// If the supplied path contains a scheme prefix, the URI fromString function will be
	/// called to create a new unrelated URI instance.
	///
	/// This method will determine the base path to use by applying the following rules.
	///
	///  - If the URI path ends with a forward slash (or is a directory for file URIs), the
	///    base path is the URI path.
	///
	///  - Otherwise, the rightmost path element is stripped from the URI path to form
	///    the base path.
	///
	/// @param path a relative or absolute path, or a full URI
	/// @return a new URI
	///
	public: static std::unique_ptr<Uri> resolve(const Uri & uri, std::string_view path);

	public: static std::unique_ptr<Uri> resolve(const File & file, std::string_view path);
	public: static std::unique_ptr<Uri> resolve(const Http & http, std::string_view path);
	public: static std::unique_ptr<Uri> resolve(const Https & https, std::string_view path);
	public: static std::unique_ptr<Uri> resolve(const StringUri & stringUri, std::string_view path);
	public: static std::unique_ptr<Uri> resolve(const ZipEntry & zipEntry, std::string_view path);
	public: static std::unique_ptr<Uri> resolve(const ZipFile & zipFile, std::string_view path);

	private: struct UriResolveVisitor : public UriVisitor {
		std::string_view path;
		std::unique_ptr<Uri> resultUri;

		UriResolveVisitor(std::string_view path_) : path(path_) {}

		public: void visit(const File & uri) override {
			resultUri = UriResolve::resolve(uri, path);
		}

		public: void visit(const Http & uri) override {
			resultUri = UriResolve::resolve(uri, path);
		}

		public: void visit(const Https & uri) override {
			resultUri = UriResolve::resolve(uri, path);
		}

		public: void visit(const StringUri & uri) override {
			resultUri = UriResolve::resolve(uri, path);
		}

		public: void visit(const ZipEntry & uri) override {
			resultUri = UriResolve::resolve(uri, path);
		}

		public: void visit(const ZipFile & uri) override {
			resultUri = UriResolve::resolve(uri, path);
		}

		public: void visit(const Uri & uri) override {
			resultUri = UriResolve::resolve(uri, path);
		}
	};

	friend struct UriResolveVisitor;
};

inline std::unique_ptr<Uri> UriResolve::resolve(const Uri & uri, std::string_view path) {
	UriResolveVisitor visitor(path);
	uri.visit(visitor);
	return std::move(visitor.resultUri);
}

inline std::unique_ptr<Uri> UriResolve::resolve(const File & file, std::string_view path) {
	static const std::regex scheme { "[a-zA-Z][a-zA-Z0-9+-\\.]*:" };

	auto cleanPath = Util::Strings::trim(path);
	auto str = std::string(cleanPath);
	std::string sPath;
	bool hasFileScheme = false;

	if (Util::Strings::startsWith(str, "file:")) {
		// Prefixed with file schema.
		// Relative or absolute?
		sPath = str.substr(5);
		hasFileScheme = true;
	} else if (Util::Strings::startsWithRegex(str, scheme)) {
		std::unique_ptr<Uri> uri;
		fromString(uri, str);
		return uri;
	} else {
		// No scheme prefix.
		// Absolute or relative file path.
		sPath = str;
	}

	if ((!hasFileScheme && sPath.empty()) || (hasFileScheme && sPath == "//")) {
		// Empty.. return the path if it is a folder,
		// otherwise return the parent path of the file.

		if (file.isRegularDirectory()) {
			return std::unique_ptr<Uri>(new File(file));
		} else {
			return std::unique_ptr<Uri>(new File(file.getParentDirectory()));
		}
	} else if (sPath[0] == '/') {
		// Absolute.

		if (hasFileScheme) {
			// Invalid?
			if (sPath.length() < 3 || sPath.substr(0, 3) != "///") {
				using ::toString;

				ThrowBalauException(
					Exception::IllegalArgumentException, toString("Illegal path string in file URI: ", path)
				);
			}

			// Strip the leading double slash "//".
			sPath = sPath.substr(2);
		}

		return std::unique_ptr<Uri>(new File(sPath));
	} else {
		// Relative.. resolve according to the current path
		// (current file if folder or parent otherwise).

		auto bPath = boost::filesystem::path(sPath);

		if (file.isRegularDirectory()) {
			auto newPath = (file.getEntry() / bPath).lexically_normal();
			return std::unique_ptr<Uri>(new File(newPath));
		} else {
			auto newPath = (file.getParentDirectory().getEntry() / bPath).lexically_normal();
			return std::unique_ptr<Uri>(new File(newPath));
		}
	}
}

inline std::unique_ptr<Uri> UriResolve::resolve(const Http & http, std::string_view path) {
	static const std::regex scheme { "[a-zA-Z][a-zA-Z0-9+-\\.]*:" };

	auto cleanPath = Util::Strings::trim(path);
	auto str = std::string(cleanPath);

	if (Util::Strings::startsWithRegex(str, scheme)) {
		std::unique_ptr<Uri> uri;
		fromString(uri, str);
		return uri;
	}

	const UriComponents components(http);

	if (Util::Strings::startsWith(str, "/")) {
		// Absolute path.
		std::string url = ::toString(components.scheme(), "://", components.host(), str);
		return std::unique_ptr<Uri>(new Http(url));
	} else {
		// Relative path.
		// TODO normalise path.
		std::string url = ::toString(components.scheme(), "://", components.host(), "/", components.path(), "/", str);
		return std::unique_ptr<Uri>(new Http(url));
	}
}

inline std::unique_ptr<Uri> UriResolve::resolve(const Https & https, std::string_view path) {
	static const std::regex scheme { "[a-zA-Z][a-zA-Z0-9+-\\.]*:" };

	auto cleanPath = Util::Strings::trim(path);
	auto str = std::string(cleanPath);

	if (Util::Strings::startsWithRegex(str, scheme)) {
		std::unique_ptr<Uri> uri;
		fromString(uri, str);
		return uri;
	}

	const UriComponents components(https);

	if (Util::Strings::startsWith(str, "/")) {
		// Absolute path.
		std::string url = ::toString(components.scheme(), "://", components.host(), str);
		return std::unique_ptr<Uri>(new Https(url));
	} else {
		// Relative path.
		// TODO normalise path.
		std::string url = ::toString(components.scheme(), "://", components.host(), "/", components.path(), "/", str);
		return std::unique_ptr<Uri>(new Https(url));
	}
}

inline std::unique_ptr<Uri> UriResolve::resolve(const StringUri & stringUri, std::string_view path) {
	static const std::regex scheme { "[a-zA-Z][a-zA-Z0-9+-\\.]*:" };

	auto cleanPath = Util::Strings::trim(path);
	auto str = std::string(cleanPath);

	if (Util::Strings::startsWithRegex(str, scheme)) {
		std::unique_ptr<Uri> uri;
		fromString(uri, str);
		return uri;
	}

	ThrowBalauException(
		  Exception::IllegalArgumentException
		, "StringUri does not support calls to the resolve method without specifying a schema in the path."
	);

	// Prevent compiler warning.
	return std::unique_ptr<Uri>();
}

inline std::unique_ptr<Uri> UriResolve::resolve(const ZipEntry & zipEntry, std::string_view path) {
	static const std::regex scheme { "[a-zA-Z][a-zA-Z0-9+-\\.]*:" };

	auto cleanPath = Util::Strings::trim(path);
	auto str = std::string(cleanPath);

	if (Util::Strings::startsWithRegex(str, scheme)) {
		std::unique_ptr<Uri> uri;
		fromString(uri, str);
		return uri;
	}

	if (Util::Strings::startsWith(str, "/")) {
		// Absolute path.
		return std::unique_ptr<Uri>(new ZipEntry(zipEntry.archive, zipEntry.archive.getEntryIndex(str)));
	} else {
		// Relative path.
		const std::string entryName = zipEntry.archive.getEntryName(zipEntry.entryIndex);
		const std::string normalisedName = UriComponents::normalizePath(entryName);
		const auto newName = normalisedName + str;
		return std::unique_ptr<Uri>(new ZipEntry(zipEntry.archive, zipEntry.archive.getEntryIndex(newName)));
	}
}

inline std::unique_ptr<Uri> UriResolve::resolve(const ZipFile & zipFile, std::string_view path) {
	static const std::regex scheme { "[a-zA-Z][a-zA-Z0-9+-\\.]*:" };

	auto cleanPath = Util::Strings::trim(path);
	auto str = std::string(cleanPath);

	if (Util::Strings::startsWithRegex(str, scheme)) {
		std::unique_ptr<Uri> uri;
		fromString(uri, str);
		return uri;
	}

	boost::filesystem::path p(str);

	if (p.is_relative()) {
		auto p2 = boost::filesystem::relative(p, zipFile.entry.path());
		return std::unique_ptr<Uri>(new ZipFile(File(p2)));
	} else { // absolute
		return std::unique_ptr<Uri>(new ZipFile(File(p)));
	}
}

} // namespace Balau::Resource

#endif // COM_BORA_SOFTWARE__BALAU_RESOURCE__URI_RESOLVE
