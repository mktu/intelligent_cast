#include <string>

#include "IntelligentCast.h" // for basic usage
#include "OperatorOverloadExtension.h" // for advanced usage


int main()
{
// *** Basic usage ***
	// convert to string from integer 
	std::string string_from_int = intelligent_cast<std::string>(100);

	// convert to string from double 
	std::string string_from_double = intelligent_cast<std::string>(1.79769e+308);

	// convert to integer from string
	int int_from_char = intelligent_cast<int>("100");

	// convert to double from string
	double double_from_char = intelligent_cast<double>("1.79769e+308");

	// convert to wchar from char
	std::wstring wstring_from_string  = intelligent_cast<std::wstring>("test");

	// convert to char from wchar
	std::string string_from_wstring  = intelligent_cast<std::string>(L"test");

// *** Advanced usage ***
	std::string str="results are ";
	std::string converted = str + 10 + " and " + 5.5;
	// converted should be "results are 10 and 5.5"

	return 0;
}