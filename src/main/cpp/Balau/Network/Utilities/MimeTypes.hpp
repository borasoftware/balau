// @formatter:off
//
// Balau core C++ library
// Copyright (C) 2017 Bora Software (contact@borasoftware.com)
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

#ifndef COM_BORA_SOFTWARE__BALAU_NETWORK_UTILITIES__MIME_TYPES
#define COM_BORA_SOFTWARE__BALAU_NETWORK_UTILITIES__MIME_TYPES

///
/// @file MimeTypes.hpp
///
/// Mime type utilities.
///

#include <Balau/Type/StdTypes.hpp>

#include <memory>
#include <string>
#include <unordered_map>

namespace Balau::Network {

///
/// Holds a map of mime types, keyed to the corresponding file extension.
///
/// Instances of this class are internally const.
///
/// If a custom set of mime types is required, the data of the default mime types
/// instance can be copied in order to create a custom mime types instance.
///
/// Once constructed, the mime types instance should be shared via a shared_ptr
/// container.
///
class MimeTypes {
	///
	/// The default set of mime types available.
	///
	public: static std::shared_ptr<MimeTypes> defaultMimeTypes;

	///
	/// Construct a custom mime types instance by moving the supplied data.
	///
	public: MimeTypes(std::unordered_map<std::string, std::string> && data_) noexcept
		: data(std::move(data_)) {}

	///
	/// Lookup a mime type from the supplied path.
	///
	/// If no registered file extension is found, an empty string view is returned.
	///
	public: std::string_view lookup(const std::string & path) const;

	///
	/// Get the internal data in order to construct a custom mime types instance.
	///
	public: const std::unordered_map<std::string, std::string> & getData() const {
		return data;
	}

	///////////////////////// Private implementation //////////////////////////

	private: const std::unordered_map<std::string, std::string> data;

	public: MimeTypes() = delete;
	public: MimeTypes(MimeTypes &) = delete;
	public: MimeTypes & operator = (MimeTypes &) = delete;
};

} // namespace Balau::Network

#endif // COM_BORA_SOFTWARE__BALAU_NETWORK_UTILITIES__MIME_TYPES
