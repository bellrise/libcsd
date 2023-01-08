/* <libcsd/detail.h>
    Copyright (c) 2022-2023 bellrise */

#pragma once

#include "libcsd/error.h"
#include <libcsd/list.h>
#include <libcsd/detail.h>
#include <strings.h>

/**
 * @class map<T, U>
 * Dynamically resizable dictionary;
 */
template <typename T, typename U>
struct map {
	list<T> m_keys;
	list<U> m_values;
	size_t m_len = 0;

public:
	inline size_t len()
	{
		return m_len;
	}

	inline list<T> keys()
	{
		return m_keys;
	}

	inline list<U> values()
	{
		return m_values;
	}

	inline map<T, U> copy()
	{
		return this;
	}

	map() {}

	map(T key, U value)
	{
		append(key, value);
	}

	template <csd::IsComparable<T> V>
	map<T, U> fromkeys(list<V> keys)
	{
		map<T, U> kv;

		for (int i = 0; i < m_len; i++)
			kv.append(m_keys[i], nullptr);

		return kv;
	}

	template <csd::IsComparable<T> V>
	void update(V key, U value)
	{
		for (int i = 0; i < m_len; i++)
			if (key == m_keys[i])
				m_values[i] = value;
	}

	void clear()
	{
		for (int i = 0; i < m_len; i++) {
			remove(i);
		}

		m_len = 0;
	}

	template <csd::IsComparable<T> V>
	U pop(const V& key)
	{
		U value = get(key);
		remove(key);
		return value;
	}

	map<T, U> popitem()
	{
		map<T, U> kv;
		kv.append(m_keys[0], m_values[0]);
		remove(0);
		return kv;
	}

	template <csd::IsComparable<T> V>
	bool has_key(const V& key)
	{
		for (int i = 0; i < m_len; i++) {
			if (m_keys[i] == key)
				return true;
		}

		return false;
	}

	str to_str() const
	{
		str ret = "{ ";

		if (m_len == 0)
			return "{}";

		for (size_t i = 0; i < m_len; i++) {
			ret.append(m_keys[i]);
			ret.append(": ");
			ret.append(m_values[i]);

			if (i + 1 != m_len)
				ret.append(", ");
		}

		ret.append(" }");

		return ret;
	}

	void append(T key, U value)
	{
		m_keys.append(key);
		m_values.append(value);
		m_len++;
	}

	void append(map<T, U> other)
	{
		for (int i = 0; i < other.len(); i++) {
			append(other.keys()[i], other.values()[i]);
		}
	}

	void remove(int index)
	{
		if (index >= len())
			throw csd::index_exception(index, 0, len() - 1);

		m_keys.remove(index);
		m_values.remove(index);
		m_len--;
	}

	template <csd::IsComparable<T> V>
	void remove(const V& key)
	{
		for (int i = 0; i < m_len; i++) {
			if (m_keys[i] == key) {
				remove(i);
				return;
			}
		}
	}

	template <csd::IsComparable<T> V>
	U get(const V& key)
	{
		for (int i = 0; i < m_len; i++)
			if (m_keys[i] == key)
				return m_values[i];

		return 0;
	}

	U& operator[](int index)
	{
		return m_values[index];
	}


	template <csd::IsComparable<T> V>
	U& operator[](const V& key)
	{
		for (int i = 0; i < len(); i++)
			if (m_keys[i] == key)
				return m_values[i];
	}

	void operator+=(map<T, U> other)
	{
		for (int i = 0; i < other.len(); i++) {
			append(other.keys()[i], other.values()[i]);
		}

	}

	void operator-=(map<T, U> other)
	{
		for (int i = 0; i < other.len(); i++) {
			remove(other.keys()[i]);
		}
	}
};
