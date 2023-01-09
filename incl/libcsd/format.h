/* <libcsd/format.h>
   Copyright (c) 2023 bellrise */

#pragma once

#include <libcsd/str.h>

namespace csd {

inline str format(const str& empty_fmt)
{
	return empty_fmt;
}

/**
 * @function format
 * Format the given string with the following arguments.
 *
 *  str hello = csd::format("Hello there {}, you are {} yo!", "John", 12);
 *  println(hello); // Hello there John, you are 12 yo!
 */
template <csd::StringConvertible T, typename... VT>
str format(const str& fmt, T first_arg, VT... args)
{
	int index;
	str formatted;

	if ((index = fmt.find("{}")) == str::invalid_index)
		return fmt;

	formatted += fmt.substr(0, index);
	formatted += str(first_arg);
	formatted += format(fmt.substr(index + 2), args...);

	return formatted;
}


}
