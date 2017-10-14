#pragma once
#include <tchar.h>
#include <vector>
#include <string>
#include <locale.h>

namespace intelligent_cast_detail
{

	//---------------------------------------------------------------------------
	/**
	 * @brief string->wstring conversion
	 * @return std::wstring 
	 * @param[in]	src : input string
	 *
	 *	if conversion failed, this provides the only characters that can be converted.
	 */
	//---------------------------------------------------------------------------
	inline
	std::wstring to_wstring_impl(const std::string& src)
	{
		std::vector<wchar_t> to(src.length() + 1);
		size_t size;
		mbstowcs_s(&size, &to[0], to.size(), src.c_str(), _TRUNCATE);
		return std::wstring(&to[0], size-1);
	}
	//---------------------------------------------------------------------------
	/**
	 * @brief wstring->string conversion
	 * @return std::string
	 * @param[in]	src : input string
	 *
	 *	if conversion failed, this provides the only characters that can be converted.
	 */
	//---------------------------------------------------------------------------
	inline
	std::string to_mbstring_impl(const std::wstring& src)
	{
		std::vector<char> to(src.length() * MB_CUR_MAX + 1);
		size_t size;
		wcstombs_s(&size, &to[0], to.size(), src.c_str(), _TRUNCATE);
		return std::string(&to[0], size-1);
	}
}	// End of Namespace intelligent_cast_detail

inline
std::string to_mbstring(const std::wstring& src)
{
	return intelligent_cast_detail::to_mbstring_impl(src);
}

inline
const std::string& to_mbstring(const std::string& src)
{
	return src;
}

inline
std::wstring to_wstring(const std::string& src)
{
	return intelligent_cast_detail::to_wstring_impl(src);
}

inline
const std::wstring& to_wstring(const std::wstring& src)
{
	return src;
}


namespace intelligent_cast_detail
{
	//---------------------------------------------------------------------------
	/**
	 * @brief wstring <-> string conversion
	 * @return std::wstring 
	 * @param[in]	src : input
	 *
	 *	this function will be overloaded to interconvert.
	 */
	//---------------------------------------------------------------------------
	inline
	std::wstring interconvert_string(const std::string& src)
	{
		return to_wstring(src);
	}
	//---------------------------------------------------------------------------
	/**
	 * @brief wstring <-> string conversion
	 * @return std::string 
	 * @param[in]	src : input
	 */
	//---------------------------------------------------------------------------
	inline
	std::string interconvert_string(const std::wstring& src)
	{
		return to_mbstring(src);
	}
	//---------------------------------------------------------------------------
	/**
	 * @brief format string
	 * @return std::string
	 * @param[in]	src input
	 * @param[in]	format : format
	 *
	 *	it format string that include numeric value
	 */
	//---------------------------------------------------------------------------
	template<typename T>
	std::string format(const T& src, const char* format)
	{
		char buf[255];
		sprintf_s(buf, sizeof(buf)/sizeof(char), format, src); 
		return std::string(buf);
	}
	template<typename T>
	std::wstring format(const T& src, const wchar_t* format)
	{
		wchar_t buf[255];
		swprintf_s(buf, sizeof(buf)/sizeof(wchar_t), format, src); 
		return std::wstring(buf);
	}
	//---------------------------------------------------------------------------
	/**
	 * @brief conversion rule for convertiong string to another type of string
	 *
	 *	this avoids unnecessary copying when From type and To type are same string type.
	 */
	//---------------------------------------------------------------------------
	template<typename From, typename To>
	struct string_conversion_rules
	{
		typedef To result_type;
		static result_type convert(const From& from){return interconvert_string(from);} 
	};
	template<typename From>
	struct string_conversion_rules<From, From>
	{
		typedef const From& result_type;
		static result_type convert(result_type from){return from;}
	};

//---------------------------------------------------------------------------
/**
 * @brief   this macro defines converter class for converting numeric value to string, or converting string to numeric value
 * @param   type		 numeric value type
 * @param   namespace_ 	 namespace name for this type of conversion
 * @param   c_format 	 format string ro converting numeric value to string
 * @param   c_toval 	 conversion function for string to numeric value
 * @param   w_format 	 format string ro converting numeric value to wstring
 * @param   w_toval 	 conversion function for wstring to numeric value
 * @see     string_converter
 * 
 *   (example) converter for int
 * @code
	NUMERIC_STRING_CONVERTER(
						int, int_,	// any namespace is ok
						"%d", "0123456789-", atoi(str.c_str()),		// for string
						L"%d", L"0123456789-", _wtoi(str.c_str())	// for wstring
						);

 * @endcode
 */
//---------------------------------------------------------------------------
#define NUMERIC_STRING_CONVERTER(type, namespace_, \
								c_format, c_toval, w_format, w_toval) \
	template<typename> struct namespace_##char_traits \
	{ \
	}; \
	template<> struct namespace_##char_traits<char> \
	{ \
		static const char* format()							{ return c_format; } \
		static type 		to_val(const std::string& str)	{ return c_toval; } \
	}; \
	template<> struct namespace_##char_traits<wchar_t> \
	{ \
		static const wchar_t* format()						{ return w_format;} \
		static type		 to_val(const std::wstring& str)	{ return w_toval; } \
	};\
	template<typename StdStrType> \
	struct string_converter<type, StdStrType> : string_convertible\
	{ \
	typedef StdStrType std_str_type;\
	typedef type value_type; \
	typedef typename StdStrType::value_type char_type;\
	static std_str_type to_string(const value_type& val)	{ return format(val, namespace_##char_traits<char_type>::format()); }\
	static value_type get_value(const StdStrType& str)	{ return namespace_##char_traits<char_type>::to_val(str.c_str()); } \
	} \

	struct string_unconvertible
	{
		static const bool is_convertible = false;
	};

	struct string_convertible
	{
		static const bool is_convertible = true;
	};

	//---------------------------------------------------------------------------
	/**
	 * @brief converter class for string and numeric value
	 * @param T string or numeric value or user defined class
	 * @param StdStrType string class that must be std::string or std::wstring
	 *
	 *	this class must be specialized. Specialized classes for numeric value are defined as macro above.
	 */
	//---------------------------------------------------------------------------
	template<typename T, typename StdStrType>
	struct string_converter : string_unconvertible
	{
	};
	//---------------------------------------------------------------------------
	/**
	 * @brief converter specialized for conversion between argment string type and std::string type.
	 * @param String converted string type that must be std::string or std::wstring
	 *
	 *	to_string function provides argment type to std::string.\n
	 *  reduction function provides std::string to argment type.
	 */
	//---------------------------------------------------------------------------
	template<typename StdStrType>
	struct string_converter<std::string, StdStrType> : string_convertible
	{
		typedef std::string value_type, handle_type;
		typedef typename string_conversion_rules<value_type, StdStrType>::result_type std_str_type;
		typedef typename string_conversion_rules<StdStrType, value_type>::result_type reduction_type;
		static std_str_type to_string(const value_type& val){ return string_conversion_rules<value_type, StdStrType>::convert(val); };
		static reduction_type reduction(const StdStrType& string){ return string_converter<StdStrType, value_type>::to_string(string); }
	
	};
	//---------------------------------------------------------------------------
	/**
	 * @brief converter specialized for conversion between argment string type and std::wstring type.
	 * @param String converted string type that must be std::string or std::wstring
	 *
	 *	to_string function provides argment type to std::wstring.\n
	 *  reduction function provides std::wstring to argment type.
	 */
	//---------------------------------------------------------------------------
	template<typename StdStrType>
	struct string_converter<std::wstring, StdStrType> : string_convertible
	{
		typedef std::wstring value_type, handle_type;
		typedef typename string_conversion_rules<value_type, StdStrType>::result_type std_str_type;
		typedef typename string_conversion_rules<StdStrType, value_type>::result_type reduction_type;
		static std_str_type to_string(const value_type& val){ return string_conversion_rules<value_type, StdStrType>::convert(val); };
		static reduction_type reduction(const StdStrType& string){ return string_converter<StdStrType, value_type>::to_string(string); }
	};
	//---------------------------------------------------------------------------
	/**
	 * @brief converter specialized for conversion between argment string type and const wchar_t*.
	 * @param StdStrType a type which is converted to and which must be std::string or std::wstring
	 *
	 *	to_string function provides argment type to std::wstring.\n
	 *  reduction function provides std::wstring to argment type.
	 */
	//---------------------------------------------------------------------------
	template<typename StdStrType>
	struct string_converter<const wchar_t*, StdStrType> : string_convertible
	{
		typedef StdStrType std_str_type;
		typedef const wchar_t* value_type;
		typedef value_type reduction_type;
		typedef std::wstring handle_type;
		static std_str_type to_string(value_type const val)	{ return string_converter<handle_type, StdStrType>::to_string(val); };
		static reduction_type reduction(const StdStrType& string){ return string.c_str(); }
	};
	//---------------------------------------------------------------------------
	/**
	 * @brief converter specialized for conversion between argment string type and const char* type.
	 * @param StdStrType a type which is converted to and which must be std::string or std::wstring
	 *
	 *	to_string function provides argment type to std::string.\n
	 *  reduction function provides std::string to argment type.
	 */
	//---------------------------------------------------------------------------
	template<typename StdStrType>
	struct string_converter<const char*, StdStrType> : string_convertible
	{
		typedef StdStrType std_str_type;
		typedef const char* value_type;
		typedef value_type reduction_type;
		typedef std::string handle_type;
		static std_str_type to_string(value_type const val)		{ return string_converter<handle_type, StdStrType>::to_string(val); };
		static reduction_type reduction(const StdStrType& string){ return string.c_str(); }
	};
#ifdef _AFX
	template<typename StdStrType>
	struct string_converter<CStringA, StdStrType> : string_convertible
	{
		typedef StdStrType std_str_type;
		typedef CStringA value_type, reduction_type;
		typedef std::string handle_type;	
		static std_str_type to_string(value_type const& val)		{ return string_converter<handle_type, StdStrType>::to_string(val.GetString()); };
		static reduction_type reduction(const StdStrType& string)	{ return reduction_type( string_converter<StdStrType, handle_type>::to_string(string).c_str() ); }
	};
	template<typename StdStrType>
	struct string_converter<CStringW, StdStrType> : string_convertible
	{
		typedef StdStrType std_str_type;
		typedef CStringW value_type, reduction_type;
		typedef std::wstring handle_type;	
		static std_str_type to_string(value_type const& val)		{ return string_converter<handle_type, StdStrType>::to_string(val.GetString()); };
		static reduction_type reduction(const StdStrType& string)	{ return reduction_type( string_converter<StdStrType, handle_type>::to_string(string).c_str() ); }
	};
#endif

NUMERIC_STRING_CONVERTER(int, int_,
						"%d", atoi(str.c_str()),
						L"%d", _wtoi(str.c_str()));

NUMERIC_STRING_CONVERTER(long, long_,
						"%ld", atol(str.c_str()),
					    L"%ld", _wtol(str.c_str()));

NUMERIC_STRING_CONVERTER(unsigned int, uint_,
						"%u", strtoul(str.c_str(), NULL, 10),
					    L"%u", wcstoul(str.c_str(), NULL, 10));

NUMERIC_STRING_CONVERTER(short, short_,
						"%d", static_cast<short>( atoi(str.c_str()) ),
					    L"%d", static_cast<short>( _wtoi(str.c_str()) ));

NUMERIC_STRING_CONVERTER(unsigned short, ushort_,
						"%u", static_cast<unsigned short>( strtoul(str.c_str(), NULL, 10) ),
					    L"%u", static_cast<unsigned short>( wcstoul(str.c_str(), NULL, 10) ));

NUMERIC_STRING_CONVERTER(char, char_,
						"%d", static_cast<char> ( atoi(str.c_str()) ),
					    L"%d", static_cast<char> ( _wtoi(str.c_str())) );

NUMERIC_STRING_CONVERTER(unsigned char, ucahr_,
						"%u", static_cast<unsigned char> ( strtoul(str.c_str(), NULL, 10) ),
					    L"%u", static_cast<unsigned char> ( wcstoul(str.c_str(), NULL, 10) ) );

NUMERIC_STRING_CONVERTER(unsigned long, ulong_,
						"%lu", strtoul(str.c_str(), NULL, 10),
					    L"%lu", wcstoul(str.c_str(), NULL, 10));

NUMERIC_STRING_CONVERTER(long long, longlong_,
						"%lld", _atoi64(str.c_str()),
					    L"%lld", _wtoi64(str.c_str()));

NUMERIC_STRING_CONVERTER(unsigned long long, ulonglong_,
						"%llu", _strtoui64(str.c_str(), NULL, 10),
					    L"%llu", _wcstoui64(str.c_str(), NULL, 10));

NUMERIC_STRING_CONVERTER(double, double_,
						"%g", atof(str.c_str()),
					    L"%g", _wtof(str.c_str()));

NUMERIC_STRING_CONVERTER(float, float_,
						"%g", static_cast<float>(atof(str.c_str())),
					    L"%g", static_cast<float>(_wtof(str.c_str())));


#undef NUMERIC_STRING_CONVERTER

	/** a string type tag to be added to type_traits */
	struct string_type{};

	/** a numeric type tag to be added to type_traits */
	struct numeric_type{};

	//---------------------------------------------------------------------------
	/**
	 * @brief type traits for string converter
	 *
	 * In the case of string, it must be specialized because imformation held is different from other types.
	 * (mainly for defining a rule fo conversion of string type to other string type.)
	 */
	//---------------------------------------------------------------------------
	template<typename T>
	struct type_traits
	{
		typedef T original_type, tagged_type;
		typedef numeric_type type_info;
	};

	template<>
	struct type_traits<const char*>
	{
		typedef const char* original_type;
		typedef original_type tagged_type;
		typedef string_type type_info;
		typedef string_converter<tagged_type, std::string> std_str_converter;
	};

	template<>
	struct type_traits<const wchar_t*>
	{
		typedef const wchar_t* original_type;
		typedef original_type tagged_type;
		typedef string_type type_info;
		typedef string_converter<tagged_type, std::wstring> std_str_converter;
	};

	template<size_t t>
	struct type_traits<const char[t]>
	{
		typedef const char* original_type;
		typedef original_type tagged_type;
		typedef string_type type_info;
		typedef string_converter<tagged_type, std::string> std_str_converter;
	};

	template<size_t t>
	struct type_traits<const wchar_t[t]>
	{
		typedef const wchar_t* original_type;
		typedef original_type tagged_type;
		typedef string_type type_info;
		typedef string_converter<tagged_type, std::wstring> std_str_converter;
	};

	template<>
	struct type_traits<std::string>
	{
		typedef std::string original_type, tagged_type;
		typedef string_type type_info;
		typedef string_converter<tagged_type, tagged_type> std_str_converter;
	};

	template<>
	struct type_traits<std::wstring>
	{
		typedef std::wstring original_type, tagged_type;
		typedef string_type type_info;
		typedef string_converter<tagged_type, tagged_type> std_str_converter;
	};

#ifdef _AFX

	template<>
	struct type_traits<CStringA>
	{
		typedef CStringA original_type, tagged_type;
		typedef string_type type_info;
		typedef string_converter<tagged_type, std::string> std_str_converter;
	};

	template<>
	struct type_traits<CStringW>
	{
		typedef CStringW original_type, tagged_type;
		typedef string_type type_info;
		typedef string_converter<tagged_type, std::wstring> std_str_converter;
	};

#endif

	// "To" and "From" type are different type and "From" instance is cast to the string type of "To" .
	template<
		typename To,
		typename From,
		typename TypeInfoTo,
		typename TypeInfoFrom

	>
	struct cast_executor
	{
		typedef typename type_traits<To>::std_str_converter std_str_converter;
		typedef To return_type;
		typedef typename type_traits<From>::tagged_type tagged_type;
		typedef typename string_converter<tagged_type, typename std_str_converter::handle_type> string_converter;
		static return_type cast(const From& from)
		{
			static_assert(string_converter::is_convertible,"string conversion is not supported. check To and From type.");

			return std_str_converter::reduction( string_converter::to_string( from ) );
		}
	};

	// "To" is numeric type and "From" is string type("From" instance is casted to string type)
	template<
		typename To,
		typename From
	>
	struct cast_executor<To, From, numeric_type, string_type>
	{
		typedef typename type_traits<From>::std_str_converter std_str_converter;
		typedef typename type_traits<To>::tagged_type tagged_type;
		typedef typename string_converter<tagged_type, typename std_str_converter::handle_type> string_converter;
		typedef typename string_converter::value_type return_type;
		static return_type cast(const From& from)
		{
			return string_converter::get_value( std_str_converter::to_string( from ) );
		}
	};

	// "To" and "From" are differens type and neither type is string type. 
	template<
		typename To,
		typename From
	>
	struct cast_executor<To, From, numeric_type, numeric_type>
	{
		typedef typename To return_type;
		static return_type cast(const From& from)
		{
			return static_cast<To>(from);
		}
	};

	// "To" and "From" are same type, so never converted.
	template<
		typename To,
		typename TypeInfoTo,
		typename TypeInfoFrom
	>
	struct cast_executor<To, To, TypeInfoTo, TypeInfoFrom>
	{
		typedef typename const To& return_type;
		static return_type cast(const To& from)
		{
			return from;
		}
	};

	template<
		typename To, 
		typename From,
		typename TypeinfoTo =  typename type_traits<To>::type_info,
		typename TypeinfoFrom =  typename type_traits<From>::type_info,
		typename CasterType = cast_executor<To, From, TypeinfoTo, TypeinfoFrom>,
		typename ReturnType = typename CasterType::return_type
	>
	struct intelligent_cast_helper
	{
		typedef ReturnType return_type;
		static return_type cast(const From& from)
		{
			return CasterType::cast(from);
		}
	};

} // End Of Namespace intelligent_cast_detail 

//---------------------------------------------------------------------------
/*!
 * @brief   intelligent cast
 * @param[in] from a type of argment
 * @returns instance to be converted
 * @attention converting of std::string to const wchar_t* must not use,
 *            please use std::string to std::wstring conversion insteady.
 */
//---------------------------------------------------------------------------
template<typename To, typename From>
typename intelligent_cast_detail::intelligent_cast_helper<To, From>::return_type
intelligent_cast(const From& from)
{
	using namespace intelligent_cast_detail;
	return intelligent_cast_helper<To, From>::cast(from);
}

