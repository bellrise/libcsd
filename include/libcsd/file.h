/* <libcsd/file.h>
	Copyright (c) 2023 bellrise */

#pragma once

#include <libcsd/error.h>
#include <libcsd/str.h>
#include <stdio.h>

namespace csd {

struct file
{
	file();
	~file();

	void open(const str& path, const str& mode);
	void close();
	bool is_open() const;

	size_t write(const bytes& buf);
	bytes read(int size);

	size_t write_string(const str& s);
	str read_string(int size);

	str read_all();

	/* Return the size of the file in bytes */
	size_t size() const;

  private:
	enum ios
	{
		ios_closed = 0,
		ios_read = 1,
		ios_write = 2,
		ios_binary = 4
	};

	int m_fd;
	int m_flags;
	str m_path;
	str m_mode;

	bool is_readable();
	bool is_writable();
};

} // namespace csd
