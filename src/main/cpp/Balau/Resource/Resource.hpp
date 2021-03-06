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
/// @file Resource.hpp
///
/// Abstract base class of resource classes.
///

#ifndef COM_BORA_SOFTWARE__BALAU_RESOURCE__RESOURCE
#define COM_BORA_SOFTWARE__BALAU_RESOURCE__RESOURCE

namespace Balau::Resource {

class Uri;

///
/// Abstract base class of resource classes.
///
/// Instances of concrete classes are typically used within a
/// std::unique_ptr<Resource> or std::shared_ptr<Resource>.
///
class Resource {
	///
	/// Get the URI associated with this resource.
	///
	public: virtual const Uri & uri() const = 0;

	///
	/// Close the stream(s) in the resource.
	///
	public: virtual void close() = 0;

	///
	/// Destroy the resource instance.
	///
	public: virtual ~Resource() = default;
};

} // namespace Balau::Resource

#endif // COM_BORA_SOFTWARE__BALAU_RESOURCE__RESOURCE
