#include <gtest/gtest.h>
#include <algorithm>
#include "../IntelligentCast/IntelligentCast.h"

namespace
{
	using namespace intelligent_cast_detail;

	template<typename T>
	struct teste_data
	{
		T input_;
		std::string expected_string_;
		std::wstring expected_wstring_;
	};

	template <typename T>
	class cast_from_numeric_test : public ::testing::TestWithParam<teste_data<T> > 
	{
	public:
		void test()
		{
			auto param = GetParam();
			auto actual_string = intelligent_cast<std::string>(param.input_);
			auto actual_wstring = intelligent_cast<std::wstring>(param.input_);
			ASSERT_EQ(param.expected_string_, actual_string);
			ASSERT_EQ(param.expected_wstring_, actual_wstring);
		}
	};

#define  CREATE_TEST_DATA_2(type,name) \
	const teste_data<type> name ## _param_list[] = 

#define  CREATE_TEST_DATA(type) CREATE_TEST_DATA_2(type,type)

#define CREATE_CAST_FROM_NUMERIC_TEST_CASE_2(type, name) \
	typedef cast_from_numeric_test<type> cast_ ## name ## _from_numeric_test; \
	TEST_P(cast_ ## name ## _from_numeric_test, cast_ ## name ## _from_string) \
	{ \
		test(); \
	} \
	INSTANTIATE_TEST_CASE_P(IntelligentCast, \
                        cast_ ## name ## _from_numeric_test, \
						::testing::ValuesIn(name ## _param_list) \
						)

#define CREATE_CAST_FROM_NUMERIC_TEST_CASE(type) CREATE_CAST_FROM_NUMERIC_TEST_CASE_2(type,type)

	CREATE_TEST_DATA(int)
	{
		{INT_MIN, "-2147483648", L"-2147483648"},
		{123456789, "123456789", L"123456789" },
		{INT_MAX, "2147483647", L"2147483647"}
	};
	
	CREATE_TEST_DATA_2(unsigned int,uint)
	{
		{123456789, "123456789", L"123456789"},
		{UINT_MAX, "4294967295", L"4294967295"},
	};

	CREATE_TEST_DATA_2(unsigned char, uchar)
	{
		{123, "123", L"123"},
		{UCHAR_MAX, "255", L"255"},
	};

	CREATE_TEST_DATA(char)
	{
		{123, "123", L"123"},
		{CHAR_MAX, "127", L"127"},
		{CHAR_MIN, "-128", L"-128"},
	};

	CREATE_TEST_DATA(short)
	{
		{SHRT_MIN, "-32768", L"-32768"},
		{SHRT_MAX, "32767", L"32767"},
	};

	CREATE_TEST_DATA_2(unsigned short, ushort)
	{
		{0, "0", L"0"},
		{USHRT_MAX, "65535", L"65535"},
	};

	CREATE_TEST_DATA(long)
	{
		{LONG_MIN, "-2147483648", L"-2147483648"},
		{LONG_MAX, "2147483647", L"2147483647"}
	};

	CREATE_TEST_DATA_2(unsigned long,ulong)
	{
		{0, "0", L"0"},
		{ULONG_MAX, "4294967295", L"4294967295"},
	};

	CREATE_TEST_DATA_2(long long, llong)
	{
		{LLONG_MIN, "-9223372036854775808", L"-9223372036854775808"},
		{LLONG_MAX, "9223372036854775807", L"9223372036854775807"}
	};

	CREATE_TEST_DATA_2(unsigned long long,ullong)
	{
		{123456789, "123456789", L"123456789"},
		{ULLONG_MAX, "18446744073709551615", L"18446744073709551615"},
	};

	CREATE_TEST_DATA(double)
	{
		// default precision is 6
		{3.14159, "3.14159", L"3.14159"},
		{13.1415, "13.1415", L"13.1415"},
		{0, "0", L"0"},
		{DBL_MIN, "2.22507e-308", L"2.22507e-308"},
		{DBL_MAX, "1.79769e+308", L"1.79769e+308"}
	};

	CREATE_TEST_DATA(float)
	{
		{FLT_MIN, "1.17549e-038", L"1.17549e-038"},
		{FLT_MAX, "3.40282e+038", L"3.40282e+038"}
	};


	CREATE_CAST_FROM_NUMERIC_TEST_CASE(int);
	CREATE_CAST_FROM_NUMERIC_TEST_CASE(char);
	CREATE_CAST_FROM_NUMERIC_TEST_CASE(short);
	CREATE_CAST_FROM_NUMERIC_TEST_CASE(long);
	CREATE_CAST_FROM_NUMERIC_TEST_CASE(double);
	CREATE_CAST_FROM_NUMERIC_TEST_CASE(float);

	CREATE_CAST_FROM_NUMERIC_TEST_CASE_2(unsigned int, uint);
	CREATE_CAST_FROM_NUMERIC_TEST_CASE_2(unsigned char, uchar);
	CREATE_CAST_FROM_NUMERIC_TEST_CASE_2(unsigned short, ushort);
	CREATE_CAST_FROM_NUMERIC_TEST_CASE_2(unsigned long, ulong);
	CREATE_CAST_FROM_NUMERIC_TEST_CASE_2(long long, llong);
	CREATE_CAST_FROM_NUMERIC_TEST_CASE_2(unsigned long long, ullong);

}