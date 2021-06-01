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

#ifndef COM_BORA_SOFTWARE__BALAU_APPLICATION__INJECTOR_HEADER_BODY
#define COM_BORA_SOFTWARE__BALAU_APPLICATION__INJECTOR_HEADER_BODY

#include <Balau/Application/InjectableHeader.hpp>

//
//    Use case                                  interface    implementation
//
//  - inject no dependencies                    HBBase1      HBDerived
//  - inject with dependency                    HBBase2      HBDerivedD
//  - inject named no dependencies              HBBase1      HBDerivedN
//  - inject named with dependency              HBBase2      HBDerivedND
//
//  - inject construct no dependencies          HBBaseC1     HBDerivedC
//  - inject construct with dependency          HBBaseC2     HBDerivedCD
//  - inject construct named no dependencies    HBBaseC1     HBDerivedCN
//  - inject construct named with dependency    HBBaseC2     HBDerivedCND
//
//  - inject types no dependencies              HBBaseT1     HBDerivedT
//  - inject types with dependency              HBBaseT2     HBDerivedTD
//  - inject types named no dependencies        HBBaseT1     HBDerivedTN
//  - inject types named with dependency        HBBaseT2     HBDerivedTND
//

namespace Balau {

struct HBBase1  {
	virtual ~HBBase1()  = default;
};

struct HBBase2  {
	virtual ~HBBase2()  = default;
};

struct HBBaseC1 {
	virtual ~HBBaseC1() = default;
};

struct HBBaseC2 {
	virtual ~HBBaseC2() = default;
};

struct HBBaseT1 {
	virtual ~HBBaseT1() = default;
};

struct HBBaseT2 {
	virtual ~HBBaseT2() = default;
};

struct HBDerived : public HBBase1 {
	BalauInjectHeader(HBDerived);
};

struct HBDerivedD : public HBBase2 {
	std::shared_ptr<HBBase1> b;
	explicit HBDerivedD(std::shared_ptr<HBBase1> b_);
	BalauInjectHeader(HBDerivedD);
};

struct HBDerivedN : public HBBase1 {
	BalauInjectHeader(HBDerivedN);
};

struct HBDerivedND : public HBBase2 {
	std::shared_ptr<HBBase1> b;
	explicit HBDerivedND(std::shared_ptr<HBBase1> b_);
	BalauInjectHeader(HBDerivedND);
};

struct HBDerivedC : public HBBaseC1 {
	BalauInjectHeaderConstruct(HBDerivedC);
};

struct HBDerivedCD : public HBBaseC2 {
	std::shared_ptr<HBBase1> b;
	BalauInjectHeaderConstruct(HBDerivedCD, b);
};

struct HBDerivedCN : public HBBaseC1 {
	BalauInjectHeaderConstruct(HBDerivedCN);
};

struct HBDerivedCND : public HBBaseC2 {
	std::shared_ptr<HBBase1> b;
	BalauInjectHeaderConstruct(HBDerivedCND, b);
};

struct HBDerivedT : public HBBaseT1 {
	BalauInjectHeader(HBDerivedT);
};

struct HBDerivedTD : public HBBaseT2 {
	std::shared_ptr<HBBase1> b;
	explicit HBDerivedTD(std::shared_ptr<HBBase1> b_);
	BalauInjectHeader(HBDerivedTD);
};

struct HBDerivedTN : public HBBaseT1 {
	BalauInjectHeader(HBDerivedTN);
};

struct HBDerivedTND : public HBBaseT2 {
	std::shared_ptr<HBBase1> b;
	explicit HBDerivedTND(std::shared_ptr<HBBase1> b_);
	BalauInjectHeader(HBDerivedTND);
};

} // namespace Balau

#endif // COM_BORA_SOFTWARE__BALAU_APPLICATION__INJECTOR_HEADER_BODY
