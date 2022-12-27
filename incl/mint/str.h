/* <mint/str.h>
   Copyright (c) 2022 bellrise */

#pragma once
#include <stddef.h>

struct str;

/**
 * @concept StringConvertible
 * Any type that can be converted to a `str`. Any type returned by T::to_str()
 * passable to the str::str() constructor is also convertable to a string.
 * This means that your to_str() method does not need to return a str, but can
 * instead return a const char*.
 */
template <typename T>
concept StringConvertible = requires (T t)
{
	static_cast<str>(t.to_str());
};

/**
 * @class str
 * Heap-allocated string.
 */
struct str
{
	static constexpr size_t invalid_index = -1;

	str();
	str(str &&moved);
	str(const str &other);
	str(const char *string);
	str(const char *string, size_t maxlen);

	/* Conversion constructors */
	str(size_t number);
	str(int number);
	str(char c);

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

	/* Returns the index at which the found sub-string starts,
	   or str::invalid_index if no string is found. */
	size_t find(const str &substr);

	/* As in any popular language, substr() returns a slice of the string
	   starting at `start_index`, and continuing for `length` bytes. An invalid
	   set of arguments will just return an empty string. The default `length`
	   will collect to the end of the string. */
	str substr(size_t start_index, size_t length = -1);

	/* Chain-modify methods */
	str &replace(char from, char to);
	str &append(const str &next);
	str &append(const char *next);

	/* Operator overloads */
	str &operator=(const str &other);
	str &operator=(const char *other);
	str &operator+(const str &next);
	str &operator+(const char *next);
	str &operator+=(const str &next);
	str &operator+=(const char *next);
	bool operator==(const str &other) const;

	/* Returns the character at the given index. May throw index_exception if
	   `index` is out of bounds. */
	char &operator[](size_t index);
	const char &operator[](size_t index) const;

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
