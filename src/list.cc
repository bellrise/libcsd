/* libcsd/src/list.cc
   Copyright (c) 2023 bellrise */

#include <libcsd/list.h>

namespace csd {

list<str> split_str(str to_split, const str& by)
{
	list<str> parts;
	int index;

	while (1) {
		if ((index = to_split.find(by)) == str::invalid_index) {
			parts.append(to_split);
			break;
		}

		parts.append(to_split.substr(0, index));
		to_split = to_split.substr(index + by.len());
	}

	return parts;
}

} // namespace csd
