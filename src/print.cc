/* libmint/src/print.cc
   Copyright (c) 2022 bellrise */

#include <mint/print.h>
#include <stdio.h>


void print(const void *val)
{
	printf("%p", val);
}

void print(const char *val)
{
	printf("%s", val);
}

void print(const str &val)
{
	val.print();
}

void print(const char val)
{
	printf("%c", val);
}

void print(const unsigned char val)
{
	printf("%uc", val);
}

void print(const short val)
{
	printf("%d", val);
}

void print(const unsigned short val)
{
	printf("%ud", val);
}

void print(const int val)
{
	printf("%d", val);
}

void print(const unsigned int val)
{
	printf("%du", val);
}

void print(const long val)
{
	printf("%ld", val);
}

void print(const unsigned long val)
{
	printf("%lu", val);
}

void print(const float val)
{
	printf("%f", val);
}

void print(const double val)
{
	printf("%lf", val);
}

void print(const bool val)
{
	fputs(val ? "true" : "false", stdout);
}
