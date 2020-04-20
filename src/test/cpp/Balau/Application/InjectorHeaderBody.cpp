// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include "InjectorHeaderBody.hpp"

#include "../../../../main/cpp/Balau/Application/InjectableBody.hpp"

namespace Balau {

BalauInjectBody(HBDerived)

BalauInjectBody(HBDerivedD, b)

HBDerivedD::HBDerivedD(std::shared_ptr<HBBase1> b_) : b(std::move(b_)) {}

BalauInjectBodyNamed(HBDerivedN)

BalauInjectBodyNamed(HBDerivedND, b, "ND")

HBDerivedND::HBDerivedND(std::shared_ptr<HBBase1> b_) : b(std::move(b_)) {}

BalauInjectBody(HBDerivedC)

BalauInjectBody(HBDerivedCD, b)

BalauInjectBodyNamed(HBDerivedCN)

BalauInjectBodyNamed(HBDerivedCND, b, "ND")

BalauInjectBodyTypes(HBDerivedT)

BalauInjectBodyTypes(HBDerivedTD, std::shared_ptr<HBBase1>)

HBDerivedTD::HBDerivedTD(std::shared_ptr<HBBase1> b_) : b(std::move(b_)) {}

BalauInjectBodyNamedTypes(HBDerivedTN)

BalauInjectBodyNamedTypes(HBDerivedTND, std::shared_ptr<HBBase1>, "ND")

HBDerivedTND::HBDerivedTND(std::shared_ptr<HBBase1> b_) : b(std::move(b_)) {}

} // namespace Balau
