/* <libcsd/map.h>
    Copyright (c) 2022-2023 bellrise */

#pragma once

#include <libcsd/format.h>
#include <libcsd/maybe.h>
#include <libcsd/list.h>

/**
 * @class map<K, V>
 * Stores an array of key-value pairs. Lookup of a pair happens using the key.
 */
template <typename K, typename V>
struct map
{
	struct pair
	{
		K key;
		V value;

		str to_str() const
		{
			return csd::format("{{}: {}}", key, value);
		}
	};

	using iterator = typename list<pair>::iterator;
	using const_iterator = typename list<pair>::const_iterator;

	map() = default;

	map(K key, V value)
	{
		append(key, value);
	}

	template<typename ...VK>
	map(VK ...args)
	{
		append(args...);
	}

	list<K> keys()
	{
		list<K> keys;

		for (pair p : m_pairs)
			keys.append(p.key);

		return keys;
	}

	list<V> values()
	{
		list<V> values;

		for (pair p : m_pairs)
			values.append(p.value);

		return values;
	}

	list<pair> items()
	{
		return m_pairs;
	}

	template <csd::IsComparable<K> T>
	bool has_key(const T& key)
	{
		for (int i = 0; i < this->len(); i++)
			if (m_pairs[i].key == key)
				return true;

		return false;
	}

	inline int len()
	{
		return m_len;
	}

	void clear()
	{
		for (int i = 0; i < m_len - 1; i++) {
			m_pairs.remove(i);
		}

		m_len = 0;
	}

	template <csd::IsComparable<K> T>
	void update(T key, V value)
	{
		for (pair p : m_pairs)
			if (p.key == key)
				p.value = value;
	}

	void append(K key, V value)
	{
		if (has_key(key)) {
			update(key, value);
		} else {
			m_pairs.append({ key, value });
			m_len++;
		}
	}

	template<typename... Rest>
	void append(K key, V value, Rest... rest)
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

	template <csd::IsComparable<K> T>
	void remove(const T& key)
	{
		for (int i = 0; i < m_len; i++)
			if (m_pairs[i].key == key)
				remove(i);
	}

	template<csd::IsComparable<K> T, typename... Rest>
	void remove(const T& key, Rest... rest)
	{
		remove(key);

		if (sizeof...(rest) >= 1)
			remove(rest...);
	}

	template <csd::IsComparable<K> T>
	maybe<V> get(const T& key)
	{
		for (pair p : m_pairs) {
			if (p.key == key)
				return p.value;
		}

		return {};
	}

	template <csd::IsComparable<K> T>
	maybe<V> pop(const T& key)
	{
		maybe<V> value = get(key);
		remove(key);
		return value;
	}

	str to_str() const
	{
		str ret = "{ ";

		if (m_len == 0)
			return "{}";

		for (int i = 0; i < m_len; i++) {
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

	map& operator=(const map<K, V>& other)
	{
		return this(other);
	}

	V& operator[](int index)
	{
		return m_pairs.at(index).value;
	}

	const V& operator[](size_t index) const
	{
		return m_pairs.at(index).value;
	}

	template <csd::IsComparable<K> T>
	V& operator[](const T& key)
	{
		for (int i = 0; i < m_len; i++)
			if (m_pairs[i].key == key)
				return m_pairs[i].value;

		append(key, static_cast<V>(0));
		return m_pairs[m_len - 1].value;
	}

	map& operator+=(map<K, V> other)
	{
		for (int i = 0; i < other.len(); i++)
			append(other.keys()[i], other.values()[i]);

		return *this;
	}

	map& operator-=(map<K, V> other)
	{
		for (int i = 0; i < other.len(); i++)
			remove(other.keys()[i]);

		return *this;
	}

	template <csd::IsComparable<K> Ko, csd::IsComparable<V> Vo>
	bool operator==(map<Ko, Vo> other)
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

private:
	list<pair> m_pairs;
	int m_len = 0;
};
