/* libcsd/src/bytes.cc
   Copyright (c) 2022-2023 bellrise */

#include <libcsd/bytes.h>
#include <libcsd/error.h>
#include <libcsd/format.h>
#include <string.h>

bytes::bytes()
	: m_ptr(nullptr)
	, m_size(0)
	, m_user_provided(false)
{ }

bytes::bytes(bytes&& moved_bytes)
	: m_ptr(moved_bytes.m_ptr)
	, m_size(moved_bytes.m_size)
	, m_user_provided(moved_bytes.m_user_provided)
{
	moved_bytes.m_ptr = nullptr;
	moved_bytes.m_size = 0;
	moved_bytes.m_user_provided = false;
}

bytes::~bytes()
{
	if (!m_user_provided)
		delete[] m_ptr;
}

int bytes::size() const
{
	return m_size;
}

str bytes::to_str() const
{
	return csd::format("<bytes {} size={}>", (void *) m_ptr, m_size);
}

str bytes::as_str() const
{
	int null_at = m_size;

	for (int i = 0; i < m_size; i++) {
		if (m_ptr[i] == 0x00) {
			null_at = i;
			break;
		}
	}

	return str((const char *) m_ptr, null_at);
}

void bytes::alloc(int nbytes)
{
	if (m_user_provided) {
		if (nbytes > m_size) {
			throw csd::memory_exception("bytes: cannot alloc() more space in a "
										"user-provided memory area");
		}

		return;
	}

	if (nbytes == m_size)
		return;

	if (m_ptr == nullptr) {
		m_ptr = new byte[nbytes];
		m_size = nbytes;
		m_user_provided = false;
		return;
	}

	byte *old_ptr = m_ptr;
	int to_copy = m_size;

	m_ptr = new byte[nbytes];

	if (nbytes < m_size)
		to_copy = nbytes;

	memmove(m_ptr, old_ptr, to_copy);
	delete[] old_ptr;
	m_size = nbytes;
	m_user_provided = false;
}

void bytes::use_static_buffer(byte *raw_ptr, int nbytes)
{
	m_ptr = raw_ptr;
	m_size = nbytes;
	m_user_provided = true;
}

void bytes::zero()
{
	memset(m_ptr, 0, m_size);
}

bytes bytes::copy() const
{
	bytes buf;

	buf.m_ptr = new byte[m_size];
	buf.m_size = m_size;
	buf.copy_from(m_ptr, m_size);

	return buf;
}

void bytes::copy_from(const byte *raw_ptr, int nbytes)
{
	int to_copy = nbytes;

	if (nbytes > m_size)
		to_copy = m_size;
	memmove(m_ptr, raw_ptr, to_copy);
}

void bytes::copy_to(byte *raw_ptr, int nbytes) const
{
	int to_copy = nbytes;

	if (nbytes > m_size)
		to_copy = m_size;
	memmove(raw_ptr, m_ptr, to_copy);
}

bytes::byte *bytes::raw_ptr() const
{
	return m_ptr;
}

bytes& bytes::operator=(const bytes& other)
{
	if (!m_user_provided && m_ptr)
		delete[] m_ptr;

	m_user_provided = false;
	m_ptr = nullptr;

	alloc(other.m_size);
	copy_from(other.m_ptr, other.m_size);

	return *this;
}
