/* <libcsd/detail.h>
    Copyright (c) 2022 bellrise */

#pragma once

namespace csd {

/* Utilities for the csd library */

/* Similar to std::remove_reference, these trait structs just strip the
   references off the type. Note that you should be using base_type<T>,
   and not base_type_s<T>::type. */

template <typename T>
struct base_type_s
{ using type = T; };

template <typename T>
struct base_type_s<T&>
{ using type = T; };

template <typename T>
struct base_type_s<T&&>
{ using type = T; };

template <typename T>
using base_type = typename base_type_s<T>::type;


template <typename T, typename U>
struct same_type_s
{ static constexpr bool result = false; };

template <typename T>
struct same_type_s<T, T>
{ static constexpr bool result = true; };

template <typename T, typename U>
constexpr static bool same_type = same_type_s<T, U>::result;

template <typename T>
concept IsMovable = requires (base_type<T> a, base_type<T> v)
{
	a = static_cast<base_type<T>&&>(v);
};

template <typename T, typename U>
concept IsComparable = requires (base_type<T> a, base_type<U> b)
{
	a == b;
};

}
