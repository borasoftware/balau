// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
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

//#define _BBalauExpand(ARG)       ARG
//#define _BBalauStringExpand(ARG) #ARG

//#define _BBalauInjectHeaderConstruct__2(TYPE, F0)
//	_BalauInjectHeader(TYPE);
//
//	public: explicit TYPE(_BalauHIPF(F0)) : _BalauHIMF(F0) {}

//#define _BBalauVariadicNArg_(...) _BBalauExpand(_BBalauVariadicNArgN(__VA_ARGS__))
//#define _BBalauVariadicNArgN(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, N, ...) N
//#define _BBalauVariadicRSeqN() 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0
//#define BBalauVariadicNArg(...)  _BBalauExpand(_BBalauVariadicNArg_(__VA_ARGS__, _BBalauVariadicRSeqN()))
//#define _BBalauInjectHeaderConstruct__(N)          _BBalauInjectHeaderConstruct__##N
//#define _BBalauInjectHeaderConstruct__Eval(N)      _BBalauInjectHeaderConstruct__(N)
//#define _BBalauInjectHeaderConstruct(...)        BalauVaArgsExpand(BalauVaArgsExpand(_BalauInjectHeaderConstruct__Eval(BalauVaArgsExpand(BalauVaArgsExpand(BalauVariadicNArg)(__VA_ARGS__))))(__VA_ARGS__))
//#define MACRO_VA_ARGS(...) BalauVaArgsExpand(BalauVaArgsExpand(MACRO_WITH_3_PARAMS)( __VA_ARGS__))
//#define __BBalauInjectHeaderConstruct(A, B)        _BBalauInjectHeaderConstruct__Eval(BBalauVariadicNArg(A, B))(A, B)
//#define STRING2(x) #x
//#define STRING(x) STRING2(x)
////#pragma message("content: " STRING(_BBalauInjectHeaderConstruct(HBDerivedCD, b)))

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
