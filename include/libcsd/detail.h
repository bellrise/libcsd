/* <libcsd/detail.h>
	Copyright (c) 2022-2023 bellrise */

#pragma once

namespace csd {

struct true_result
{
	static constexpr bool result = true;
};

struct false_result
{
	static constexpr bool result = false;
};

template <typename T>
struct base_type_s
{
	using type = T;
};

template <typename T>
struct base_type_s<T&>
{
	using type = T;
};

template <typename T>
struct base_type_s<T&&>
{
	using type = T;
};

template <typename T>
struct base_type_s<T *>
{
	using type = T;
};

template <typename T>
struct base_type_s<T **>
{
	using type = T;
};

/**
 * @type base_type<T>
 * Returns the base type of T, removing any pointer and reference.
 * base_type<int&&> will return int.
 */
template <typename T>
using base_type = typename base_type_s<T>::type;

template <typename T>
struct remove_const_s
{
	using type = T;
};

template <typename T>
struct remove_const_s<const T>
{
	using type = T;
};

/**
 * @type base_type<T>
 * Returns the base type of T, removing any const.
 * remove_const<const int> will return int.
 */
template <typename T>
using remove_const = typename remove_const_s<T>::type;

template <typename T, typename U>
struct same_type_s : false_result
{ };

template <typename T>
struct same_type_s<T, T> : true_result
{ };

/**
 * @var same_type<T, U>
 * Compares two types, and evaluates to `true` if both are of the same type.
 * Otherwise, evaluates to `false`. Because this is constexpr, it will inline
 * the value into your code without any external symbols.
 */
template <typename T, typename U>
constexpr static bool same_type = same_type_s<T, U>::result;

/**
 * @concept IsMovable<T>
 * Any type that can be moved.
 */
template <typename T>
concept IsMovable = requires(base_type<T> a, base_type<T> v) {
						a = static_cast<base_type<T>&&>(v);
					};

/**
 * @concept IsComparable<T, U>
 * Allows for any two types that can be compared to each other using the
 * equality operator (==).
 */
template <typename T, typename U>
concept IsComparable = requires(base_type<T> a, base_type<U> b) {
						   a == b;
						   b == a;
						   a != b;
						   b != a;
					   };

template <typename T>
constexpr inline base_type<T>&& move(T&& thing)
{
	return static_cast<base_type<T>&&>(thing);
}

/**
 * @concept
 * Any F that is callable or implements the () operator, and takes the correct
 * amount & types of arguments.
 */
template <typename F, typename... Args>
concept Callable = requires(F f, Args... args) { f(args...); };

} // namespace csd
