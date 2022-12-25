/* <mint/print.h>
   Copyright (c) 2022 bellrise */

#pragma once
#include <mint/str.h>
#include <mint/box.h>

/* Print functions. */

void print(const void *val);
void print(const char *val);
void print(const str &val);
void print(const char val);
void print(const unsigned char val);
void print(const short val);
void print(const unsigned short val);
void print(const int val);
void print(const unsigned int val);
void print(const long val);
void print(const unsigned long val);
void print(const float val);
void print(const double val);

template <typename T>
void print(const box<T> &ref)
{
	print(*ref);
}

template <StringConvertible T>
void print(const T &value)
{
	print(value.to_str());
}

/* Multiple-argument print() functions. */

template <typename... T>
void print(const T&... values)
{
	((print(values), print(' ')), ...);
}

template <typename... T>
void println(const T&... values)
{
	((print(values), print(' ')), ...);
	print('\n');
}
