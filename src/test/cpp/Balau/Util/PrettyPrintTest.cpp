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
#include <Balau/Util/PrettyPrint.hpp>

#include <cstdint>

namespace Balau {

using Testing::is;

namespace Util {

const std::vector<std::pair<double, std::string>> fixedTestData = {
	  std::make_pair(0.0001,                      "0.000")
	, std::make_pair(0.0012,                      "0.001")
	, std::make_pair(0.0123,                      "0.012")
	, std::make_pair(0.1234,                      "0.123")
	, std::make_pair(1.2345,                      "1.234")
	, std::make_pair(12.3456,                     "12.346")
	, std::make_pair(123.4567,                    "123.457")
	, std::make_pair(1234.5678,                   "1234.568")
	, std::make_pair(12345.6789,                  "12345.679")
	, std::make_pair(123456.7890,                 "123456.789")
	, std::make_pair(1234567.8901,                "1234567.890")
	, std::make_pair(12345678.9012,               "12345678.901")
	, std::make_pair(123456789.0123,              "123456789.012")
	, std::make_pair(1234567890.1234,             "1234567890.123")
	, std::make_pair(12345678901.2345,            "12345678901.235")
	, std::make_pair(123456789012.3456,           "123456789012.346")
	, std::make_pair(1234567890123.4567,          "1234567890123.457")
	, std::make_pair(12345678901234.5678,         "12345678901234.568")
	, std::make_pair(-12345678901234.5678,        "-12345678901234.568")
	, std::make_pair(-1234567890123.4567,         "-1234567890123.457")
	, std::make_pair(-123456789012.3456,          "-123456789012.346")
	, std::make_pair(-12345678901.2345,           "-12345678901.235")
	, std::make_pair(-1234567890.1234,            "-1234567890.123")
	, std::make_pair(-123456789.0123,             "-123456789.012")
	, std::make_pair(-12345678.9012,              "-12345678.901")
	, std::make_pair(-1234567.8901,               "-1234567.890")
	, std::make_pair(-123456.7890,                "-123456.789")
	, std::make_pair(-12345.6789,                 "-12345.679")
	, std::make_pair(-1234.5678,                  "-1234.568")
	, std::make_pair(-123.4567,                   "-123.457")
	, std::make_pair(-12.3456,                    "-12.346")
	, std::make_pair(-1.2345,                     "-1.234")
	, std::make_pair(-0.1234,                     "-0.123")
	, std::make_pair(-0.0123,                     "-0.012")
	, std::make_pair(-0.0012,                     "-0.001")
	, std::make_pair(-0.0001,                     "-0.000")
};

struct PrettyPrintTest : public Testing::TestGroup<PrettyPrintTest> {
	PrettyPrintTest() {
		RegisterTestCase(fixed);
		RegisterTestCase(scientific);
		RegisterTestCase(metricPrefix);
		RegisterTestCase(binaryPrefix);
		RegisterTestCase(byteValue);
		RegisterTestCase(duration);
	}

	void fixed() {
		for (const auto & element : fixedTestData) {
			AssertThat(PrettyPrint::fixed(element.first, 3), is(element.second));
		}
	}

	const std::vector<std::pair<double, std::string>> scientificTestData = {
		  std::make_pair(0.000000000000000001234,  "1.234e-18")
		, std::make_pair(0.00000000000000001234,   "1.234e-17")
		, std::make_pair(0.0000000000000001234,    "1.234e-16")
		, std::make_pair(0.000000000000001234,     "1.234e-15")
		, std::make_pair(0.00000000000001234,      "1.234e-14")
		, std::make_pair(0.0000000000001234,       "1.234e-13")
		, std::make_pair(0.000000000001234,        "1.234e-12")
		, std::make_pair(0.00000000001234,         "1.234e-11")
		, std::make_pair(0.0000000001234,          "1.234e-10")
		, std::make_pair(0.000000001234,           "1.234e-09")
		, std::make_pair(0.00000001234,            "1.234e-08")
		, std::make_pair(0.0000001234,             "1.234e-07")
		, std::make_pair(0.000001234,              "1.234e-06")
		, std::make_pair(0.00001234,               "1.234e-05")
		, std::make_pair(0.0001234,                "1.234e-04")
		, std::make_pair(0.001234,                 "1.234e-03")
		, std::make_pair(0.01234,                  "1.234e-02")
		, std::make_pair(0.1234,                   "1.234e-01")
		, std::make_pair(1.234,                    "1.234e+00")
		, std::make_pair(12.34,                    "1.234e+01")
		, std::make_pair(123.4,                    "1.234e+02")
		, std::make_pair(1234.0,                   "1.234e+03")
		, std::make_pair(12340.0,                  "1.234e+04")
		, std::make_pair(123400.0,                 "1.234e+05")
		, std::make_pair(1234000.0,                "1.234e+06")
		, std::make_pair(12340000.0,               "1.234e+07")
		, std::make_pair(123400000.0,              "1.234e+08")
		, std::make_pair(1234000000.0,             "1.234e+09")
		, std::make_pair(12340000000.0,            "1.234e+10")
		, std::make_pair(123400000000.0,           "1.234e+11")
		, std::make_pair(1234000000000.0,          "1.234e+12")
		, std::make_pair(12340000000000.0,         "1.234e+13")
		, std::make_pair(123400000000000.0,        "1.234e+14")
		, std::make_pair(1234000000000000.0,       "1.234e+15")
		, std::make_pair(12340000000000000.0,      "1.234e+16")
		, std::make_pair(123400000000000000.0,     "1.234e+17")
		, std::make_pair(1234000000000000000.0,    "1.234e+18")
		, std::make_pair(12340000000000000000.0,   "1.234e+19")
		, std::make_pair(123400000000000000000.0,  "1.234e+20")
		, std::make_pair(1234000000000000000000.0, "1.234e+21")
	};

	void scientific() {
		for (const auto & element : scientificTestData) {
			AssertThat(PrettyPrint::scientific(element.first, 3), is(element.second));
		}
	}

	const std::vector<std::pair<double, std::string>> metricPrefixTestData = {
		  std::make_pair(0.000000000000000001234,  "1.234a")
		, std::make_pair(0.000000000000000012345,  "12.345a")
		, std::make_pair(0.000000000000000123456,  "123.456a")
		, std::make_pair(0.000000000000001234,     "1.234f")
		, std::make_pair(0.000000000000012345,     "12.345f")
		, std::make_pair(0.000000000000123456,     "123.456f")
		, std::make_pair(0.000000000001234,        "1.234p")
		, std::make_pair(0.000000000012345,        "12.345p")
		, std::make_pair(0.000000000123456,        "123.456p")
		, std::make_pair(0.000000001234,           "1.234n")
		, std::make_pair(0.000000012345,           "12.345n")
		, std::make_pair(0.000000123456,           "123.456n")
		, std::make_pair(0.000001234,              "1.234μ")
		, std::make_pair(0.000012345,              "12.345μ")
		, std::make_pair(0.000123456,              "123.456μ")
		, std::make_pair(0.001234,                 "1.234m")
		, std::make_pair(0.012345,                 "12.345m")
		, std::make_pair(0.123456,                 "123.456m")
		, std::make_pair(1.234,                    "1.234")
		, std::make_pair(12.345,                   "12.345")
		, std::make_pair(123.456,                  "123.456")
		, std::make_pair(1234.0,                   "1.234k")
		, std::make_pair(12345.0,                  "12.345k")
		, std::make_pair(123456.0,                 "123.456k")
		, std::make_pair(1234567.0,                "1.235M")
		, std::make_pair(12345678.0,               "12.346M")
		, std::make_pair(123456789.0,              "123.457M")
		, std::make_pair(1234567890.0,             "1.235G")
		, std::make_pair(12345678901.0,            "12.346G")
		, std::make_pair(123456789012.0,           "123.457G")
		, std::make_pair(1234567890123.0,          "1.235T")
		, std::make_pair(12345678901234.0,         "12.346T")
		, std::make_pair(123456789012345.0,        "123.457T")
		, std::make_pair(1234567890123456.0,       "1.235P")
		, std::make_pair(12345678901234567.0,      "12.346P")
		, std::make_pair(123456789012345678.0,     "123.457P")
		, std::make_pair(1234567890123456789.0,    "1.235E")
		, std::make_pair(12345678901234567890.0,   "12.346E")
		, std::make_pair(123456789012345678901.0,  "123.457E")
		, std::make_pair(1234567890123456789012.0, "1234.568E")
	};

	void metricPrefix() {
		for (const auto & element : metricPrefixTestData) {
			AssertThat(PrettyPrint::metricPrefix(element.first, 3), is(element.second));
		}
	}

	const std::vector<std::pair<double, std::string>> binaryPrefixTestData = {
		  std::make_pair(pow(1024.0, -6), "1a")
		, std::make_pair(pow(1024.0, -5), "1f")
		, std::make_pair(pow(1024.0, -4), "1p")
		, std::make_pair(pow(1024.0, -3), "1n")
		, std::make_pair(pow(1024.0, -2), "1μ")
		, std::make_pair(pow(1024.0, -1), "1m")
		, std::make_pair(pow(1024.0,  0), "1")
		, std::make_pair(pow(1024.0,  1), "1k")
		, std::make_pair(pow(1024.0,  2), "1M")
		, std::make_pair(pow(1024.0,  3), "1G")
		, std::make_pair(pow(1024.0,  4), "1T")
		, std::make_pair(pow(1024.0,  5), "1P")
		, std::make_pair(pow(1024.0,  6), "1E")
	};

	void binaryPrefix() {
		for (const auto & element : binaryPrefixTestData) {
			AssertThat(PrettyPrint::binaryPrefix(element.first, 0), is(element.second));
		}
	}

	const std::vector<std::pair<double, std::string>> byteValueTestData = {
		  std::make_pair(pow(1024.0,  0), "1B")
		, std::make_pair(pow(1024.0,  1), "1kB")
		, std::make_pair(pow(1024.0,  2), "1MB")
		, std::make_pair(pow(1024.0,  3), "1GB")
		, std::make_pair(pow(1024.0,  4), "1TB")
		, std::make_pair(pow(1024.0,  5), "1PB")
		, std::make_pair(pow(1024.0,  6), "1EB")
	};

	void byteValue() {
		for (const auto & element : byteValueTestData) {
			AssertThat(PrettyPrint::byteValue((size_t) element.first), is(std::string(element.second)));
		}
	}

	const std::vector<std::pair<std::chrono::milliseconds, std::string>> durationTestData = {
		  std::make_pair(std::chrono::milliseconds(-42),       "-42.0ms")
		, std::make_pair(std::chrono::milliseconds(42),        "42.0ms")
		, std::make_pair(std::chrono::milliseconds(143123),    "02m23.1s")
		, std::make_pair(std::chrono::milliseconds(43143123),  "11h59m03.1s")
		, std::make_pair(std::chrono::milliseconds(215943123), "02d11h59m03.1s")
	};

	void duration() {
		for (const auto & element : durationTestData) {
			AssertThat(PrettyPrint::duration(element.first, 1), is(std::string(element.second)));
		}
	}
};

} // namespace Util

} // namespace Balau
