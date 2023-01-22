/* <libcsd/file.h>
    Copyright (c) 2023 bellrise */

#pragma once

#include <libcsd/error.h>
#include <libcsd/list.h>
#include <libcsd/str.h>
#include <stdio.h>

/*
 * TODO for csd::file:
 * The type should implement open() instead of opening the file while writing or
 * reading. If the constructor takes a path, it should be obvious that the file
 * is opened, and not kept close until you try to write to it.
 *
 * A pair of read/write methods for bytes should be provided for binary files.
 *
 * Replace a FILE with the underlying POSIX file descriptor.
 */

namespace csd {

struct file
{
	file(const str& path, const str& mode);
	~file();

	void write(str s);
	str readlines();

    private:
	FILE *m_fp;
	str m_path;
	str m_mode;
	bool m_state = false;

	bool is_readable();
	bool is_writable();
};

} // namespace csd
