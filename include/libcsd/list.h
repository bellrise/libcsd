/* <libcsd/list.h>
	Copyright (c) 2022-2023 bellrise */

#pragma once

#include <libcsd/routine.h>
#include <memory.h>

/**
 * @class list<T>
 * Dynamically resizable array of T.
 */
template <typename T>
struct list
{
	using filter_consumer = routine<bool(const T&)>;
	using apply_consumer = routine<void(T&)>;
	using iterator = csd::iterator<T, T **, T&>;
	using const_iterator = csd::iterator<T, T **, const T&>;

	list()
		: m_space(0)
		, m_len(0)
		, m_ptr(nullptr)
	{ }

	template <typename... Vt>
	list(Vt... values)
		: m_space(0)
		, m_len(0)
		, m_ptr(nullptr)
	{
		T value_array[] = {values...};
		for (size_t i = 0; i < sizeof...(values); i++)
			append(value_array[i]);
	}

	list(const list& copied_list)
		: m_space(0)
		, m_len(copied_list.len())
		, m_ptr(nullptr)
	{
		allocate_atleast(m_len);
		copy_range(m_ptr, copied_list.m_ptr, m_len);
	}

	list(list&& moved_list)
		: m_space(moved_list.m_space)
		, m_len(moved_list.m_len)
		, m_ptr(moved_list.m_ptr)
	{
		moved_list.m_space = 0;
		moved_list.m_len = 0;
		moved_list.m_ptr = nullptr;
	}

	~list()
	{
		clear();
	}

	inline int len() const
	{
		return m_len;
	}

	list& append(const T& copied_value)
	{
		allocate_atleast(++m_len);
		m_ptr[m_len - 1] = new T(copied_value);
		return *this;
	}

	template <csd::IsMovable V>
	list& append(V&& moved_value)
	{
		allocate_atleast(++m_len);
		m_ptr[m_len - 1] = new T(csd::move(moved_value));
		return *this;
	}

	list& extend(const list& other_list)
	{
		for (const T& elem : other_list)
			append(elem);
		return *this;
	}

	void remove(int index)
	{
		index = resolve_index(index);
		delete m_ptr[index];

		for (int i = index; i < len() - 1; i++) {
			m_ptr[i] = m_ptr[i + 1];
			m_ptr[i + 1] = nullptr;
		}

		allocate_atleast(--m_len);
	}

	void remove_many(list<int> indices)
	{
		if (indices.len() == 1) {
			remove(indices[0]);
			return;
		}

		for (int index : indices) {
			index = resolve_index(index);
			delete m_ptr[index];
			m_ptr[index] = nullptr;
		}

		for (int run = 0; run < len(); run++) {
			for (int i = 0; i < len() - 1; i++) {
				if (m_ptr[i] != nullptr)
					continue;

				/* If the slot is unused, shift over by 1. */
				m_ptr[i] = m_ptr[i + 1];
				m_ptr[i + 1] = nullptr;
			}
		}

		m_len -= indices.len();
		allocate_atleast(m_len);
	}

	template <csd::IsComparable<T> V>
	void remove(const V& item)
	{
		for (int i = 0; i < len(); i++) {
			if (item == *m_ptr[i]) {
				remove(i);
				return;
			}
		}
	}

	void clear()
	{
		if (m_ptr) {
			delete_range(m_ptr, 0, m_len);
			delete[] m_ptr;
		}

		m_ptr = nullptr;
		m_len = 0;
		m_space = 0;
	}

	/**
	 * @method filter
	 * Filter the list, keeping only the elements that pass the check by
	 * the consumer. This means that when passing the value to the consumer,
	 * it should return a boolean value if the value should be kept.
	 *
	 *  list<int> numbers = { 1, 2, 3, 4, 5, 6 };
	 *  numbers.filter([] (const int& number) {
	 *      return number < 3;
	 *  });
	 *
	 *  // `numbers` is now { 1, 2 }
	 *
	 * In the example above an array of numbers is created, and filter with
	 * a lambda is called. All elements are passed by their `const T&` type.
	 * The lambda in this case only returns true if the number is smaller
	 * than 3, meaning only { 1, 2 } will be kept from the initial list.
	 */
	list& filter(auto consumer)
	{
		list<int> to_remove;

		for (int i = 0; i < len(); i++) {
			if (!consumer(at(i)))
				to_remove += i;
		}

		remove_many(to_remove);
		return *this;
	}

	/**
	 * @method apply
	 * Apply a change to each element, using an apply_consumer. May be used
	 * similarly to the filter consumer using a lambda, but the argument
	 * passed is of type `T&`, which can be modified.
	 */
	list& apply(auto consumer)
	{
		for (T& item : *this)
			consumer(item);
		return *this;
	}

	T& at(int index)
	{
		return *(m_ptr[resolve_index(index)]);
	}

	const T& at(int index) const
	{
		return *(m_ptr[resolve_index(index)]);
	}

	void remove_many(list<size_t> indices)
	{
		if (indices.len() == 1) {
			remove(indices[0]);
			return;
		}

		for (int i = 0; i < indices.len(); i++) {
			if (indices[i] >= len())
				throw csd::index_exception(i, 0, len() - 1);

			delete m_ptr[indices[i]];
			m_ptr[indices[i]] = nullptr;
		}

		for (size_t run = 0; run < len(); run++) {
			for (size_t i = 0; i < len() - 1; i++) {
				if (m_ptr[i] != nullptr)
					continue;

				/* If the slot is unused, shift over by 1. */
				m_ptr[i] = m_ptr[i + 1];
				m_ptr[i + 1] = nullptr;
			}
		}

		m_len -= indices.len();
		allocate_atleast(m_len);
	}

	str to_str() const
	{
		return string_repr<T>();
	}

	/* Operator overloads. */

	list& operator=(const list& other)
	{
		copy_from(other);
		return *this;
	}

	T& operator[](size_t index)
	{
		return at(index);
	}

	const T& operator[](size_t index) const
	{
		return at(index);
	}

	list& operator+=(const T& thing)
	{
		append(thing);
		return *this;
	}

	template <csd::IsMovable V>
	list& operator+=(V&& thing)
	{
		append(csd::move(thing));
		return *this;
	}

	/* comparable T & V */
	template <csd::IsComparable<T> V>
	bool operator==(const list<V>& other) const
	{
		if (len() != other.len())
			return false;

		for (int i = 0; i < len(); i++) {
			if (*m_ptr[i] != other[i])
				return false;
		}

		return true;
	}

	/* non-comparable T & V */
	template <typename V>
	bool operator==([[maybe_unused]] const list<V>& other) const
	{
		return false;
	}

	template <typename V>
	bool operator<(const list<V>& other) const
	{
		return len() < other.len();
	}

	template <typename V>
	bool operator>(const list<V>& other) const
	{
		return len() > other.len();
	}

	template <typename V>
	bool operator<=(const list<V>& other) const
	{
		return len() <= other.len();
	}

	template <typename V>
	bool operator>=(const list<V>& other) const
	{
		return len() >= other.len();
	}

	template <typename V>
	bool operator!() const
	{
		return !len();
	}

	iterator begin()
	{
		return iterator(&m_ptr[0]);
	}

	iterator end()
	{
		return iterator(&m_ptr[len()]);
	}

	const_iterator begin() const
	{
		return const_iterator(&m_ptr[0]);
	}

	const_iterator end() const
	{
		return const_iterator(&m_ptr[len()]);
	}

  private:
	int resolve_index(int index) const
	{
		if (index < 0)
			index = len() + index;
		if (index < 0 || index >= len())
			throw csd::index_exception(index, 0, len() - 1);
		return index;
	}

	/**
	 * @method allocate_atleast
	 * Allocate some memory to at least n slots. The allocation size is
	 * always the nearest power of 2, with the max of a 1024 slot step. This
	 * ensures reasonable allocation sizes for small arrays (2, 4, 8, ...
	 * elements) and a increasingly more hungry allocation strategy for
	 * large arrays (allocates another 1024 slots per call).
	 */
	void allocate_atleast(int n)
	{
		if (m_space >= n)
			return;

		int new_size = 1024;
		for (int i = 0; i < 10; i++) {
			if ((int) (1 << i) < n)
				continue;
			new_size = (1 << i);
			break;
		}

		if (n > 1024)
			new_size = ((n >> 10) << 10) + 1024;

		if (m_ptr == nullptr) {
			m_ptr = new T *[new_size];
			zero_range(m_ptr, 0, new_size);
			m_space = new_size;
			return;
		}

		T **old_ptr = m_ptr;

		m_ptr = new T *[new_size];

		zero_range(m_ptr, 0, new_size);
		memmove(m_ptr, old_ptr, sizeof(*m_ptr) * m_space);

		delete[] old_ptr;
		m_space = new_size;
	}

	void copy_range(T **to_ptr, T **from_ptr, int n)
	{
		delete_range(to_ptr, 0, n);
		for (int i = 0; i < n; i++) {
			to_ptr[i] = new T(*from_ptr[i]);
		}
	}

	void delete_range(T **ptr, int from, int to)
	{
		for (int i = from; i < to; i++) {
			if (ptr[i] == nullptr)
				continue;
			delete ptr[i];
			ptr[i] = nullptr;
		}
	}

	inline void zero_range(T **ptr, int from, int to)
	{
		for (int i = from; i < to; i++)
			ptr[i] = nullptr;
	}

	template <typename U>
	str string_repr() const
	{
		return "<list (non-printable elements)>";
	}

	template <csd::StringConvertible U>
	str string_repr() const
	{
		str builder = '[';

		if (len() == 0)
			return "[]";

		for (int i = 0; i < len() - 1; i++)
			builder += str(*m_ptr[i]) + ", ";

		builder += str(*m_ptr[len() - 1]);
		return builder + ']';
	}

	void copy_from(const list& other_list)
	{
		clear();
		for (const auto& v : other_list)
			append(v);
	}

	int m_space;
	int m_len;
	T **m_ptr;
};

namespace csd {
list<str> split_str(str to_split, const str& by);
}
