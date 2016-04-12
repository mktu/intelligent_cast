#include <gtest/gtest.h>
#include <algorithm>
#include "../IntelligentCast/OperatorOverloadExtension.h"



namespace
{
	using namespace intelligent_cast_detail;
			
	TEST(adding_operation_test, adding_operation)
	{
		static_assert(is_string_castable<std::string,int>::value,"");
		static_assert(!is_string_castable<int,std::string>::value,"");
		static_assert(is_string_castable<std::string,decltype("char")>::value,"");
		static_assert(!can_allow_adding_operator<std::string,const char*>::value,"");
		auto s1 = std::string("ge") + "l";
		auto s2 = std::string("") + 10 + "15" + 1.5 + L"13" + std::string("");
		auto s3 = std::wstring(L"") + 10 + "15" + 1.5 + L"13" + std::string("");

		ASSERT_EQ(std::string("10151.513"), s2);
		ASSERT_EQ(std::wstring(L"10151.513"), s3);
	}
}