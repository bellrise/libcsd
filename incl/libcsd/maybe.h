/* <libcsd/maybe.h>
   Copyright (c) 2022 bellrise */

#pragma once
#include <libcsd/error.h>
#include <string.h>

/**
 * @class maybe<T>
 * Possibly a value. Used as a return type in functions, which usually return
 * a value but may possibly fail, in which case they return an empty maybe<T>,
 * notifying the caller about an error.
 *
 *	maybe<int> to_int(str string)  // we may return an int
 *	{
 *      int parsed_int = 0;
 *      bool failed_to_parse;
 *
 *      // ... parse ...
 *
 *      if (failed_to_parse)       // if something fails, return an empty
 *          return {};             // `maybe<int> {}` signifying an error
 *      return parsed_int;         // otherwise, just return a regular value
 *	}
 *
 *	void f()
 *	{
 *      maybe value = to_int("ding dong");
 *
 *      if (value.is_ok()) {
 *          println("Parsed number:", value.unpack());
 *      } else {
 *          println("Failed to parse int");
 *      }
 *	}
 *
 * If a value is returned, maybe<T>.is_ok() will return true, which means the
 * value may be safely unpacked. If is_ok() returns false and the user still
 * tries to unpack the value, an `unpack_exception` is thrown.
 */
template <typename T>
struct maybe
{
	maybe()
		: m_ok(false)
	{ }

	maybe(const T &copied_value)
		: m_value(new T(copied_value))
		, m_ok(true)
	{ }

	maybe(T &&moved_value)
		: m_value(new T(moved_value))
		, m_ok(true)
	{ }

	~maybe()
	{
		delete m_value;
	}

	bool is_ok()
	{
		return m_ok;
	}

	T&& unpack()
	{
		if (!is_ok())
			throw unpack_exception();
		return static_cast<T&&>(*m_value);
	}

	void operator=(maybe &) =delete;

private:
	T *m_value;
	bool m_ok;
};
