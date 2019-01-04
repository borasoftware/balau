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

#if BALAU_LOGGING_THREAD_LOCAL_ALLOCATOR == 1

thread_local LoggerAllocatorState loggerAllocatorState;

#endif

} // namespace Balau::LoggingSystem
