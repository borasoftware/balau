
// @formatter:off
//
// Balau core C++ library
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
#include "FileUtf8To32ReadResource.hpp"
#include "File.hpp"
#include "../Exception/ResourceExceptions.hpp"

namespace Balau::Resource {

FileUtf8To32ReadResource::FileUtf8To32ReadResource(const File & file_)
	: file(std::make_unique<File>(file_))
	, stream(new std::u32ifstream(file_.getEntry().path().native())) {
	// Overload 7 is typically called with its second argument, f, obtained
	// directly from a new-expression: the locale is responsible for calling
	// the matching delete from its own destructor.
	stream->imbue(std::locale(std::locale(), new std::codecvt<char32_t, char, std::mbstate_t>));

	if (!stream->is_open()) {
		ThrowBalauException(Exception::NotFoundException, toString(file_));
	}
}

const Uri & FileUtf8To32ReadResource::uri() const {
	return *file;
}

const File & FileUtf8To32ReadResource::getFile() const {
	return *file;
}

} // namespace Balau::Resource
