/* <libcsd/bytes.h>
   Copyright (c) 2023 bellrise */

#pragma once

#include <libcsd/str.h>

/**
 * @class bytes
 * Continguous array of read/write bytes. By default, an array is allocated on
 * the heap, but you may provide a custom buffer with a size, but then the bytes
 * type will no longer be able to resize the buffer.
 */
struct bytes
{
	using byte = unsigned char;

	bytes();
	bytes(bytes&& moved_bytes);
	~bytes();

	/**
	 * @method copy constructor
	 * The implicit copy constructor is deleted, as it may result in some
	 * unexpected behaviour. Use ::copy() instead, to explicitly create a
	 * copy.
	 */
	bytes(const bytes& copied_bytes) = delete;

	int size() const;
	str to_str() const;
	bytes copy() const;

	void alloc(int nbytes);
	void use_static_buffer(byte *raw_ptr, int nbytes);
	void copy_from(const byte *raw_ptr, int nbytes);
	void copy_to(byte *raw_ptr, int nbytes) const;
	void zero();

	byte *raw_ptr() const;

	template <typename T>
	void copy_from(const T *any_ptr, int nbytes)
	{
		copy_from((const byte *) any_ptr, nbytes);
	}

	bytes& operator=(const bytes& other);

    private:
	byte *m_ptr;
	int m_size;
	bool m_user_provided;
};
