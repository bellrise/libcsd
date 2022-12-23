/* libmint/src/str.cc
   Copyright (c) 2022 bellrise */

#include <mint/str.h>
#include <string.h>
#include <stdio.h>
#include <new>

str::str()
	: m_ptr(nullptr)
	, m_space(0)
	, m_len(0)
{
}

str::str(const str &other)
	: m_ptr(nullptr)
	, m_space(0)
	, m_len(0)
{
	copy_from(other);
}

str::str(str &&moved)
	: m_ptr(moved.m_ptr)
	, m_space(moved.m_space)
	, m_len(moved.m_len)
{
	moved.m_ptr = nullptr;
	moved.m_space = 0;
	moved.m_len = 0;
}

str::str(const char *string)
	: m_ptr(nullptr)
	, m_space(0)
	, m_len(0)
{
	if (string == nullptr)
		return;

	m_len = strlen(string);
	m_space = m_len + 1;
	m_ptr = new char[m_space];
	memcpy(m_ptr, string, m_len);
	m_ptr[m_space-1] = 0;
}

str::str(const char *string, size_t maxlen)
	: m_ptr(nullptr)
	, m_space(0)
	, m_len(0)
{
	copy_from_raw(string, maxlen);
}

str::~str()
{
	delete [] m_ptr;
}

size_t str::len() const
{
	return m_len;
}

str str::to_str() const
{
	return str(*this);
}

void str::print() const
{
	if (len() == 0)
		return;
	printf("%.*s", (int) len(), m_ptr);
}

str &str::replace(char from, char to)
{
	for (size_t i = 0; i < m_len; i++) {
		if (m_ptr[i] == from)
			m_ptr[i] = to;
	}

	return *this;
}

str &str::append(const str &next)
{
	size_t old_len = len();
	size_t bytes_to_copy = next.len();
	size_t required_bytes = len() + next.len() + 1;

	if (resize(required_bytes) < required_bytes) {
		bytes_to_copy = next.len() - (m_space - len());
		m_len = len() + bytes_to_copy;
	} else {
		m_len = len() + next.len();
	}

	memmove(&m_ptr[old_len], next.m_ptr, bytes_to_copy);
	m_ptr[m_space-1] = 0;

	return *this;
}

str &str::append(const char *next)
{
	size_t old_len = len();
	size_t next_len = strlen(next);
	size_t bytes_to_copy = next_len;
	size_t required_bytes = len() + next_len + 1;

	if (resize(required_bytes) < required_bytes) {
		bytes_to_copy = next_len - (m_space - len());
		m_len = len() + bytes_to_copy;
	} else {
		m_len = len() + next_len;
	}

	memmove(&m_ptr[old_len], next, bytes_to_copy);
	m_ptr[m_space-1] = 0;

	return *this;
}

void str::copy_from(const str &other)
{
	size_t required_bytes = other.len() + 1;

	if (resize(required_bytes) < required_bytes)
		memmove(m_ptr, other.m_ptr, m_space - 1);
	else
		memmove(m_ptr, other.m_ptr, other.len());

	m_len = other.len();
	m_ptr[m_space-1] = 0;
}

void str::copy_from_raw(const char *other, size_t other_len)
{
	size_t required_bytes = other_len + 1;

	if (resize(required_bytes) < required_bytes)
		memmove(m_ptr, other, m_space - 1);
	else
		memmove(m_ptr, other, other_len);

	m_len = other_len;
	m_ptr[m_space-1] = 0;
}

size_t str::resize(size_t nbytes)
{
	/* Our implementation of resize() only resizes up, without any downsizes.
	   This means that if we request a small size of bytes, we keep the old
	   buffer, and alloc a new one only if we don't have enough space. */

	if (m_space >= nbytes)
		return m_space;

	char *old_ptr = m_ptr;

	try {
		m_ptr = new char[nbytes+1];
	} catch (std::bad_alloc &v) {
		return m_space;
	}

	/* Copy the memory from the old buffer to the new one. */
	memmove(m_ptr, old_ptr, m_space);
	m_space = nbytes;

	delete [] old_ptr;
	return m_space;
}

str &str::operator=(const str &other)
{
	copy_from(other);
	return *this;
}

str &str::operator=(const char *other)
{
	copy_from_raw(other, strlen(other));
	return *this;
}

str &str::operator+(const str &next)
{
	append(next);
	return *this;
}

str &str::operator+(const char *next)
{
	append(next);
	return *this;
}

str &str::operator+=(const str &next)
{
	append(next);
	return *this;
}

str &str::operator+=(const char *next)
{
	append(next);
	return *this;
}
