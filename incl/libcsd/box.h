/* <libcsd/box.h>
   Copyright (c) 2022-2023 bellrise */

#pragma once

#include <libcsd/rc.h>
#include <libcsd/str.h>

/**
 * @class box<T>
 * Reference-counted allocated space on the heap. You can use a box<T>
 * similiarly to a regular pointer, with * and -> operators for accessing
 * the value underneath. For example, this snippet will work:
 *
 *  box<int> number { 12 };
 *  *number = 33;
 *
 * Because this is reference counted, a box<T> can be both returned and passed
 * to other functions:
 *
 *  box<int> allocate_number(int value)
 *  {
 *      return box<int> { value };
 *  }
 *
 *  void modify(box<int> number)
 *  {
 *      *number = 66;
 *  }
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
	box()
	    : m_ptr(new T)
	    , m_arc(new csd::__arc)
	{}

	box(T&& moved_value)
	    : m_ptr(new T(moved_value))
	    , m_arc(new csd::__arc)
	{
		csd::__arc_init(*m_arc, 1);
	}

	box(const T& copy_value)
	    : m_ptr(new T(copy_value))
	    , m_arc(new csd::__arc)
	{
		csd::__arc_init(*m_arc, 1);
	}

	box(const box& other)
	    : m_ptr(other.m_ptr)
	    , m_arc(other.m_arc)
	{
		csd::__arc_inc(*m_arc);
	}

	/* Boxes may not be moved. */
	box(box&& moved) = delete;

	~box()
	{
		if (!csd::__arc_dec(*m_arc)) {
			delete m_ptr;
			delete m_arc;
		}
	}

	bool same_reference_as(const box& other) const
	{
		return m_ptr == other.m_ptr;
	}

	box<T>& operator=(box new_box)
	{
		if (!csd::__arc_dec(*m_arc)) {
			delete m_ptr;
			delete m_arc;
		}

		m_ptr = new_box.m_ptr;
		m_arc = new_box.m_arc;
		csd::__arc_inc(*m_arc);

		return *this;
	}

	T& operator*()
	{
		return *m_ptr;
	}

	const T& operator*() const
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

	bool operator==(const box& other) const
	{
		return same_reference_as(other);
	}

	str to_str() const
	{
		char buf[64];
		snprintf(buf, 64, "<box %p refs=%zu>", m_ptr,
			 csd::__arc_load(*m_arc));
		return buf;
	}

    private:
	T *m_ptr;
	csd::__arc *m_arc;
};
