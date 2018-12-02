// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
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
