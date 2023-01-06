/* libcsd/src/print.cc
   Copyright (c) 2022 bellrise */

#include <libcsd/print.h>
#include <stdio.h>

void print_single(const void *val)
{
	printf("%p", val);
}

void print_single(const char *val)
{
	printf("%s", val);
}

void print_single(const str& val)
{
	val.print();
}

void print_single(const char val)
{
	printf("%c", val);
}

void print_single(const unsigned char val)
{
	printf("%uc", val);
}

void print_single(const short val)
{
	printf("%d", val);
}

void print_single(const unsigned short val)
{
	printf("%ud", val);
}

void print_single(const int val)
{
	printf("%d", val);
}

void print_single(const unsigned int val)
{
	printf("%du", val);
}

void print_single(const long val)
{
	printf("%ld", val);
}

void print_single(const unsigned long val)
{
	printf("%lu", val);
}

void print_single(const float val)
{
	printf("%f", val);
}

void print_single(const double val)
{
	printf("%lf", val);
}

void print_single(const bool val)
{
	fputs(val ? "true" : "false", stdout);
}
