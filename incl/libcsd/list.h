/* <libcsd/list.h>
    Copyright (c) 2022 bellrise */

#pragma once

#include <libcsd/detail.h>

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
		delete [] m_ptr;
	}

	size_t len() const
	{
		return m_len;
	}

	void append(T copied_value)
	{
		resize(++m_len);
		m_ptr[m_len - 1] = copied_value;
	}

	void remove(int index)
	{
		if (index >= len())
			throw index_exception(index, 0, len() - 1);

		delete m_ptr[index];

		for (int i = index; i < m_len; i++)
			m_ptr[i] = m_ptr[i + 1];

		resize(--m_len);
	}

	template<T>
	requires std::equality_comparable<T>
	void remove(T item)
	{
		for (int i = 0; i < m_len; i++) {
			if (item == m_ptr[i]) {
				remove(i);
				return;
			}
		}
	}

	str to_str() const
	{
		return str("<list len=") + len() + '>';
	}

	T &operator[](size_t index)
	{
		if (index >= len())
			throw index_exception(index, 0, len() - 1);
		return m_ptr[index];
	}

	const T &operator[](size_t index) const
	{
		if (index >= len())
			throw index_exception(index, 0, len() - 1);
		return m_ptr[index];
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
			new_size = (1 << i);
			if (new_size >= n)
				break;
		}

		if (n > 1024)
			new_size = ((n << 10) >> 10) + 1024;

		if (m_ptr == nullptr) {
			m_ptr = new T[new_size];
			m_space = new_size;
			return;
		}

		T *old_ptr = m_ptr;
		m_ptr = new T[new_size];
		copy_objects(m_ptr, old_ptr, m_space);
		delete [] old_ptr;
		m_space = new_size;
	}

	template <typename P>
	void copy_objects(P *new_ptr, P *old_ptr, size_t n)
	{
		for (size_t i = 0; i < n; i++)
			new_ptr[i] = old_ptr[i];
	}

	template <csd::IsMovable P>
	void copy_objects(P *new_ptr, P *old_ptr, size_t n)
	{
		for (size_t i = 0; i < n; i++)
			new_ptr[i] = static_cast<T&&>(old_ptr[i]);
	}

	size_t m_space;
	size_t m_len;
	T *m_ptr;
};
