/* <libcsd/print.h>
   Copyright (c) 2022 bellrise */

#pragma once
#include <libcsd/str.h>

/* Print functions. */

void print_single(const void *val);
void print_single(const char *val);
void print_single(const str& val);
void print_single(const char val);
void print_single(const unsigned char val);
void print_single(const short val);
void print_single(const unsigned short val);
void print_single(const int val);
void print_single(const unsigned int val);
void print_single(const long val);
void print_single(const unsigned long val);
void print_single(const float val);
void print_single(const double val);
void print_single(const bool val);

template <typename T>
void print(const T* value)
{
	print_single(value);
}

template <csd::ImplementsToString T>
void print(const T& value)
{
	print_single(value.to_str());
}

/* Multiple-argument print() functions. */

template <typename T, typename... VT>
void print(const T& value, const VT&... values)
{
	print_single(value);

	if constexpr (sizeof...(values) != 0) {
		print_single(' ');
		print(values...);
	}
}

template <typename T, typename... VT>
void println(const T& value, const VT&... values)
{
	print_single(value);

	if constexpr (sizeof...(values) != 0) {
		print_single(' ');
		print(values...);
	}

	print_single('\n');
}
