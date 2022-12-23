/* <mint/str.h>
   Copyright (c) 2022 bellrise */

#pragma once
#include <stddef.h>
#include <stdio.h>

struct str;

/**
 * @concept StringConvertible
 * Any type that can be converted to a `str`. Any type returned by T::to_str()
 * passable to the str::str() constructor is also convertable to a string.
 * This means that your to_str() method does not need to return a str, but can
 * instead return a const char*.
 */
template <typename T>
concept StringConvertible = requires(T t, str v)
{
	static_cast<str>(t.to_str());
};

/**
 * @class str
 * Heap-allocated string.
 */
struct str
{
	str();
	str(str &&moved);
	str(const str &other);
	str(const char *string);
	str(const char *string, size_t maxlen);

	/* Accept anything that can be converted into a string. */
	template <StringConvertible T>
	str(const T &object)
		: str(object.to_str())
	{ }

	~str();

	/* Stat methods */
	size_t len() const;
	str to_str() const;
	void print() const;

	/* Chain-modify methods */
	str &replace(char from, char to);
	str &append(const str &next);
	str &append(const char *next);

	/* Overloads */
	str &operator=(const str &other);
	str &operator=(const char *other);
	str &operator+(const str &next);
	str &operator+(const char *next);
	str &operator+=(const str &next);
	str &operator+=(const char *next);

protected:
	char *m_ptr;
	size_t m_space;
	size_t m_len;

	void copy_from(const str &other);
	void copy_from_raw(const char *other, size_t other_len);

	/* Resize the buffer the string is stored in. This must return the number
	   of available bytes to use, and not the resized space. This way any caller
	   functions may check if the next operation will fit in the buffer. resize()
	   may modify m_space. */
	[[nodiscard]] size_t resize(size_t nbytes);
};
