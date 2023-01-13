/* libcsd/src/mem_type.cc
   Copyright (c) 2023 bellrise */

#include <libcsd/detail.h>

namespace csd {

mem_type::mem_type(int flags)
    : type(flags)
{}

bool mem_type::is_static()
{
	return type & flags::STATIC;
}

bool mem_type::is_heap()
{
	return type & flags::HEAP;
}

bool mem_type::is_stack()
{
	return type & flags::STACK;
}

bool mem_type::is_readable()
{
	return type & flags::READ;
}

bool mem_type::is_writable()
{
	return type & flags::WRITE;
}

} // namespace csd
