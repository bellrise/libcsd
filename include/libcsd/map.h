/* <libcsd/map.h>
	Copyright (c) 2022-2023 bellrise */

#pragma once

#include <libcsd/detail.h>
#include <libcsd/error.h>
#include <libcsd/format.h>
#include <libcsd/list.h>
#include <libcsd/maybe.h>

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

	template <typename... KV>
	map(KV... args)
	{
		append(args...);
	}

	map(const map& copied_map)
	{
		for (const pair& kv : copied_map)
			m_pairs.append(kv);
	}

	map(map&& moved_map)
		: m_pairs(csd::move(moved_map.m_pairs))
	{ }

	list<K> keys() const
	{
		list<K> keys;

		for (const pair& p : m_pairs)
			keys.append(p.key);

		return keys;
	}

	list<V> values() const
	{
		list<V> values;

		for (const pair& p : m_pairs)
			values.append(p.value);

		return values;
	}

	list<pair> items() const
	{
		return m_pairs;
	}

	template <csd::IsComparable<K> T>
	bool has_key(const T& key) const
	{
		for (const pair& kv : m_pairs) {
			if (kv.key == key)
				return true;
		}

		return false;
	}

	inline int len() const
	{
		return m_pairs.len();
	}

	void clear()
	{
		m_pairs.clear();
	}

	/**
	 * @method update
	 * Update a value at `key`. Throws if `key` is not found.
	 */
	template <csd::IsComparable<K> T>
	map& update(const T& key, const V& value)
	{
		for (pair p : m_pairs) {
			if (p.key == key) {
				p.value = value;
				return *this;
			}
		}

		throw csd::index_exception(key);
	}

	/**
	 * @method append
	 * Append a new key-value pair to the map. If such a key already exists,
	 * it calls update() instead.
	 */
	map& append(const K& key, const V& value)
	{
		if (has_key(key))
			update(key, value);
		else
			m_pairs.append({key, value});

		return *this;
	}

	map& remove(const K& key)
	{
		m_pairs.filter([key = key](const pair& kv) {
			return kv.key != key;
		});

		return *this;
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
		str ret = '{';

		if (!len())
			return "{}";

		for (int i = 0; i < len(); i++) {
			ret.append(m_pairs[i].key).append(": ").append(m_pairs[i].value);

			if (i + 1 != len())
				ret.append(", ");
		}

		return ret + '}';
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

	map& operator=(const map& other)
	{
		return this(other);
	}

	/**
	 * @method operator[]
	 * Access the value at `key`. Throws if such a key does not exist.
	 */
	template <csd::IsComparable<K> T>
	V& operator[](const T& map_key)
	{
		for (const auto& [key, value] : m_pairs) {
			if (key == map_key)
				return value;
		}

		throw csd::index_exception(map_key);
	}

	template <csd::IsComparable<K> T>
	const V& operator[](const T& map_key) const
	{
		for (const auto& [key, value] : m_pairs) {
			if (key == map_key)
				return value;
		}

		throw csd::index_exception(map_key);
	}

	map& operator+=(const map& other)
	{
		for (const auto& [key, value] : other)
			append(key, value);

		return *this;
	}

	map& operator-=(const map& other)
	{
		for (const auto& [key, _] : other) {
			if (has_key(key))
				remove(key);
		}

		return *this;
	}

	template <csd::IsComparable<K> Ko, csd::IsComparable<V> Vo>
	bool operator==(map<Ko, Vo> other)
	{
		if (len() != other.len())
			return false;

		for (int i = 0; i < len(); i++) {
			if (m_pairs[i].key != other.m_pairs[i].key
				|| m_pairs[i].value != other.m_pairs[i].value)
				return false;
		}

		return true;
	}

  private:
	list<pair> m_pairs;

	/* This is private, because the user shouldn't append many items in the
	   same call, but rather in a loop or with a .append() chain. */
	template <typename... Rest>
	map& append(const K& key, const V& value, Rest... rest)
	{
		append(key, value);

		if (sizeof...(rest) >= 2)
			append(rest...);

		return *this;
	}
};
