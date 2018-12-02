// //////////////////////////////////////////////////////////
// hash.h
// Copyright (c) 2014,2015 Stephan Brumme. All rights reserved.
// see http://create.stephan-brumme.com/disclaimer.html
//

#ifndef BALAU_TP__HASH_LIBRARY__HASH_H
#define BALAU_TP__HASH_LIBRARY__HASH_H

#include <string>

namespace Balau {

namespace HashLibrary {

/// abstract base class
class Hash {
	public:
	/// compute hash of a memory block
	virtual std::string operator ()(const void * data, size_t numBytes) = 0;

	/// compute hash of a string, excluding final zero
	virtual std::string operator ()(const std::string & text) = 0;

	/// add arbitrary number of bytes
	virtual void add(const void * data, size_t numBytes) = 0;

	/// return latest hash as hex characters
	virtual std::string getHash() = 0;

	/// restart
	virtual void reset() = 0;
};

} // namespace HashLibrary

} // namespace Balau

#endif // BALAU_TP__HASH_LIBRARY__HASH_H
