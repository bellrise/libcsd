/* <libcsd/stream.h>
   Copyright (c) 2024 bellrise */

#pragma once

#include <libcsd/routine.h>

/**
 * @class stream
 * Represents a read/write object in a stream form, meaning you cannot
 * seek through it, just read or write. The {file} type can be turned
 * into a stream, and passed to functions that accept streams. This is
 * a handy abstraction to implement protocols - the underlying file
 * or network device is unimportant - as long as it implements .stream().
 *
 * The requirements for a working stream are simple: set your allowed
 * methods with .set_read_single, .set_write_single, .set_read_batch
 * and .set_write_batch. If a method is not set, {stream} will treat
 * it as not readable/writable.
 *
 * For example, calling only set_read_single with the given function will
 * only allow for single-byte reading from the underlying file/device etc.
 *
 * To make a solid stream, implement is_open. If this function is not set,
 * .is_open() will always return true as the stream has no idea if the
 * underlying file/device etc. is still open.
 */
struct stream
{
	using byte = bytes::byte;
	using read_single_function_t = routine<byte()>;
	using write_single_function_t = routine<void(byte)>;
	using read_function_t = routine<bytes(int)>;
	using write_function_t = routine<int(const bytes&)>;
	using is_open_function_t = routine<bool()>;

	stream();
	stream(const stream&);
	stream(stream&&);

	bool is_writable();
	bool is_readable();
	bool is_open();

	char read_char();
	byte read_byte();

	void write_char(char);
	void write_byte(byte);

	void set_read_single(read_single_function_t f);
	void set_write_single(write_single_function_t f);
	void set_read_batch(read_function_t f);
	void set_write_batch(write_function_t f);
	void set_is_open(is_open_function_t f);

	stream& operator=(const stream&);

  private:
	read_single_function_t m_read_single;
	write_single_function_t m_write_single;
	read_function_t m_read;
	write_function_t m_write;
	is_open_function_t m_is_open;
	bytes m_byte_buffer;
	char m_byte_buffer_space;
};
