// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include "CommandLineTest.hpp"

using Balau::Testing::assertThat;
using Balau::Testing::is;
using Balau::Testing::throws;

enum Key {
	  HELP, KEY1, KEY2, KEY3, KEY4, KEY5, KEY6, KEY7, KEY8, KEY9
};

const std::string VALUE1 = "key-one-value";
const std::string VALUE2 = "key-two-value";

const std::string FINAL_VALUE = "42";
const float FINAL_VALUE_AS_FLOAT = 42.0f;
const double FINAL_VALUE_AS_DOUBLE = 42.0;

const std::string DEFAULT_VALUE = "default-value";
const float DEFAULT_FLOAT_VALUE = 54.0f;
const double DEFAULT_DOUBLE_VALUE = 54.0;

namespace Balau {

///////////////////////////// Test implementations ////////////////////////////

void basicImpl(int argc, const char * argv[], CommandLineStyle style) {
	auto commandLine = CommandLine<Key>(style)
		.withOption(KEY1, "k", "key-one", true, "The first key.")
		.withOption(KEY2, "m", "key-two", true, "The second key.")
		.withOption(KEY3, "3", false, "Specify in order to use third style.")
		.withOption(KEY4, "n", "key-four", true, "The fourth key.")
		.withHelpOption(HELP, "h", "help", "Displays this help message");

	commandLine.parse(argc, argv, false);

	assertThat(commandLine.getOption(KEY1), is(VALUE1));
	assertThat(commandLine.getOption(KEY2), is(VALUE2));
	assertThat(commandLine.hasOption(KEY3), is(true));
	assertThat(commandLine.hasOption(KEY4), is(false));

	assertThat(commandLine.getOptionOrDefault(KEY2, DEFAULT_VALUE), is(VALUE2));
	assertThat(commandLine.getOptionOrDefault(KEY4, DEFAULT_VALUE), is(DEFAULT_VALUE));
}

void finalValueImpl(int argc, const char * argv[], CommandLineStyle style) {
	auto commandLine = CommandLine<Key>(style)
		.withOption(KEY1, "k", "key-one", true, "The first key.")
		.withOption(KEY2, "m", "key-two", true, "The second key.")
		.withOption(KEY3, "3", false, "Specify in order to use third style.")
		.withOption(KEY4, "n", "key-four", true, "The fourth key.")
		.withHelpOption(HELP, "h", "help", "Displays this help message")
		.withFinalValue();

	commandLine.parse(argc, argv, false);

	assertThat(commandLine.hasFinalValue(),                         is(true));
	assertThat(commandLine.getOption(KEY1),                         is(VALUE1));
	assertThat(commandLine.getOption(KEY2),                         is(VALUE2));
	assertThat(commandLine.hasOption(KEY3),                         is(true));
	assertThat(commandLine.hasOption(KEY4),                         is(false));
	assertThat(commandLine.getOptionOrDefault(KEY1, DEFAULT_VALUE), is(VALUE1));
	assertThat(commandLine.getOptionOrDefault(KEY4, DEFAULT_VALUE), is(DEFAULT_VALUE));
	assertThat(commandLine.getFinalValue(),                         is(FINAL_VALUE));

	assertThat(commandLine.getFinalValue(),                                      is(FINAL_VALUE));
	assertThat(commandLine.getFinalValueAsFloat(),                               is(FINAL_VALUE_AS_FLOAT));
	assertThat(commandLine.getFinalValueAsDouble(),                              is(FINAL_VALUE_AS_DOUBLE));
	assertThat(commandLine.getFinalValueOrDefault(DEFAULT_VALUE),                is(FINAL_VALUE));
	assertThat(commandLine.getFinalValueAsFloatOrDefault(DEFAULT_FLOAT_VALUE),   is(FINAL_VALUE_AS_FLOAT));
	assertThat(commandLine.getFinalValueAsDoubleOrDefault(DEFAULT_DOUBLE_VALUE), is(FINAL_VALUE_AS_DOUBLE));
}

const signed short       sValue = -23;
const unsigned short     usValue = 42U;
const signed int         iValue = -123456789;
const unsigned int       uiValue = 353567643U;
const signed long long   lValue = -14234237653423324L;
const unsigned long long ulValue = 34523457456865723UL;
const float              fValue = 32.56f;
const double             dValue = 123.456e-7;

const std::string sValueStr  = ::toString(sValue);
const std::string usValueStr = ::toString(usValue);
const std::string iValueStr  = ::toString(iValue);
const std::string uiValueStr = ::toString(uiValue);
const std::string lValueStr  = ::toString(lValue);
const std::string ulValueStr = ::toString(ulValue);
const std::string fValueStr  = ::toString(fValue);
const std::string dValueStr  = ::toString(dValue);

void numericValueImpl(int argc, const char * argv[], CommandLineStyle style) {
	auto commandLine = CommandLine<Key>(style)
		.withOption(KEY1, "a", true, "Documentation line for switch 'a'.")
		.withOption(KEY2, "b", true, "Documentation line for switch 'b'.")
		.withOption(KEY3, "c", true, "Documentation line for switch 'c'.")
		.withOption(KEY4, "d", true, "Documentation line for switch 'd'.")
		.withOption(KEY5, "e", true, "Documentation line for switch 'e'.")
		.withOption(KEY6, "f", true, "Documentation line for switch 'f'.")
		.withOption(KEY7, "g", true, "Documentation line for switch 'g'.")
		.withOption(KEY8, "i", true, "Documentation line for switch 'i'.")
		.withHelpOption(HELP, "h", "help", "Displays this help message");

	commandLine.parse(argc, argv, true);

	assertThat(commandLine.hasFinalValue(), is(false));

	assertThat(commandLine.getOption(KEY1), is(sValueStr));
	assertThat(commandLine.getOption(KEY2), is(usValueStr));
	assertThat(commandLine.getOption(KEY3), is(iValueStr));
	assertThat(commandLine.getOption(KEY4), is(uiValueStr));
	assertThat(commandLine.getOption(KEY5), is(lValueStr));
	assertThat(commandLine.getOption(KEY6), is(ulValueStr));
	assertThat(commandLine.getOption(KEY7), is(fValueStr));
	assertThat(commandLine.getOption(KEY8), is(dValueStr));

	assertThat([&] () { commandLine.getOption(KEY9); }, throws(Exception::OptionNotFoundException("", 0, "", ::toString(KEY9))));

	assertThat([&] () { commandLine.getOption(KEY9); }, throws<Exception::OptionNotFoundException>());

	assertThat(commandLine.getOptionOrDefault(KEY1, ""), is(sValueStr));
	assertThat(commandLine.getOptionOrDefault(KEY2, ""), is(usValueStr));
	assertThat(commandLine.getOptionOrDefault(KEY3, ""), is(iValueStr));
	assertThat(commandLine.getOptionOrDefault(KEY4, ""), is(uiValueStr));
	assertThat(commandLine.getOptionOrDefault(KEY5, ""), is(lValueStr));
	assertThat(commandLine.getOptionOrDefault(KEY6, ""), is(ulValueStr));
	assertThat(commandLine.getOptionOrDefault(KEY7, ""), is(fValueStr));
	assertThat(commandLine.getOptionOrDefault(KEY8, ""), is(dValueStr));

	assertThat(commandLine.getOptionAsShort(KEY1),         is(std::stoi(sValueStr)));
	assertThat(commandLine.getOptionAsUnsignedShort(KEY2), is(std::stoul(usValueStr)));
	assertThat(commandLine.getOptionAsInt(KEY3),           is(std::stoi(iValueStr)));
	assertThat(commandLine.getOptionAsUnsignedInt(KEY4),   is(std::stoul(uiValueStr)));
	assertThat(commandLine.getOptionAsLong(KEY5),          is(std::stol(lValueStr)));
	assertThat(commandLine.getOptionAsUnsignedLong(KEY6),  is(std::stoul(ulValueStr)));
	assertThat(commandLine.getOptionAsFloat(KEY7),         is(std::stof(fValueStr)));
	assertThat(commandLine.getOptionAsDouble(KEY8),        is(std::stod(dValueStr)));

	assertThat(commandLine.getOptionAsShortOrDefault(KEY1, 0),         is(std::stoi(sValueStr)));
	assertThat(commandLine.getOptionAsUnsignedShortOrDefault(KEY2, 0), is(std::stoul(usValueStr)));
	assertThat(commandLine.getOptionAsIntOrDefault(KEY3, 0),           is(std::stoi(iValueStr)));
	assertThat(commandLine.getOptionAsUnsignedIntOrDefault(KEY4, 0),   is(std::stoul(uiValueStr)));
	assertThat(commandLine.getOptionAsLongOrDefault(KEY5, 0),          is(std::stol(lValueStr)));
	assertThat(commandLine.getOptionAsUnsignedLongOrDefault(KEY6, 0),  is(std::stoul(ulValueStr)));
	assertThat(commandLine.getOptionAsFloatOrDefault(KEY7, 0),         is(std::stof(fValueStr)));
	assertThat(commandLine.getOptionAsDoubleOrDefault(KEY8, 0),        is(std::stod(dValueStr)));
	assertThat(commandLine.getOptionAsDoubleOrDefault(KEY9, -42.0),    is(-42.0));

	assertThat([&] () { commandLine.getFinalValue();         }, throws<Exception::OptionNotFoundException>());
	assertThat([&] () { commandLine.getFinalValueAsFloat();  }, throws<Exception::OptionNotFoundException>());
	assertThat([&] () { commandLine.getFinalValueAsDouble(); }, throws<Exception::OptionNotFoundException>());

	assertThat(commandLine.getFinalValueOrDefault(DEFAULT_VALUE),                is(DEFAULT_VALUE));
	assertThat(commandLine.getFinalValueAsFloatOrDefault(DEFAULT_FLOAT_VALUE),   is(DEFAULT_FLOAT_VALUE));
	assertThat(commandLine.getFinalValueAsDoubleOrDefault(DEFAULT_DOUBLE_VALUE), is(DEFAULT_DOUBLE_VALUE));
}

////////////////////////// Switch-space-value tests ///////////////////////////

void CommandLineTest::basicSsv() {
	const int argc = 5;

	const char * argv[] = {
		"-k", VALUE1.c_str(), "-3", "--key-two", VALUE2.c_str()
	};

	basicImpl(argc, argv, CommandLineStyle::SwitchSpaceValue);
}

void CommandLineTest::finalValueSsv() {
	const int argc = 6;

	const char * argv[] = {
		  "-k", VALUE1.c_str()
		, "-3"
		, "--key-two", VALUE2.c_str()
		, FINAL_VALUE.c_str()
	};

	finalValueImpl(argc, argv, CommandLineStyle::SwitchSpaceValue);
}

void CommandLineTest::numericValueSsv() {
	const int argc = 17;

	const char * argv[] = {
		  "appName"
		, "-a", sValueStr.c_str()
		, "-b", usValueStr.c_str()
		, "-c", iValueStr.c_str()
		, "-d", uiValueStr.c_str()
		, "-e", lValueStr.c_str()
		, "-f", ulValueStr.c_str()
		, "-g", fValueStr.c_str()
		, "-i", dValueStr.c_str()
	};

	numericValueImpl(argc, argv, CommandLineStyle::SwitchSpaceValue);
}

void CommandLineTest::docOutputSsv() {
	auto commandLine = CommandLine<Key>()
		.withOption(KEY1, "k", "key-one", true, "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.")
		.withOption(KEY2, "m", "key-two", true, "Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.")
		.withOption(KEY3, "3", false, "ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam.")
		.withOption(KEY4, "n", "key-four", true, "The fourth key.")
		.withHelpOption(HELP, "h", "help", "Displays this help message")
		.withFinalValue();

	const std::string actual = commandLine.getHelpText(4, 80);

	const std::string expected = 1 + R"(
    -h, --help     - Displays this help message
    -k, --key-one  - Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor i
                     ncididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostru
                     d exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aut
                     e irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat n
                     ulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui
                      officia deserunt mollit anim id est laborum.
    -m, --key-two  - Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deseru
                     nt mollit anim id est laborum.
    -3             - ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incidid
                     unt ut labore et dolore magna aliqua. Ut enim ad minim veniam.
    -n, --key-four - The fourth key.
)";

	assertThat(actual, is(expected));
}

////////////////////////// Switch-equals-value tests //////////////////////////

void CommandLineTest::basicSev() {
	const int argc = 3;

	const std::string a0 = "k=" + VALUE1;
	const std::string a1 = "3";
	const std::string a2 = "key-two=" + VALUE2;

	const char * argv[] = {
		  a0.c_str()
		, a1.c_str()
		, a2.c_str()
	};

	basicImpl(argc, argv, CommandLineStyle::SwitchEqualsValue);
}

void CommandLineTest::finalValueSev() {
	const int argc = 4;

	const std::string a0 = "k=" + VALUE1;
	const std::string a1 = "3";
	const std::string a2 = "key-two=" + VALUE2;

	const char * argv[] = {
		  a0.c_str()
		, a1.c_str()
		, a2.c_str()
		, FINAL_VALUE.c_str()
	};

	finalValueImpl(argc, argv, CommandLineStyle::SwitchEqualsValue);
}

void CommandLineTest::numericValueSev() {
	const int argc = 9;

	const std::string a0 = "a=" + sValueStr;
	const std::string a1 = "b=" + usValueStr;
	const std::string a2 = "c=" + iValueStr;
	const std::string a3 = "d=" + uiValueStr;
	const std::string a4 = "e=" + lValueStr;
	const std::string a5 = "f=" + ulValueStr;
	const std::string a6 = "g=" + fValueStr;
	const std::string a7 = "i=" + dValueStr;

	const char * argv[] = {
		  "appName"
		, a0.c_str()
		, a1.c_str()
		, a2.c_str()
		, a3.c_str()
		, a4.c_str()
		, a5.c_str()
		, a6.c_str()
		, a7.c_str()
	};

	numericValueImpl(argc, argv, CommandLineStyle::SwitchEqualsValue);
}

void CommandLineTest::docOutputSev() {
	auto commandLine = CommandLine<Key>(CommandLineStyle::SwitchEqualsValue)
		.withOption(KEY1, "k", "key-one", true, "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.")
		.withOption(KEY2, "m", "key-two", true, "Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.")
		.withOption(KEY3, "3", false, "ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam.")
		.withOption(KEY4, "n", "key-four", true, "The fourth key.")
		.withHelpOption(HELP, "h", "help", "Displays this help message")
		.withFinalValue();

	const std::string actual = commandLine.getHelpText(4, 80);

	const std::string expected = 1 + R"(
    h, help     - Displays this help message
    k, key-one  - Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor i
                  ncididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostru
                  d exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aut
                  e irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat n
                  ulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui
                   officia deserunt mollit anim id est laborum.
    m, key-two  - Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deseru
                  nt mollit anim id est laborum.
    3           - ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incidid
                  unt ut labore et dolore magna aliqua. Ut enim ad minim veniam.
    n, key-four - The fourth key.
)";

	assertThat(actual, is(expected));
}

//////////////////////////////// Detect tests /////////////////////////////////

void CommandLineTest::basicDetect() {
	const int argc1 = 5;

	const char * argv1[] = {
		"-k", VALUE1.c_str(), "-3", "--key-two", VALUE2.c_str()
	};

	basicImpl(argc1, argv1, CommandLineStyle::Detect);

	const int argc2 = 3;

	const std::string a0 = "k=" + VALUE1;
	const std::string a1 = "3";
	const std::string a2 = "key-two=" + VALUE2;

	const char * argv2[] = {
		  a0.c_str()
		, a1.c_str()
		, a2.c_str()
	};

	basicImpl(argc2, argv2, CommandLineStyle::Detect);
}

void CommandLineTest::finalValueDetect() {
	const int argc1 = 6;

	const char * argv1[] = {
		"-k", VALUE1.c_str()
		, "-3"
		, "--key-two", VALUE2.c_str()
		, FINAL_VALUE.c_str()
	};

	finalValueImpl(argc1, argv1, CommandLineStyle::Detect);

	const int argc2 = 4;

	const std::string a0 = "k=" + VALUE1;
	const std::string a1 = "3";
	const std::string a2 = "key-two=" + VALUE2;

	const char * argv2[] = {
		  a0.c_str()
		, a1.c_str()
		, a2.c_str()
		, FINAL_VALUE.c_str()
	};

	finalValueImpl(argc2, argv2, CommandLineStyle::Detect);
}

void CommandLineTest::numericValueDetect() {
	const int argc1 = 17;

	const char * argv1[] = {
		  "appName"
		, "-a", sValueStr.c_str()
		, "-b", usValueStr.c_str()
		, "-c", iValueStr.c_str()
		, "-d", uiValueStr.c_str()
		, "-e", lValueStr.c_str()
		, "-f", ulValueStr.c_str()
		, "-g", fValueStr.c_str()
		, "-i", dValueStr.c_str()
	};

	numericValueImpl(argc1, argv1, CommandLineStyle::Detect);

	const int argc2 = 9;

	const std::string a0 = "a=" + sValueStr;
	const std::string a1 = "b=" + usValueStr;
	const std::string a2 = "c=" + iValueStr;
	const std::string a3 = "d=" + uiValueStr;
	const std::string a4 = "e=" + lValueStr;
	const std::string a5 = "f=" + ulValueStr;
	const std::string a6 = "g=" + fValueStr;
	const std::string a7 = "i=" + dValueStr;

	const char * argv2[] = {
		"appName"
		, a0.c_str()
		, a1.c_str()
		, a2.c_str()
		, a3.c_str()
		, a4.c_str()
		, a5.c_str()
		, a6.c_str()
		, a7.c_str()
	};

	numericValueImpl(argc2, argv2, CommandLineStyle::Detect);
}

void CommandLineTest::docOutputDetect() {

}

} // namespace Balau
