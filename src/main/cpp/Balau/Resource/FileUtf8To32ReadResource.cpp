
// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include "FileUtf8To32ReadResource.hpp"
#include "File.hpp"
#include "../Exception/ResourceExceptions.hpp"

namespace Balau::Resource {

FileUtf8To32ReadResource::FileUtf8To32ReadResource(const File & file_)
	: file(std::make_unique<File>(file_))
	, stream(file_.getEntry().path().native()) {
	// Overload 7 is typically called with its second argument, f, obtained
	// directly from a new-expression: the locale is responsible for calling
	// the matching delete from its own destructor.
	stream.imbue(std::locale(std::locale(), new std::codecvt<char32_t, char, std::mbstate_t>));

	if (!stream.is_open()) {
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
