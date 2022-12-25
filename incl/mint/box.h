/* <mint/box.h>
   Copyright (c) 2022 bellrise */

#pragma once
#include <mint/rc.h>

/**
 * @class box<T>
 * Reference-counted allocated space on the heap. You can use a box<T>
 * similiarly to a regular pointer, with * and -> operators for accessing
 * the value underneath. For example, this snippet will work:
 *
 *  box<int> number { 12 };
 *  *number = 33;
 *
 * Note that a box _must_ be initialized with a value or another box. Because
 * this is reference counted, a box<T> can be both returned and passed to
 * other functions:
 *
 *  box<int> allocate_number(int value)
 *  {
 *      return box<int> { value };
 *  }
 *
 *	void modify(box<int> number)
 *	{
 *      *number = 66;
 *	}
 *
 *  void f()
 *  {
 *      box<int> a = allocate_number(16);
 *      modify(a);
 *      println(a);     // 66
 *  }
 *
 * This will not produce any leaks. Note that you can rewrite `return box<int>`
 * as a single `return value;` which will get implicitly casted to a box<int>.
 */
template <typename T>
struct box
{
	/* Boxes must be initialized with a value. */
	box() = delete;

	box(T &&moved_value)
		: m_ptr(new T(moved_value))
		, m_arc(new mint::__arc)
	{
		mint::__arc_init(*m_arc, 1);
	}

	box(const T &copy_value)
		: m_ptr(new T(copy_value))
		, m_arc(new mint::__arc)
	{
		mint::__arc_init(*m_arc, 1);
	}

	box(box &other)
		: m_ptr(other.m_ptr)
		, m_arc(other.m_arc)
	{
		mint::__arc_inc(*m_arc);
	}

	/* Boxes may not be moved. */
	box(box &&moved) = delete;

	~box()
	{
		if (!mint::__arc_dec(*m_arc)) {
			delete m_ptr;
			delete m_arc;
		}
	}

	box<T> &operator=(box new_box)
	{
		if (!mint::__arc_dec(*m_arc)) {
			delete m_ptr;
			delete m_arc;
		}

		m_ptr = new_box.m_ptr;
		m_arc = new_box.m_arc;
		mint::__arc_inc(*m_arc);

		return *this;
	}

	T &operator*()
	{
		return *m_ptr;
	}

	const T &operator*() const
	{
		return *m_ptr;
	}

	T *operator->()
	{
		return m_ptr;
	}

	const T *const operator->() const
	{
		return m_ptr;
	}

private:
	T *m_ptr;
	mint::__arc *m_arc;
};
