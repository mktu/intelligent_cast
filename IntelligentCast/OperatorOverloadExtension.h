#pragma once
#include<type_traits>
#include "IntelligentCast.h"


namespace intelligent_cast_detail
{
	template<typename T>
	struct is_string
	{
		static const bool value = false;
	};
	template<>
	struct is_string<std::string>
	{
		static const bool value = true;
	};
	template<>
	struct is_string<std::wstring>
	{
		static const bool value = true;
	};

	template<
		typename T, 
		typename U
	>
	class is_string_castable
	{
		template<typename T, typename U>
		static auto check(typename string_converter<T,U>::value_type*) -> std::true_type;
		template<typename T, typename U>
		static auto check(...) -> std::false_type;

		typedef decltype(check<T,U>(nullptr)) result_type;
	public:
		static const bool value = result_type::value && is_string<T>::value;
	};

	template<
		typename T, 
		typename U
	>
	class is_addable
	{
		template<typename T, typename U>
		static auto check(T*, U*) -> decltype
			(
				std::declval<T>(0) + std::declval<U>(0),
				std::true_type()
			);
		template<typename T, typename U>
		static auto check(...) -> std::false_type;
			
		typedef decltype(check<T,U>(nullptr,nullptr)) result_type;
	public:
		static const bool value = result_type::value;

	};

	template<typename StrType, typename T>
	struct can_allow_adding_operator
	{
		typedef typename type_traits<T>::original_type original_type;
		static const bool value = ! is_addable<StrType, original_type>::value && 
			is_string_castable<StrType, original_type>::value;
	};
}

template<typename StrType, typename T>
inline typename std::enable_if< 
	intelligent_cast_detail::can_allow_adding_operator<StrType,T>::value
	, StrType>::type operator+(const StrType& str, const T& t)
{
	return str+intelligent_cast<StrType>(t);
}