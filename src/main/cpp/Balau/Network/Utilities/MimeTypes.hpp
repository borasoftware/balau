// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2017 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#ifndef COM_BORA_SOFTWARE__BALAU_NETWORK_UTILITIES__MIME_TYPES
#define COM_BORA_SOFTWARE__BALAU_NETWORK_UTILITIES__MIME_TYPES

///
/// @file MimeTypes.hpp
///
/// Mime type utilities.
///

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
