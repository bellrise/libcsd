/* libcsd/src/str.cc
   Copyright (c) 2022-2023 bellrise */

#include <ctype.h>
#include <libcsd/bytes.h>
#include <libcsd/error.h>
#include <new>
#include <stdio.h>
#include <string.h>

str::str()
	: m_ptr(nullptr)
	, m_space(0)
	, m_len(0)
{ }

str::str(const str& other)
	: m_ptr(nullptr)
	, m_space(0)
	, m_len(0)
{
	copy_from(other);
}

str::str(str&& moved)
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
	m_ptr[m_space - 1] = 0;
}

str::str(const char *string, int maxlen)
	: m_ptr(nullptr)
	, m_space(0)
	, m_len(0)
{
	copy_from_raw(string, maxlen);
}

str::str(void *pointer)
	: m_ptr(nullptr)
	, m_space(0)
	, m_len(0)
{
	char buf[16];
	memset(buf, 0, 16);

	snprintf(buf, 16, "%p", pointer);
	copy_from_raw(buf, strlen(buf));
}

str::str(size_t number)
	: m_ptr(nullptr)
	, m_space(0)
	, m_len(0)
{
	char buf[16];
	memset(buf, 0, 16);

	snprintf(buf, 16, "%zu", number);
	copy_from_raw(buf, strlen(buf));
}

str::str(float number)
	: m_ptr(nullptr)
	, m_space(0)
	, m_len(0)
{
	char buf[16];
	memset(buf, 0, 16);

	snprintf(buf, 16, "%f", number);
	copy_from_raw(buf, strlen(buf));
}

str::str(int number)
	: m_ptr(nullptr)
	, m_space(0)
	, m_len(0)
{
	char buf[16];
	memset(buf, 0, 16);

	snprintf(buf, 16, "%d", number);
	copy_from_raw(buf, strlen(buf));
}

str::str(char c)
	: m_ptr(nullptr)
	, m_space(0)
	, m_len(0)
{
	copy_from_raw(&c, 1);
}

str::~str()
{
	delete[] m_ptr;
}

int str::len() const
{
	return m_len;
}

str str::to_str() const
{
	return str(*this);
}

bytes str::to_bytes() const
{
	bytes buf;

	buf.alloc(m_len + 1);
	buf.zero();
	buf.copy_from(m_ptr, m_len);

	return buf;
}

void str::print() const
{
	if (len() == 0)
		return;
	printf("%.*s", (int) len(), m_ptr);
}

bool str::empty() const
{
	return len() == 0;
}

bool str::contains(const str& substr) const
{
	return find(substr) != invalid_index;
}

bool str::begins_with(const str& other) const
{
	if (len() < other.len())
		return false;
	return other == substr(0, other.len());
}

bool str::ends_with(const str& other) const
{
	if (len() < other.len())
		return false;
	return other == substr(len() - other.len());
}

str str::copy() const
{
	str s;
	s.copy_from(*this);
	return s;
}

int str::find(const str& substr) const
{
	if (len() < substr.len() || substr.len() == 0)
		return invalid_index;

	for (int i = 0; i <= len() - substr.len(); i++) {
		if (!strncmp(m_ptr + i, substr.m_ptr, substr.len()))
			return i;
	}

	return invalid_index;
}

str str::substr(int start_index, int length) const
{
	if (start_index >= len())
		return str();

	/* Clamping the length value for our use case. */
	if (length + start_index >= len())
		length = str::invalid_index;
	if (length == str::invalid_index)
		length = len() - start_index;
	if (length == 0)
		return str();

	return str(m_ptr + start_index, length);
}

str& str::lstrip()
{
	int i = 0;
	while (i < m_len - 1) {
		if (!isspace(m_ptr[i]))
			break;
		++i;
	}

	*this = substr(i);
	return *this;
}

str& str::rstrip()
{
	int i = m_len;
	while (i > 0) {
		if (!isspace(m_ptr[i]))
			break;
		--i;
	}

	*this = substr(0, i + 1);
	return *this;
}

str& str::strip()
{
	lstrip();
	rstrip();
	return *this;
}

const csd::str_view str::view()
{
	return {m_ptr, m_len};
}

str& str::replace(char from, char to)
{
	for (int i = 0; i < m_len; i++) {
		if (m_ptr[i] == from)
			m_ptr[i] = to;
	}

	return *this;
}

str& str::replace(const str& substr, const str& replacement)
{
	str new_str;
	int index;

	if ((index = find(substr)) == invalid_index)
		return *this;

	*this = this->substr(0, index) + replacement
		  + this->substr(index + substr.len());
	return *this;
}

str& str::append(const str& next)
{
	int old_len = len();
	int bytes_to_copy = next.len();
	int required_bytes = len() + next.len() + 1;

	if (resize(required_bytes) < required_bytes) {
		bytes_to_copy = next.len() - (m_space - len());
		m_len = len() + bytes_to_copy;
	} else {
		m_len = len() + next.len();
	}

	memmove(&m_ptr[old_len], next.m_ptr, bytes_to_copy);
	m_ptr[m_space - 1] = 0;

	return *this;
}

str& str::append(const char *next)
{
	int old_len = len();
	int next_len = strlen(next);
	int bytes_to_copy = next_len;
	int required_bytes = len() + next_len + 1;

	if (resize(required_bytes) < required_bytes) {
		bytes_to_copy = next_len - (m_space - len());
		m_len = len() + bytes_to_copy;
	} else {
		m_len = len() + next_len;
	}

	memmove(&m_ptr[old_len], next, bytes_to_copy);
	m_ptr[m_space - 1] = 0;

	return *this;
}

void str::copy_from(const str& other)
{
	int required_bytes = other.len() + 1;

	if (resize(required_bytes) < required_bytes)
		memmove(m_ptr, other.m_ptr, m_space - 1);
	else
		memmove(m_ptr, other.m_ptr, other.len());

	m_len = other.len();
	m_ptr[m_space - 1] = 0;
}

void str::copy_from_raw(const char *other, int other_len)
{
	int required_bytes = other_len + 1;

	if (resize(required_bytes) < required_bytes)
		memmove(m_ptr, other, m_space - 1);
	else
		memmove(m_ptr, other, other_len);

	m_len = other_len;
	m_ptr[m_space - 1] = 0;
}

int str::resolve_index(int index) const
{
	if (index < 0)
		index = len() + index;
	if (index < 0 || index >= len())
		throw csd::index_exception(index, 0, len() - 1);
	return index;
}

int str::resize(int nbytes)
{
	/* Our implementation of resize() only resizes up. */
	if (m_space >= nbytes)
		return m_space;

	char *old_ptr = m_ptr;

	try {
		m_ptr = new char[nbytes + 1];
	} catch (std::bad_alloc& v) {
		return m_space;
	}

	memmove(m_ptr, old_ptr, m_space);
	m_space = nbytes;

	delete[] old_ptr;
	return m_space;
}

str& str::operator=(const str& other)
{
	copy_from(other);
	return *this;
}

str& str::operator=(const char *other)
{
	copy_from_raw(other, strlen(other));
	return *this;
}

str& str::operator+(const str& next)
{
	append(next);
	return *this;
}

str& str::operator+(const char *next)
{
	append(next);
	return *this;
}

str& str::operator+=(const str& next)
{
	append(next);
	return *this;
}

str& str::operator+=(const char *next)
{
	append(next);
	return *this;
}

bool str::operator==(const str& other) const
{
	return len() == other.len() && !strncmp(m_ptr, other.m_ptr, len());
}

char& str::operator[](int index)
{
	return m_ptr[resolve_index(index)];
}

const char& str::operator[](int index) const
{
	return m_ptr[resolve_index(index)];
}

const str::iterator str::begin() const
{
	return iterator(&m_ptr[0]);
}

const str::iterator str::end() const
{
	return iterator(&m_ptr[m_len]);
}
