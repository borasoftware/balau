// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2017 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include "SharedMemoryQueueImpl.hpp"

namespace Balau::Interprocess::Impl {

thread_local SharedMemoryQueueTLS SharedMemoryQueueTLS::storage;

} // namespace Balau::Interprocess::Impl
