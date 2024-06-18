/* <libcsd/src/stream.cc>
   Copyright (c) 2024 bellrise */

#include <libcsd/stream.h>

namespace csd {

stream::stream()
	: m_read_single()
	, m_write_single()
	, m_read()
	, m_write()
	, m_is_open()
	, m_byte_buffer()
{
	m_byte_buffer.use_static_buffer(
		reinterpret_cast<byte *>(&m_byte_buffer_space), 1);
	m_read_single = nullptr;
}

stream::stream(const stream& other)
	: m_read_single(other.m_read_single)
	, m_write_single(other.m_write_single)
	, m_read(other.m_read)
	, m_write(other.m_write)
	, m_is_open(other.m_is_open)
	, m_byte_buffer()
{
	m_byte_buffer.use_static_buffer(
		reinterpret_cast<byte *>(&m_byte_buffer_space), 1);
}

stream::stream(stream&& moved)
	: m_read_single(csd::move(moved.m_read_single))
	, m_write_single(csd::move(moved.m_write_single))
	, m_read(csd::move(moved.m_read))
	, m_write(csd::move(moved.m_write))
	, m_is_open(csd::move(moved.m_is_open))
	, m_byte_buffer()
{
	moved.m_read_single = nullptr;
	moved.m_write_single = nullptr;
	moved.m_read = nullptr;
	moved.m_write = nullptr;
	moved.m_is_open = nullptr;

	m_byte_buffer.use_static_buffer(
		reinterpret_cast<byte *>(&m_byte_buffer_space), 1);
}

bool stream::is_writable()
{
	return is_open() && (m_write_single.has_routine() || m_write.has_routine());
}

bool stream::is_readable()
{
	return is_open() && (m_read_single.has_routine() || m_read.has_routine());
}

bool stream::is_open()
{
	return m_is_open.has_routine() ? m_is_open() : false;
}

char stream::read_char()
{
	return read_byte();
}

bytes::byte stream::read_byte()
{
	if (!is_readable())
		throw csd::stream_exception("stream is not readable");

	if (m_read_single.has_routine())
		return m_read_single();

	return m_read(1)[0];
}

void stream::write_char(char c)
{
	write_byte(c);
}

void stream::write_byte(bytes::byte b)
{
	if (!is_writable())
		throw csd::stream_exception("stream is not writable");

	if (m_write_single.has_routine())
		return m_write_single(b);

	m_byte_buffer[0] = b;
	m_write(m_byte_buffer);
}

void stream::set_read_single(read_single_function_t f)
{
	m_read_single = f;
}

void stream::set_write_single(write_single_function_t f)
{
	m_write_single = f;
}

void stream::set_read_batch(read_function_t f)
{
	m_read = f;
}

void stream::set_write_batch(write_function_t f)
{
	m_write = f;
}

void stream::set_is_open(is_open_function_t f)
{
	m_is_open = f;
}

stream& stream::operator=(const stream& other)
{
	m_read_single = other.m_read_single;
	m_write_single = other.m_write_single;
	m_read = other.m_read;
	m_write = other.m_write;
	m_is_open = other.m_is_open;
	return *this;
}

} // namespace csd
