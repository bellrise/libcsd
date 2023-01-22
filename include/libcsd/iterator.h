/* <libcsd/iterator.h>
   Copyright (c) 2023 bellrise */

#pragma once

#include <libcsd/detail.h>

namespace csd {

template <typename T, typename Pointer = T *, typename Reference = T&>
struct iterator
{
	/* T, Pointer & Reference should be based on the same type T. */
	static_assert(same_type<base_type<T>, base_type<Pointer>>);
	static_assert(
	    same_type<base_type<T>, remove_const<base_type<Reference>>>);

	iterator(Pointer ptr)
	    : m_ptr(ptr)
	{ }

	/**
	 * @method operator*
	 * Dereference the object. If the Pointer type is a double or triple
	 * indirection, it will dereference it twice or thrice.
	 */
	Reference operator*() const
	{
		if constexpr (same_type<T ***, Pointer>)
			return ***m_ptr;
		else if constexpr (same_type<T **, Pointer>)
			return **m_ptr;
		else
			return *m_ptr;
	}

	Pointer operator->()
	{
		return m_ptr;
	}

	iterator& operator++()
	{
		m_ptr++;
		return *this;
	}

	iterator& operator++(int)
	{
		return (*this)->operator++();
	}

	friend bool operator==(const iterator& a, const iterator& b)
	{
		return a.m_ptr == b.m_ptr;
	}

	friend bool operator!=(const iterator& a, const iterator& b)
	{
		return a.m_ptr != b.m_ptr;
	}

    protected:
	Pointer m_ptr;
};

} // namespace csd
