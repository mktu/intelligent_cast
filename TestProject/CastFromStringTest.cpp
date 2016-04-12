#include <gtest/gtest.h>
#include <algorithm>
#include "../IntelligentCast/IntelligentCast.h"

namespace
{
	using namespace intelligent_cast_detail;

	template<typename T>
	struct teste_data
	{
		std::string input_string_;
		std::wstring input_wstring_;
		T expected_;
	};

	template <typename T>
	class cast_from_string_test : public ::testing::TestWithParam<teste_data<T> > 
	{
	public:
		void test()
		{
			auto param = GetParam();
			auto casted_from_string = intelligent_cast<T>(param.input_string_);
			auto casted_from_wstring = intelligent_cast<T>(param.input_wstring_);
			ASSERT_EQ(param.expected_, casted_from_string);
			ASSERT_EQ(param.expected_, casted_from_wstring);
		}
	};

#define  CREATE_TEST_DATA_2(type,name) \
	const teste_data<type> name ## _param_list[] = 

#define  CREATE_TEST_DATA(type) CREATE_TEST_DATA_2(type,type)

#define CREATE_CAST_FROM_STRING_TEST_CASE_2(type, name) \
	typedef cast_from_string_test<type> cast_ ## name ## _from_string_test; \
	TEST_P(cast_ ## name ## _from_string_test, cast_ ## name ## _from_string) \
	{ \
		test(); \
	} \
	INSTANTIATE_TEST_CASE_P(IntelligentCast, \
                        cast_ ## name ## _from_string_test, \
						::testing::ValuesIn(name ## _param_list) \
						)

#define CREATE_CAST_FROM_STRING_TEST_CASE(type) CREATE_CAST_FROM_STRING_TEST_CASE_2(type,type)

	CREATE_TEST_DATA(int)
	{
		{"-2147483648", L"-2147483648", INT_MIN},
		{"123456789", L"123456789", 123456789},
		{"2147483647", L"2147483647", INT_MAX}
	};

	CREATE_TEST_DATA_2(unsigned,uint)
	{
		{"123456789", L"123456789", 123456789},
		{"4294967295", L"4294967295", UINT_MAX},
	};

	CREATE_TEST_DATA_2(unsigned char,uchar)
	{
		{"123", L"123", 123},
		{"255", L"255", UCHAR_MAX},
	};

	CREATE_TEST_DATA(char)
	{
		{"123", L"123", 123},
		{"127", L"127", CHAR_MAX},
		{"-128", L"-128", CHAR_MIN},

	};

	CREATE_TEST_DATA(short)
	{
		{"-32768", L"-32768", SHRT_MIN},
		{"32767", L"32767", SHRT_MAX},
	};

	CREATE_TEST_DATA_2(unsigned short, ushort)
	{
		{"0", L"0", 0},
		{"65535", L"65535", USHRT_MAX},
	};

	CREATE_TEST_DATA(long)
	{
		{"-2147483648", L"-2147483648", LONG_MIN},
		{"2147483647", L"2147483647", LONG_MAX}
	};

	CREATE_TEST_DATA_2(unsigned long,ulong)
	{
		{"0", L"0", 0},
		{"4294967295", L"4294967295", ULONG_MAX},
	};

	CREATE_TEST_DATA_2(long long, llong)
	{
		{"-9223372036854775808", L"-9223372036854775808", LLONG_MIN},
		{"9223372036854775807", L"9223372036854775807", LLONG_MAX}
	};

	CREATE_TEST_DATA_2(unsigned long long,ullong)
	{
		{"123456789", L"123456789", 123456789},
		{"18446744073709551615 ", L"18446744073709551615 ", ULLONG_MAX},
	};

	CREATE_TEST_DATA(double)
	{
		{"-0.0", L"-0.0", 0},
		{"2.2250738585072014e-308", L"2.2250738585072014e-308", DBL_MIN},
		{"1.7976931348623158e+308", L"1.7976931348623158e+308", DBL_MAX}
	};

	CREATE_TEST_DATA(float)
	{
		{"-1.0", L"-1.0", -1.0},
		{"1.175494351e-38F", L"1.175494351e-38F", FLT_MIN},
		{"3.402823466e+38F", L"3.402823466e+38F", FLT_MAX}
	};

	CREATE_TEST_DATA_2(std::string, string)
	{
		{"abcde", L"abcde", std::string("abcde")},
		{"", L"", std::string("")}
	};

	CREATE_TEST_DATA_2(std::wstring, wstring)
	{
		{"abcde", L"abcde", std::wstring(L"abcde")},
		{"", L"", std::wstring(L"")}
	};

	CREATE_CAST_FROM_STRING_TEST_CASE(int);
	CREATE_CAST_FROM_STRING_TEST_CASE_2(unsigned int, uint);
	CREATE_CAST_FROM_STRING_TEST_CASE(char);
	CREATE_CAST_FROM_STRING_TEST_CASE_2(unsigned char, uchar);
	CREATE_CAST_FROM_STRING_TEST_CASE(short);
	CREATE_CAST_FROM_STRING_TEST_CASE_2(unsigned short, ushort);
	CREATE_CAST_FROM_STRING_TEST_CASE(long);
	CREATE_CAST_FROM_STRING_TEST_CASE_2(unsigned long, ulong);
	CREATE_CAST_FROM_STRING_TEST_CASE_2(long long, llong);
	CREATE_CAST_FROM_STRING_TEST_CASE_2(unsigned long long, ullong);
	CREATE_CAST_FROM_STRING_TEST_CASE(double);
	CREATE_CAST_FROM_STRING_TEST_CASE(float);
	CREATE_CAST_FROM_STRING_TEST_CASE_2(std::string, string);
	CREATE_CAST_FROM_STRING_TEST_CASE_2(std::wstring, wstring);


	TEST(char_to_numeric_test, char_to_numeric)
	{
		auto char_data = "0123456789";
		auto wchar_data = L"0123456789";
		auto converted_from_char = intelligent_cast<int>(char_data);
		auto converted_from_wchar = intelligent_cast<int>(wchar_data);
		ASSERT_EQ(123456789,converted_from_char);
		ASSERT_EQ(123456789,converted_from_wchar);
	}

	TEST(char_to_string_test, char_to_string)
	{
		auto converted_from_char = intelligent_cast<std::wstring>("0123456789");
		auto converted_from_wchar = intelligent_cast<std::string>( L"0123456789");
		ASSERT_EQ(L"0123456789",converted_from_char);
		ASSERT_EQ("0123456789",converted_from_wchar);
	}
}