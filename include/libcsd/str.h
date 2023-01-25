/* <libcsd/str.h>
   Copyright (c) 2022-2023 bellrise */

#pragma once

#include <libcsd/iterator.h>
#include <stddef.h>

struct bytes;
struct str;

namespace csd {

/**
 * @concept ImplementsToString
 * Any type that implements the to_str() method, which may return str or
 * any other str-constructible type.
 */
template <typename T>
concept ImplementsToString = requires(T t) { static_cast<str>(t.to_str()); };

/**
 * @concept StringConvertible
 * Any type that can be converted to a `str`. Accepts types that have a
 * .to_str() method or can be passed into one of the str constructors.
 */
template <typename T>
concept StringConvertible =
    ImplementsToString<T> || requires(T t) { static_cast<str>(t); };

} // namespace csd

/**
 * @class str
 * Heap-allocated string.
 */
struct str
{
	using iterator = csd::iterator<char>;

	static constexpr int invalid_index = -1;

	str();
	str(str&& moved);
	str(const str& other);
	str(const char *string);
	str(const char *string, int maxlen);

	str(void *pointer);
	str(size_t number);
	str(float number);
	str(int number);
	str(char c);

	template <csd::ImplementsToString T>
	str(const T& object)
	    : str(object.to_str())
	{ }

	~str();

	int len() const;
	str to_str() const;
	bytes to_bytes() const;
	void print() const;
	bool empty() const;
	bool contains(const str& substr) const;
	bool begins_with(const str& other) const;
	bool ends_with(const str& other) const;

	/**
	 * @method find
	 * Returns the index at which the found sub-string starts,
	 * or str::invalid_index if no string is found.
	 */
	int find(const str& substr) const;

	/**
	 * @method substr
	 * As in any popular language, substr() returns a slice of the string
	 * starting at `start_index`, and continuing for `length` bytes. An
	 * invalid set of arguments will just return an empty string. The
	 * default `length` will collect to the end of the string.
	 */
	str substr(int start_index, int length = -1) const;

	const char *unsafe_ptr() const
	{
		return m_ptr;
	}

	/* Chain-modify methods */
	str& replace(char from, char to);
	str& replace(const str& substr, const str& replacement);
	str& append(const str& next);
	str& append(const char *next);

	/* Operator overloads */
	str& operator=(const str& other);
	str& operator=(const char *other);
	str& operator+(const str& next);
	str& operator+(const char *next);
	str& operator+=(const str& next);
	str& operator+=(const char *next);
	bool operator==(const str& other) const;

	/**
	 * @method []
	 * Returns the character at the given index. May throw index_exception
	 * if `index` is out of bounds.
	 */
	char& operator[](int index);
	const char& operator[](int index) const;

	const iterator begin() const;
	const iterator end() const;

    protected:
	char *m_ptr;
	int m_space;
	int m_len;

	void copy_from(const str& other);
	void copy_from_raw(const char *other, int other_len);

	int resolve_index(int index) const;

	/* Resize the buffer the string is stored in. This must return the
	   number of available bytes to use, and not the resized space. This way
	   any caller functions may check if the next operation will fit in the
	   buffer. resize() may modify m_space. */
	[[nodiscard]] int resize(int nbytes);
};
