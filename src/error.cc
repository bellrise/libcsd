/* libcsd/src/error.cc
   Copyright (c) 2022-2023 bellrise */

#include <libcsd/error.h>
#include <libcsd/format.h>

namespace csd {

index_exception::index_exception(int index)
	: m_has_bounds(false)
	, m_str_index()
	, m_index(index)
{ }

index_exception::index_exception(const str& str_index)
	: m_has_bounds(false)
{
	m_str_index = csd::format("`{}`", str_index);
}

index_exception::index_exception(int index, int min, int max)
	: m_has_bounds(true)
	, m_index(index)
	, m_min(min)
	, m_max(max)
{ }

str index_exception::message() const
{
	str err = "index_exception: index ";

	err += m_str_index.empty() ? m_index : m_str_index;
	err += " is out of bounds";

	if (m_has_bounds)
		err += csd::format(" for a range of [{}, {}]", m_min, m_max);

	return err;
}

str unpack_exception::message() const
{
	str err = "unpack_exception: tried to unpack the value from "
			  "maybe<T>, but no value was present";
	return err;
}

nullptr_exception::nullptr_exception()
	: m_message("nullptr_exception: expected value, found null pointer")
{ }

nullptr_exception::nullptr_exception(const str& message)
	: m_message("nullptr_exception: ")
{
	m_message += message;
}

str nullptr_exception::message() const
{
	return m_message;
}

memory_exception::memory_exception(const str& message)
	: m_message(message)
{ }

str memory_exception::message() const
{
	return str("memory_exception: ") + m_message;
}

invalid_operation_exception::invalid_operation_exception(const str& message)
	: m_message(message)
{ }

str invalid_operation_exception::message() const
{
	return str("invalid_operation_exception: ") + m_message;
}

invalid_argument_exception::invalid_argument_exception(const str& message)
	: m_message(message)
{ }

str invalid_argument_exception::message() const
{
	return str("invalid_argument_exception: ") + m_message;
}

stream_exception::stream_exception(const str& message)
	: m_message(message)
{ }

str stream_exception::message() const
{
	return str("stream_exception: ") + m_message;
}

} // namespace csd
