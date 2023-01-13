/* <libcsd/bytes.h>
   Copyright (c) 2023 bellrise */

#pragma once

#include <libcsd/detail.h>

/**
 * @class bytes
 * Represents a contiguous array of raw read/write accesible bytes.
 */
struct bytes
{
	using byte = unsigned char;

	bytes();
	bytes(bytes&& moved_bytes);
	~bytes();

	/* A byte array cannot be implicitly copied, as it may result in some
	   unexpected behaviour. Use ::copy() instead, to explicitly create a
	   copy. */
	bytes(const bytes& copied_bytes) = delete;

	int size() const;

	void alloc(int nbytes);
	void use_static_buffer(byte *raw_ptr, int nbytes);

	void zero();

	/**
	 * @method copy
	 * Copies the buffer into a new allocated space.
	 */
	bytes copy() const;

	void copy_from(const byte *raw_ptr, int nbytes);
	void copy_to(byte *raw_ptr, int nbytes) const;

	byte *unsafe_ptr();

    private:
	byte *m_ptr;
	int m_size;
	bool m_user_provided;
};
