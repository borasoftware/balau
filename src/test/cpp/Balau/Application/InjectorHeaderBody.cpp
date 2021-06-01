// @formatter:off
//
// Balau core C++ library
//
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
