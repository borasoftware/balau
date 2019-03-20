// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#ifndef COM_BORA_SOFTWARE__BALAU_APPLICATION__TO_STRING_TEST
#define COM_BORA_SOFTWARE__BALAU_APPLICATION__TO_STRING_TEST

#include <Balau/Testing/TestRunner.hpp>

struct ToStringTest : public Balau::Testing::TestGroup<ToStringTest> {
	ToStringTest() {
		registerTest(&ToStringTest::toString_string,                      "toString_string");
		registerTest(&ToStringTest::toString_u16string,                   "toString_u16string");
		registerTest(&ToStringTest::toString_u32string,                   "toString_u32string");
		registerTest(&ToStringTest::toString_char,                        "toString_char");
		registerTest(&ToStringTest::toString_char16_t,                    "toString_char16_t");
		registerTest(&ToStringTest::toString_char32_t,                    "toString_char32_t");
		registerTest(&ToStringTest::toString_signed_char,                 "toString_signed_char");
		registerTest(&ToStringTest::toString_short,                       "toString_short");
		registerTest(&ToStringTest::toString_int,                         "toString_int");
		registerTest(&ToStringTest::toString_long,                        "toString_long");
		registerTest(&ToStringTest::toString_long_long,                   "toString_long_long");
		registerTest(&ToStringTest::toString_unsigned_char,               "toString_unsigned_char");
		registerTest(&ToStringTest::toString_unsigned_short,              "toString_unsigned_short");
		registerTest(&ToStringTest::toString_unsigned_int,                "toString_unsigned_int");
		registerTest(&ToStringTest::toString_unsigned_long,               "toString_unsigned_long");
		registerTest(&ToStringTest::toString_unsigned_long_long,          "toString_unsigned_long_long");
		registerTest(&ToStringTest::toString_float,                       "toString_float");
		registerTest(&ToStringTest::toString_double,                      "toString_double");
		registerTest(&ToStringTest::toString_long_double,                 "toString_long_double");
		registerTest(&ToStringTest::toString_bool,                        "toString_bool");
		registerTest(&ToStringTest::toString_const_char_ptr,              "toString_const_char_ptr");
		registerTest(&ToStringTest::toString_const_char16_t_ptr,          "toString_const_char16_t_ptr");
		registerTest(&ToStringTest::toString_const_char32_t_ptr,          "toString_const_char32_t_ptr");
		registerTest(&ToStringTest::toString_const_void_ptr,              "toString_const_void_ptr");
		registerTest(&ToStringTest::toString_type_index,                  "toString_type_index");
		registerTest(&ToStringTest::toString_function,                    "toString_function");
		registerTest(&ToStringTest::toString_vector,                      "toString_vector");
		registerTest(&ToStringTest::toString_macro,                       "toString_macro");
		registerTest(&ToStringTest::toString_parameter_pack,              "toString_parameter_pack");
		registerTest(&ToStringTest::toString16_string,                    "toString16_string");
		registerTest(&ToStringTest::toString16_u16string,                 "toString16_u16string");
		registerTest(&ToStringTest::toString16_u32string,                 "toString16_u32string");
		registerTest(&ToStringTest::toString16_char,                      "toString16_char");
		registerTest(&ToStringTest::toString16_char16_t,                  "toString16_char16_t");
		registerTest(&ToStringTest::toString16_char32_t,                  "toString16_char32_t");
		registerTest(&ToStringTest::toString16_signed_char,               "toString16_signed_char");
		registerTest(&ToStringTest::toString16_short,                     "toString16_short");
		registerTest(&ToStringTest::toString16_int,                       "toString16_int");
		registerTest(&ToStringTest::toString16_long,                      "toString16_long");
		registerTest(&ToStringTest::toString16_long_long,                 "toString16_long_long");
		registerTest(&ToStringTest::toString16_unsigned_char,             "toString16_unsigned_char");
		registerTest(&ToStringTest::toString16_unsigned_short,            "toString16_unsigned_short");
		registerTest(&ToStringTest::toString16_unsigned_int,              "toString16_unsigned_int");
		registerTest(&ToStringTest::toString16_unsigned_long,             "toString16_unsigned_long");
		registerTest(&ToStringTest::toString16_unsigned_long_long,        "toString16_unsigned_long_long");
		registerTest(&ToStringTest::toString16_float,                     "toString16_float");
		registerTest(&ToStringTest::toString16_double,                    "toString16_double");
		registerTest(&ToStringTest::toString16_long_double,               "toString16_long_double");
		registerTest(&ToStringTest::toString16_bool,                      "toString16_bool");
		registerTest(&ToStringTest::toString16_const_char_ptr,            "toString16_const_char_ptr");
		registerTest(&ToStringTest::toString16_const_char16_t_ptr,        "toString16_const_char16_t_ptr");
		registerTest(&ToStringTest::toString16_const_char32_t_ptr,        "toString16_const_char32_t_ptr");
		registerTest(&ToStringTest::toString16_const_void_ptr,            "toString16_const_void_ptr");
		registerTest(&ToStringTest::toString16_type_index,                "toString16_type_index");
		registerTest(&ToStringTest::toString16_function,                  "toString16_function");
		registerTest(&ToStringTest::toString16_vector,                    "toString16_vector");
		registerTest(&ToStringTest::toString16_macro,                     "toString16_macro");
		registerTest(&ToStringTest::toString16_parameter_pack,            "toString16_parameter_pack");
		registerTest(&ToStringTest::toString32_string,                    "toString32_string");
		registerTest(&ToStringTest::toString32_u16string,                 "toString32_u16string");
		registerTest(&ToStringTest::toString32_u32string,                 "toString32_u32string");
		registerTest(&ToStringTest::toString32_char,                      "toString32_char");
		registerTest(&ToStringTest::toString32_char16_t,                  "toString32_char16_t");
		registerTest(&ToStringTest::toString32_char32_t,                  "toString32_char32_t");
		registerTest(&ToStringTest::toString32_signed_char,               "toString32_signed_char");
		registerTest(&ToStringTest::toString32_short,                     "toString32_short");
		registerTest(&ToStringTest::toString32_int,                       "toString32_int");
		registerTest(&ToStringTest::toString32_long,                      "toString32_long");
		registerTest(&ToStringTest::toString32_long_long,                 "toString32_long_long");
		registerTest(&ToStringTest::toString32_unsigned_char,             "toString32_unsigned_char");
		registerTest(&ToStringTest::toString32_unsigned_short,            "toString32_unsigned_short");
		registerTest(&ToStringTest::toString32_unsigned_int,              "toString32_unsigned_int");
		registerTest(&ToStringTest::toString32_unsigned_long,             "toString32_unsigned_long");
		registerTest(&ToStringTest::toString32_unsigned_long_long,        "toString32_unsigned_long_long");
		registerTest(&ToStringTest::toString32_float,                     "toString32_float");
		registerTest(&ToStringTest::toString32_double,                    "toString32_double");
		registerTest(&ToStringTest::toString32_long_double,               "toString32_long_double");
		registerTest(&ToStringTest::toString32_bool,                      "toString32_bool");
		registerTest(&ToStringTest::toString32_const_char_ptr,            "toString32_const_char_ptr");
		registerTest(&ToStringTest::toString32_const_char16_t_ptr,        "toString32_const_char16_t_ptr");
		registerTest(&ToStringTest::toString32_const_char32_t_ptr,        "toString32_const_char32_t_ptr");
		registerTest(&ToStringTest::toString32_const_void_ptr,            "toString32_const_void_ptr");
		registerTest(&ToStringTest::toString32_type_index,                "toString32_type_index");
		registerTest(&ToStringTest::toString32_function,                  "toString32_function");
		registerTest(&ToStringTest::toString32_vector,                    "toString32_vector");
		registerTest(&ToStringTest::toString32_macro,                     "toString32_macro");
		registerTest(&ToStringTest::toString32_parameter_pack,            "toString32_parameter_pack");
		registerTest(&ToStringTest::toStringStruct,                       "toStringStruct");
	}

	void toString_string();
	void toString_u16string();
	void toString_u32string();
	void toString_char();
	void toString_char16_t();
	void toString_char32_t();
	void toString_signed_char();
	void toString_short();
	void toString_int();
	void toString_long();
	void toString_long_long();
	void toString_unsigned_char();
	void toString_unsigned_short();
	void toString_unsigned_int();
	void toString_unsigned_long();
	void toString_unsigned_long_long();
	void toString_float();
	void toString_double();
	void toString_long_double();
	void toString_bool();
	void toString_const_char_ptr();
	void toString_const_char16_t_ptr();
	void toString_const_char32_t_ptr();
	void toString_const_void_ptr();
	void toString_type_index();
	void toString_function();
	void toString_vector();
	void toString_macro();
	void toString_parameter_pack();
	void toString16_string();
	void toString16_u16string();
	void toString16_u32string();
	void toString16_char();
	void toString16_char16_t();
	void toString16_char32_t();
	void toString16_signed_char();
	void toString16_short();
	void toString16_int();
	void toString16_long();
	void toString16_long_long();
	void toString16_unsigned_char();
	void toString16_unsigned_short();
	void toString16_unsigned_int();
	void toString16_unsigned_long();
	void toString16_unsigned_long_long();
	void toString16_float();
	void toString16_double();
	void toString16_long_double();
	void toString16_bool();
	void toString16_const_char_ptr();
	void toString16_const_char16_t_ptr();
	void toString16_const_char32_t_ptr();
	void toString16_const_void_ptr();
	void toString16_type_index();
	void toString16_function();
	void toString16_vector();
	void toString16_macro();
	void toString16_parameter_pack();
	void toString32_string();
	void toString32_u16string();
	void toString32_u32string();
	void toString32_char();
	void toString32_char16_t();
	void toString32_char32_t();
	void toString32_signed_char();
	void toString32_short();
	void toString32_int();
	void toString32_long();
	void toString32_long_long();
	void toString32_unsigned_char();
	void toString32_unsigned_short();
	void toString32_unsigned_int();
	void toString32_unsigned_long();
	void toString32_unsigned_long_long();
	void toString32_float();
	void toString32_double();
	void toString32_long_double();
	void toString32_bool();
	void toString32_const_char_ptr();
	void toString32_const_char16_t_ptr();
	void toString32_const_char32_t_ptr();
	void toString32_const_void_ptr();
	void toString32_type_index();
	void toString32_function();
	void toString32_vector();
	void toString32_macro();
	void toString32_parameter_pack();
	void toStringStruct();
};

#endif // COM_BORA_SOFTWARE__BALAU_APPLICATION__TO_STRING_TEST
