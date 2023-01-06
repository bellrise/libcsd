/* <libcsd/list.h>
    Copyright (c) 2022 bellrise */

#pragma once

#include <libcsd/detail.h>
#include <libcsd/str.h>
#include <stddef.h>
#include <string.h>

/**
 * @class list<T>
 * Dynamically resizable array of T.
 */
template <typename T>
struct list
{
	list()
		: m_space(0)
		, m_len(0)
		, m_ptr(nullptr)
	{ }

	template <typename ...Vt>
	list(Vt ...values)
		: m_space(0)
		, m_len(0)
		, m_ptr(nullptr)
	{
		T value_array[] = { values... };
		for (size_t i = 0; i < sizeof...(values); i++)
			append(value_array[i]);
	}

	~list()
	{
		delete_range(m_ptr, 0, m_space);
		delete [] m_ptr;
	}

	inline size_t len() const
	{
		return m_len;
	}

	void append(T copied_value)
	{
		resize(++m_len);
		m_ptr[m_len - 1] = new T(copied_value);
	}

	void remove(size_t index)
	{
		if (index >= len())
			throw index_exception(index, 0, len() - 1);

		delete m_ptr[index];

		for (size_t i = index; i < len(); i++)
			m_ptr[i] = m_ptr[i + 1];

		resize(--m_len);
	}

	template <csd::IsComparable<T> V>
	void remove(V& item)
	{
		for (size_t i = 0; i < len(); i++) {
			if (item == *m_ptr[i]) {
				remove(i);
				return;
			}
		}
	}

	str to_str() const
	{
		str builder = '[';

		if (m_len > 1) {
			for (size_t i = 0; i < len() - 1; i++) {
				builder += str(*m_ptr[i]) + ", ";
			}
			builder += str(*m_ptr[len() - 1]);
		} else {
			builder += str(*m_ptr[0]);
		}

		return builder + ']';
	}

	T& operator[](size_t index)
	{
		if (index >= len())
			throw index_exception(index, 0, len() - 1);
		return *(m_ptr[index]);
	}

	const T& operator[](size_t index) const
	{
		if (index >= len())
			throw index_exception(index, 0, len() - 1);
		return *(m_ptr[index]);
	}

private:
	/* Allocate enough space for n elements. */
	void resize(size_t n)
	{
		if (m_space >= n)
			return;

		/* Allocate to the nearest power of 2, or 1024 elements if the total
		   required size is over 1024. This will ensure a small memory footprint
		   for tiny arrays, but an over-allocation strategy for large arrays. */
		size_t new_size = 1024;
		for (int i = 0; i < 10; i++) {
			if ((size_t) (1 << i) < n)
				continue;
			new_size = (1 << i);
			break;
		}

		if (n > 1024)
			new_size = ((n >> 10) << 10) + 1024;

		/* Previously empty buffer. */
		if (m_ptr == nullptr) {
			m_ptr = new T*[new_size];
			zero_range(m_ptr, 0, new_size);
			m_space = new_size;
			return;
		}

		/* We already allocated something, so we need to create a new buffer
		   and copy the old pointers to the new buffer. */
		T **old_ptr = m_ptr;

		m_ptr = new T*[new_size];

		zero_range(m_ptr, 0, new_size);
		copy_range(m_ptr, old_ptr, m_space);

		delete [] old_ptr;
		m_space = new_size;
	}

	inline void copy_range(T **to_ptr, T **from_ptr, size_t n)
	{
		for (size_t i = 0; i < n; i++)
			to_ptr[i] = from_ptr[i];
	}

	void delete_range(T **ptr, size_t from, size_t to)
	{
		for (size_t i = from; i < to; i++) {
			if (ptr[i] == nullptr)
				continue;
			delete ptr[i];
			ptr[i] = nullptr;
		}
	}

	inline void zero_range(T **ptr, size_t from, size_t to)
	{
		for (size_t i = from; i < to; i++)
			ptr[i] = nullptr;
	}

	size_t m_space;
	size_t m_len;
	T **m_ptr;
};
