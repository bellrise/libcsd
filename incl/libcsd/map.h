/* <libcsd/detail.h>
    Copyright (c) 2022-2023 bellrise */

#pragma once

#include <libcsd/list.h>
#include <libcsd/maybe.h>

/**
 * @class map<T, U>
 * Dynamically resizable dictionary;
 */
template <typename T, typename U>
struct map {
	struct pair;

private:
	list<pair> m_pairs;
	size_t m_len = 0;
	using iterator = typename list<pair>::iterator;
	using const_iterator = typename list<pair>::const_iterator;

public:
	struct pair
	{
		T key;
		U value;

		str to_str() const
		{
			str ret;

			ret.append("{ ");
			ret.append(key);
			ret.append(": ");
			ret.append(value);
			ret.append(" }");

			return ret;
		}
	};

	inline size_t len()
	{
		return m_len;
	}

	map() = default;

	map(T key, U value)
	{
		append(key, value);
	}

	template<typename ...VT>
	map(VT ...args)
	{
		append(args...);
	}

	list<T> keys()
	{
		list<T> keys;

		for (pair p : m_pairs)
			keys.append(p.key);

		return keys;
	}

	list<U> values()
	{
		list<U> values;

		for (pair p : m_pairs)
			values.append(p.value);

		return values;
	}

	list<pair> items()
	{
		return m_pairs;
	}

	template <csd::IsComparable<T> V>
	bool has_key(const V& key)
	{
		for (int i = 0; i < this->len(); i++)
			if (m_pairs[i].key == key)
				return true;

		return false;
	}

	void clear()
	{
		for (int i = 0; i < m_len - 1; i++) {
			m_pairs.remove(i);
		}

		m_len = 0;
	}

	template <csd::IsComparable<T> V>
	void update(V key, U value)
	{
		for (pair p : m_pairs)
			if (p.key == key)
				p.value = value;
	}

	void append(T key, U value)
	{
		if (has_key(key)) {
			update(key, value);
		} else {
			m_pairs.append({ key, value });
			m_len++;
		}
	}

	template<typename... Rest>
	void append(T key, U value, Rest... rest)
	{
		append(key, value);

		if (sizeof...(rest) >= 2)
			append(rest...);
	}
	void remove(int index)
	{
		if (index >= m_len)
			throw csd::index_exception(index, 0, len() - 1);

		m_pairs.remove(index);
		m_len--;
	}

	template <csd::IsComparable<T> V>
	void remove(const V& key)
	{
		for (int i = 0; i < m_len; i++)
			if (m_pairs[i].key == key)
				remove(i);
	}

	template<csd::IsComparable<T> V, typename... Rest>
	void remove(const V& key, Rest... rest)
	{
		remove(key);

		if (sizeof...(rest) >= 1)
			remove(rest...);
	}

	template <csd::IsComparable<T> V>
	maybe<U> get(const V& key)
	{
		for (pair p : m_pairs)
			if (p.key == key)
				return p.value;


		return {};
	}

	template <csd::IsComparable<T> V>
	maybe<U> pop(const V& key)
	{
		maybe<U> value = get(key);
		remove(key);
		return value;
	}

	str to_str() const
	{
		str ret = "{ ";

		if (m_len == 0)
			return "{}";

		for (size_t i = 0; i < m_len; i++) {
			ret.append(m_pairs[i].key);
			ret.append(": ");
			ret.append(m_pairs[i].value);

			if (i + 1 != m_len)
				ret.append(", ");
		}

		ret.append(" }");

		return ret;
	}

	iterator begin()
	{
		return m_pairs.begin();
	}

	iterator end()
	{
		return m_pairs.end();
	}

	const_iterator begin() const
	{
		return m_pairs.begin();
	}

	const_iterator end() const
	{
		return m_pairs.end();
	}

	map& operator=(const map<T, U>& other)
	{
		return this(other);
	}

	U& operator[](int index)
	{
		return m_pairs.at(index).value;
	}

	const U& operator[](size_t index) const
	{
		return m_pairs.at(index).value;
	}

	template <csd::IsComparable<T> V>
	U& operator[](const V& key)
	{
		for (int i = 0; i < m_len; i++)
			if (m_pairs[i].key == key)
				return m_pairs[i].value;

		append(key, static_cast<U>(0));
		return m_pairs[m_len - 1].value;
	}

	map& operator+=(map<T, U> other)
	{
		for (int i = 0; i < other.len(); i++)
			append(other.keys()[i], other.values()[i]);

		return *this;
	}

	map& operator-=(map<T, U> other)
	{
		for (int i = 0; i < other.len(); i++)
			remove(other.keys()[i]);

		return *this;
	}

	template <csd::IsComparable<T> K, csd::IsComparable<U> V>
	bool operator==(map<K, V> other)
	{
		if (m_len != other.len())
			return false;

		for (int i = 0; i < m_len; i++) {
			if (m_pairs[i].key != other.keys()[i]
			 || m_pairs[i].value != other.values()[i])
				return false;
		}

		return true;
	}
};
