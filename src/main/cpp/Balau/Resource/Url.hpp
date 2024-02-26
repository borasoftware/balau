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
/// @file Url.hpp
///
/// An abstract %Url (either http or https).
///

#ifndef COM_BORA_SOFTWARE_BALAU_RESOURCE_URL
#define COM_BORA_SOFTWARE_BALAU_RESOURCE_URL

#include <Balau/Resource/Uri.hpp>
#include <Balau/Resource/UriComponents.hpp>

namespace Balau::Resource {

///
/// An abstract %Url (either http or https).
///
class Url : public Uri {
	public: bool isRegularDirectory() const override {
		return false;
	}

	public: bool isRegularFile() const override {
		return false;
	}

	public: std::string toUriString() const override {
		return uri;
	}

	public: std::string toRawString() const override {
		return uri;
	}

	public: size_t hashcode() const noexcept override {
		return std::hash<std::string>()(uri);
	}

	////////////////////////// Private implementation /////////////////////////

	protected: Url() = default;

	protected: explicit Url(std::string && uri_) : uri(std::move(uri_)) {}

	protected: explicit Url(std::string_view uri_) : uri(std::string(uri_)) {}

	protected: explicit Url(const char * uri_) : uri(std::string(uri_)) {}

	protected: Url(const Url & copy) : uri(copy.uri) {}

	protected: Url(Url && rhs) noexcept : uri(std::move(rhs.uri)) {}

	protected: std::string appendPathComponent(const std::string & pathComponent) const {
		const UriComponents components(uri);
		// TODO normalise path.

		std::string url = ::toString(
			components.scheme(), "://", components.host(), components.path(), "/", pathComponent
		);

		if (components.hasQuery()) {
			url += "?";
			url += components.query();
		}

		if (components.hasFragment()) {
			url += "#";
			url += components.fragment();
		}

		return url;
	}


	protected: std::string uri;
};

} // namespace Balau::Resource

#endif // COM_BORA_SOFTWARE_BALAU_RESOURCE_URL
