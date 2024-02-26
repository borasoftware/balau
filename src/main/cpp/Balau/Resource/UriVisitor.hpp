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
/// @file UriVisitor.hpp
///
/// Resolve Visit URIs.
///

#ifndef COM_BORA_SOFTWARE_BALAU_RESOURCE_URI_VISITOR
#define COM_BORA_SOFTWARE_BALAU_RESOURCE_URI_VISITOR

#include <Balau/Exception/BalauException.hpp>

namespace Balau::Resource {

class File;
class Http;
class Https;
class StringUri;
class Uri;
class ZipEntry;
class ZipFile;

///
/// Visit URIs.
///
class UriVisitor {
	public: virtual void visit(const File & uri) = 0;
	public: virtual void visit(const Http & uri) = 0;
	public: virtual void visit(const Https & uri) = 0;
	public: virtual void visit(const StringUri & uri) = 0;
	public: virtual void visit(const ZipEntry & uri) = 0;
	public: virtual void visit(const ZipFile & uri) = 0;

	public: virtual void visit(const Uri & uri) {
		ThrowBalauException(Exception::NotImplementedException, "UriVisitor::visit");
	}
};

} // namespace Balau::Resource

#endif // COM_BORA_SOFTWARE_BALAU_RESOURCE_URI_VISITOR
