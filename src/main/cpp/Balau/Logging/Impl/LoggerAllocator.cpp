// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include "LoggerAllocator.hpp"

namespace Balau::LoggingSystem {

#ifdef BALAU_ENABLE_THREAD_LOCAL_LOGGING_ALLOCATOR

thread_local LoggerAllocatorState loggerAllocatorState;

#endif // BALAU_ENABLE_THREAD_LOCAL_LOGGING_ALLOCATOR

} // namespace Balau::LoggingSystem
