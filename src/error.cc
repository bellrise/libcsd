/* libcsd/src/error.cc
   Copyright (c) 2022 bellrise */

#include <libcsd/error.h>

index_exception::index_exception(int index)
	: m_has_bounds(false), m_index(index)
{ }

index_exception::index_exception(int index, int min, int max)
	: m_has_bounds(true), m_index(index), m_min(min), m_max(max)
{ }

str index_exception::message() const
{
	str err = "index_exception: index ";

	err += str(m_index) + " is out of bounds";
	if (m_has_bounds) {
		err.append(" for a range of [").append(m_min).append(", ")
			.append(m_max).append("]");
	}

	return err;
}

str unpack_exception::message() const
{
	str err = "unpack_exception: tried to unpack the value from ";
	err += "maybe<T>, but no value was present";
	return err;
}
