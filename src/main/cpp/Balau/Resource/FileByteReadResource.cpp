// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include "FileByteReadResource.hpp"
#include "File.hpp"
#include "../Exception/ResourceExceptions.hpp"

namespace Balau::Resource {

FileByteReadResource::FileByteReadResource(const File & file_)
	: file(std::make_unique<File>(file_))
	, stream(new boost::filesystem::ifstream(file_.getEntry())) {

	if (!stream->is_open()) {
		ThrowBalauException(Exception::NotFoundException, toString(file_));
	}
}

const Uri & FileByteReadResource::uri() const {
	return *file;
}

const File & FileByteReadResource::getFile() const {
	return *file;
}

} // namespace Balau::Resource
