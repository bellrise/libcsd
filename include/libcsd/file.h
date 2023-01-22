/* <libcsd/file.h>
    Copyright (c) 2022-2023 bellrise */

#pragma once

#include <libcsd/list.h>
#include <libcsd/str.h>
#include <libcsd/error.h>

#include <stdio.h>

namespace csd {

class file {
	FILE *fp;
	str path;
	str mode;
	bool state = 0;

	bool is_readable()
	{
		list<str> modes { "r", "r+", "w+", "a+" };

		for (auto m : modes) {
			if (m == mode)
				return true;
		}

		return false;
	}

	bool is_writable()
	{
		list<str> modes { "r+", "w", "w+", "a", "a+" };

		for (auto m : modes) {
			if (m == mode)
				return true;
		}

		return false;
	}

	public:
	file(str path, str mode)
		: path(path), mode(mode) {}

	~file()
	{ if (state) fclose(fp); }

	void write(str s)
	{
		if (!is_writable())
			throw csd::unsupported_operation_exeception("not writable");

		if (!state)
			fp = fopen(path.unsafe_ptr(), mode.unsafe_ptr());

		fputs(s.unsafe_ptr(), fp);
	}

	str readlines()
	{
		int c;
		str s;

		if (!is_readable())
			throw csd::unsupported_operation_exeception("not readable");

		while ((c = fgetc(fp)) != EOF)
			s.append(c);

		return s;
	}
};

}
