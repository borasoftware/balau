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

///
/// @file SerializationMacros.hpp
///
/// Macros used in serialisation methods.
///

#ifndef COM_BORA_SOFTWARE__BALAU_SERIALIZATION__SERIALIZATION_MACROS
#define COM_BORA_SOFTWARE__BALAU_SERIALIZATION__SERIALIZATION_MACROS

#include <boost/serialization/nvp.hpp>

///
/// Prettier macro for the Boost serialisation NVP macro.
///
#define BoostSerialization(NAME) BOOST_SERIALIZATION_NVP(NAME)

///
/// Prettier macro for the Boost serialisation base class object NVP macro.
///
#define BoostSerializationBase(NAME) BOOST_SERIALIZATION_BASE_OBJECT_NVP(NAME)

///
/// Prettier macro for the Boost serialisation class version macro.
///
#define BoostSerializationClassVersion(TYPE, VERSION) BOOST_CLASS_VERSION(TYPE, VERSION)


#endif // COM_BORA_SOFTWARE__BALAU_SERIALIZATION__SERIALIZATION_MACROS
