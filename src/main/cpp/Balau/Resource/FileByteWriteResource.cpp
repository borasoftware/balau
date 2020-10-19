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
#include "FileByteWriteResource.hpp"
#include "File.hpp"
#include "../Exception/ResourceExceptions.hpp"

namespace Balau::Resource {

FileByteWriteResource::FileByteWriteResource(const File & file_)
	: file(std::make_unique<File>(file_))
	, stream(new std::ofstream(file_.getEntry().path())) {

	if (!stream->is_open()) {
		ThrowBalauException(Exception::NotFoundException, toString(file_));
	}
}

const Uri & FileByteWriteResource::uri() const {
	return *file;
}

const File & FileByteWriteResource::getFile() const {
	return *file;
}

} // namespace Balau::Resource
