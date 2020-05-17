// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include <TestResources.hpp>

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

	void toString_string() {
		AssertThat(::toString(std::string("qwerty")), is(std::string("qwerty")));
	}
	
	void toString_u16string() {
		AssertThat(::toString(std::u16string(u"qwerty")), is(std::string("qwerty")));
	}
	
	void toString_u32string() {
		AssertThat(::toString(std::u32string(U"qwerty")), is(std::string("qwerty")));
	}
	
	void toString_char() {
		AssertThat(::toString('c'), is(std::string("c")));
	}
	
	void toString_char16_t() {
		char16_t c = u'c';
		AssertThat(::toString(c), is(std::string("c")));
	}
	
	void toString_char32_t() {
		char32_t c = U'c';
		AssertThat(::toString(c), is(std::string("c")));
	}
	
	void toString_signed_char() {
		AssertThat(::toString((signed char) 42), is(std::string("42")));
	}
	
	void toString_short() {
		AssertThat(::toString((short) 42), is(std::string("42")));
	}
	
	void toString_int() {
		AssertThat(::toString((int) 42), is(std::string("42")));
	}
	
	void toString_long() {
		AssertThat(::toString((long) 42), is(std::string("42")));
	}
	
	void toString_long_long() {
		AssertThat(::toString((long long) 42), is(std::string("42")));
	}
	
	void toString_unsigned_char() {
		AssertThat(::toString((unsigned char) 42), is(std::string("42")));
	}
	
	void toString_unsigned_short() {
		AssertThat(::toString((unsigned short) 42), is(std::string("42")));
	}
	
	void toString_unsigned_int() {
		AssertThat(::toString((unsigned int) 42), is(std::string("42")));
	}
	
	void toString_unsigned_long() {
		AssertThat(::toString((unsigned long) 42), is(std::string("42")));
	}
	
	void toString_unsigned_long_long() {
		AssertThat(::toString((unsigned long long) 42), is(std::string("42")));
	}
	
	void toString_float() {
		AssertThat(::toString(42.2f), startsWith(std::string("42.2")));
	}
	
	void toString_double() {
		AssertThat(::toString(42.2), startsWith(std::string("42.2")));
	}
	
	void toString_long_double() {
		AssertThat(::toString((long double) 42.2), startsWith(std::string("42.2")));
	}
	
	void toString_bool() {
		AssertThat(::toString(true), is(std::string("true")));
		AssertThat(::toString(false), is(std::string("false")));
	}
	
	void toString_const_char_ptr() {
		AssertThat(::toString("qwerty"), is(std::string("qwerty")));
	}
	
	void toString_const_char16_t_ptr() {
		AssertThat(::toString(u"qwerty"), is(std::string("qwerty")));
	}
	
	void toString_const_char32_t_ptr() {
		AssertThat(::toString(U"qwerty"), is(std::string("qwerty")));
	}
	
	void toString_const_void_ptr() {
		void * value = (void *) 42;
		AssertThat(::toString(value), is(std::string("42")));
	}
	
	void toString_type_index() {
		std::type_index ti(typeid(ToStringTest));
		AssertThat(::toString(ti), contains(std::string("ToStringTest")));
	}
	
	void toString_function() {
		std::string s = "test string";
		AssertThat(::toString(std::function<std::string ()>([&s] () { return s; })), is(s));
	}
	
	void toString_vector() {
		std::vector<long> v { 1, -5, 3, 8 };
		AssertThat(::toString(v), is(std::string("{ 1, -5, 3, 8 }")));
	}
	
	template <typename T> struct TestContainer {
		std::unique_ptr<T[]> data;
		size_t size;
	
		TestContainer(std::initializer_list<T> items) {
			data = std::unique_ptr<T[]>(new T[items.size()]);
			std::copy(items.begin(), items.end(), data.get());
			size = items.size();
		}
	
		const T * begin() const {
			return data.get();
		}
	
		const T * end() const {
			return data.get() + size;
		}
	};
	
	BALAU_CONTAINER1_TO_STRING(TestContainer)
	
	void toString_macro() {
		TestContainer<int> c { 1, 2, 3, 4 };
		AssertThat(toString(c), is(std::string("{ 1, 2, 3, 4 }")));
	}
	
	void toString_parameter_pack() {
		AssertThat(::toString(1, ",", "qwerty", ",", 42U), is(std::string("1,qwerty,42")));
	}
	
	void toString16_string() {
		AssertThat(::toString16(std::string("qwerty")), is(std::u16string(u"qwerty")));
	}
	
	void toString16_u16string() {
		AssertThat(::toString16(std::u16string(u"qwerty")), is(std::u16string(u"qwerty")));
	}
	
	void toString16_u32string() {
		AssertThat(::toString16(std::u32string(U"qwerty")), is(std::u16string(u"qwerty")));
	}
	
	void toString16_char() {
		AssertThat(::toString16('c'), is(std::u16string(u"c")));
	}
	
	void toString16_char16_t() {
		char16_t c = u'c';
		AssertThat(::toString16(c), is(std::u16string(u"c")));
	}
	
	void toString16_char32_t() {
		char32_t c = U'c';
		AssertThat(::toString16(c), is(std::u16string(u"c")));
	}
	
	void toString16_signed_char() {
		AssertThat(::toString16((signed char) 42), is(std::u16string(u"42")));
	}
	
	void toString16_short() {
		AssertThat(::toString16((short) 42), is(std::u16string(u"42")));
	}
	
	void toString16_int() {
		AssertThat(::toString16((int) 42), is(std::u16string(u"42")));
	}
	
	void toString16_long() {
		AssertThat(::toString16((long) 42), is(std::u16string(u"42")));
	}
	
	void toString16_long_long() {
		AssertThat(::toString16((long long) 42), is(std::u16string(u"42")));
	}
	
	void toString16_unsigned_char() {
		AssertThat(::toString16((unsigned char) 42), is(std::u16string(u"42")));
	}
	
	void toString16_unsigned_short() {
		AssertThat(::toString16((unsigned short) 42), is(std::u16string(u"42")));
	}
	
	void toString16_unsigned_int() {
		AssertThat(::toString16((unsigned int) 42), is(std::u16string(u"42")));
	}
	
	void toString16_unsigned_long() {
		AssertThat(::toString16((unsigned long) 42), is(std::u16string(u"42")));
	}
	
	void toString16_unsigned_long_long() {
		AssertThat(::toString16((unsigned long long) 42), is(std::u16string(u"42")));
	}
	
	void toString16_float() {
		AssertThat(::toString16(42.2f), startsWith(std::u16string(u"42.2")));
	}
	
	void toString16_double() {
		AssertThat(::toString16(42.2), startsWith(std::u16string(u"42.2")));
	}
	
	void toString16_long_double() {
		AssertThat(::toString16((long double) 42.2), startsWith(std::u16string(u"42.2")));
	}
	
	void toString16_bool() {
		AssertThat(::toString16(true), is(std::u16string(u"true")));
		AssertThat(::toString16(false), is(std::u16string(u"false")));
	}
	
	void toString16_const_char_ptr() {
		AssertThat(::toString16("qwerty"), is(std::u16string(u"qwerty")));
	}
	
	void toString16_const_char16_t_ptr() {
		AssertThat(::toString16(u"qwerty"), is(std::u16string(u"qwerty")));
	}
	
	void toString16_const_char32_t_ptr() {
		AssertThat(::toString16(U"qwerty"), is(std::u16string(u"qwerty")));
	}
	
	void toString16_const_void_ptr() {
		void * value = (void *) 42;
		AssertThat(::toString16(value), is(std::u16string(u"42")));
	}
	
	void toString16_type_index() {
		std::type_index ti(typeid(ToStringTest));
		AssertThat(::toString16(ti), contains(std::u16string(u"ToStringTest")));
	}
	
	void toString16_function() {
		std::u16string s = u"test string";
		AssertThat(::toString16(std::function<std::u16string ()>([&s] () { return s; })), is(s));
	}
	
	void toString16_vector() {
		std::vector<long> v { 1, -5, 3, 8 };
		AssertThat(::toString16(v), is(std::u16string(u"{ 1, -5, 3, 8 }")));
	}
	
	BALAU_CONTAINER1_TO_STRING16(TestContainer)
	
	void toString16_macro() {
		TestContainer<int> c { 1, 2, 3, 4 };
		AssertThat(toString16(c), is(std::u16string(u"{ 1, 2, 3, 4 }")));
	}
	
	void toString16_parameter_pack() {
		AssertThat(::toString16(1, ",", u"qwerty", ",", 42U), is(std::u16string(u"1,qwerty,42")));
	}
	
	void toString32_string() {
		AssertThat(::toString32(std::string("qwerty")), is(std::u32string(U"qwerty")));
	}
	
	void toString32_u16string() {
		AssertThat(::toString32(std::u16string(u"qwerty")), is(std::u32string(U"qwerty")));
	}
	
	void toString32_u32string() {
		AssertThat(::toString32(std::u32string(U"qwerty")), is(std::u32string(U"qwerty")));
	}
	
	void toString32_char() {
		AssertThat(::toString32('c'), is(std::u32string(U"c")));
	}
	
	void toString32_char16_t() {
		char16_t c = u'c';
		AssertThat(::toString32(c), is(std::u32string(U"c")));
	}
	
	void toString32_char32_t() {
		char32_t c = U'c';
		AssertThat(::toString32(c), is(std::u32string(U"c")));
	}
	
	void toString32_signed_char() {
		AssertThat(::toString32((signed char) 42), is(std::u32string(U"42")));
	}
	
	void toString32_short() {
		AssertThat(::toString32((short) 42), is(std::u32string(U"42")));
	}
	
	void toString32_int() {
		AssertThat(::toString32((int) 42), is(std::u32string(U"42")));
	}
	
	void toString32_long() {
		AssertThat(::toString32((long) 42), is(std::u32string(U"42")));
	}
	
	void toString32_long_long() {
		AssertThat(::toString32((long long) 42), is(std::u32string(U"42")));
	}
	
	void toString32_unsigned_char() {
		AssertThat(::toString32((unsigned char) 42), is(std::u32string(U"42")));
	}
	
	void toString32_unsigned_short() {
		AssertThat(::toString32((unsigned short) 42), is(std::u32string(U"42")));
	}
	
	void toString32_unsigned_int() {
		AssertThat(::toString32((unsigned int) 42), is(std::u32string(U"42")));
	}
	
	void toString32_unsigned_long() {
		AssertThat(::toString32((unsigned long) 42), is(std::u32string(U"42")));
	}
	
	void toString32_unsigned_long_long() {
		AssertThat(::toString32((unsigned long long) 42), is(std::u32string(U"42")));
	}
	
	void toString32_float() {
		AssertThat(::toString32(42.2f), startsWith(std::u32string(U"42.2")));
	}
	
	void toString32_double() {
		AssertThat(::toString32(42.2), startsWith(std::u32string(U"42.2")));
	}
	
	void toString32_long_double() {
		AssertThat(::toString32((long double) 42.2), startsWith(std::u32string(U"42.2")));
	}
	
	void toString32_bool() {
		AssertThat(::toString32(true), is(std::u32string(U"true")));
		AssertThat(::toString32(false), is(std::u32string(U"false")));
	}
	
	void toString32_const_char_ptr() {
		AssertThat(::toString32("qwerty"), is(std::u32string(U"qwerty")));
	}
	
	void toString32_const_char16_t_ptr() {
		AssertThat(::toString32(u"qwerty"), is(std::u32string(U"qwerty")));
	}
	
	void toString32_const_char32_t_ptr() {
		AssertThat(::toString32(U"qwerty"), is(std::u32string(U"qwerty")));
	}
	
	void toString32_const_void_ptr() {
		void * value = (void *) 42;
		AssertThat(::toString32(value), is(std::u32string(U"42")));
	}
	
	void toString32_type_index() {
		std::type_index ti(typeid(ToStringTest));
		AssertThat(::toString32(ti), contains(std::u32string(U"ToStringTest")));
	}
	
	void toString32_function() {
		std::u32string s = U"test string";
		AssertThat(::toString32(std::function<std::u32string ()>([&s] () { return s; })), is(s));
	}
	
	void toString32_vector() {
		std::vector<long> v { 1, -5, 3, 8 };
		AssertThat(::toString32(v), is(std::u32string(U"{ 1, -5, 3, 8 }")));
	}
	
	BALAU_CONTAINER1_TO_STRING32(TestContainer)
	
	void toString32_macro() {
		TestContainer<int> c { 1, 2, 3, 4 };
		AssertThat(toString32(c), is(std::u32string(U"{ 1, 2, 3, 4 }")));
	}
	
	void toString32_parameter_pack() {
		AssertThat(::toString32(1, ",", u"qwerty", ",", 42U), is(std::u32string(U"1,qwerty,42")));
	}
	
	template <typename CharT> class ToStringCTestClass {
		public: std::basic_string<CharT> getIntAsString() const {
			return ToString<CharT>()(42);
		}
	
		public: std::basic_string<CharT> getDoubleAsString() const {
			return ToString<CharT>()(123.456);
		}
	};
	
	void toStringStruct() {
		ToStringCTestClass<char> utf8Test;
	
		AssertThat(utf8Test.getIntAsString(), is(std::string("42")));
		AssertThat(utf8Test.getDoubleAsString(), is(std::string("123.456")));
	
		ToStringCTestClass<char16_t> utf16Test;
	
		AssertThat(utf16Test.getIntAsString(), is(std::u16string(u"42")));
		AssertThat(utf16Test.getDoubleAsString(), is(std::u16string(u"123.456")));
	
		ToStringCTestClass<char32_t> utf32Test;
	
		AssertThat(utf32Test.getIntAsString(), is(std::u32string(U"42")));
		AssertThat(utf32Test.getDoubleAsString(), is(std::u32string(U"123.456")));
	}
};
