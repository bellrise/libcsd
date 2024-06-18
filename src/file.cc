/* libcsd/src/file.cc
   Copyright (c) 2023 bellrise */

#include "libcsd/list.h"

#include <fcntl.h>
#include <libcsd/bytes.h>
#include <libcsd/error.h>
#include <libcsd/file.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

namespace csd {

file::file()
	: m_fd(-1)
	, m_flags(ios_closed)
	, m_path()
	, m_mode()
{ }

file::~file()
{
	if (m_flags != ios_closed)
		close();
}

void file::open(str path, const str& mode)
{
	int open_flags = 0;

	m_path = path;
	m_flags = (mode.find("r") == str::invalid_index ? 0 : ios_read)
			| (mode.find("w") == str::invalid_index ? 0 : ios_write)
			| (mode.find("b") == str::invalid_index ? 0 : ios_binary);

	if (m_flags & ios_read)
		open_flags |= O_RDONLY;
	else if (m_flags & ios_write)
		open_flags |= O_WRONLY;
	else if (m_flags & (ios_write | ios_read))
		open_flags |= O_RDWR;

	m_fd = ::open(path.view().ptr, open_flags);
	if (m_fd == -1) {
		throw csd::invalid_operation_exception("failed to open file");
		m_flags = ios_closed;
	}
}

void file::close()
{
	if (m_flags)
		::close(m_fd);
	m_flags = 0;
	m_fd = -1;
}

bool file::is_open() const
{
	return m_flags != ios_closed;
}

bool file::is_eof() const
{
	size_t offset;

	if (!is_open())
		return true;

	offset = lseek(m_fd, 0, SEEK_CUR);
	return offset == size();
}

size_t file::write(const bytes& buf)
{
	if (!is_writable())
		throw csd::invalid_operation_exception("not writable");
	return ::write(m_fd, buf.raw_ptr(), buf.size());
}

bytes file::read(int size)
{
	ssize_t bytes_read;
	bytes res;
	char *buf;

	if (!is_readable())
		throw csd::invalid_operation_exception("not readable");

	buf = (char *) malloc(size);
	bytes_read = ::read(m_fd, buf, size);

	if (bytes_read == -1) {
		free(buf);
		throw csd::invalid_operation_exception(
			"failed to read file descriptor");
	}

	if (!bytes_read) {
		free(buf);
		return res;
	}

	res.alloc(bytes_read);
	res.copy_from(buf, bytes_read);

	free(buf);
	return res;
}

size_t file::write_string(const str& s)
{
	return write(csd::move(s.to_bytes()));
}

str file::read_string(int size)
{
	return read(size).as_str();
}

void file::seek(size_t pos)
{
	if (!is_open())
		return;
	lseek(m_fd, pos, SEEK_SET);
}

stream file::stream()
{
	struct stream s;

	if (is_readable()) {
		s.set_read_single([this]() {
			bytes buf = this->read(1);
			if (!buf.size())
				throw csd::stream_exception("end of file");
			return buf[0];
		});

		s.set_is_open([this]() {
			return !this->is_eof();
		});
	}

	if (is_writable()) {
		s.set_write_single([this](bytes::byte b) {
			::write(this->m_fd, &b, 1);
		});

		s.set_is_open([this]() {
			return this->is_open();
		});
	}

	return s;
}

str file::read_all()
{
	return read_string(size());
}

list<str> file::read_lines()
{
	str contents = read_string(size());
	list<str> lines = split_str(contents, "\n");
	lines.apply([](str& line) {
		return line.strip();
	});

	return lines;
}

size_t file::size() const
{
	struct stat fs;

	if (!is_open()) {
		throw csd::invalid_operation_exception(
			"cannot get size of closed file");
	}

	if (fstat(m_fd, &fs) == -1)
		throw csd::invalid_operation_exception("failed to stat file");

	return fs.st_size;
}

bool file::is_readable()
{
	return m_flags & ios_read;
}

bool file::is_writable()
{
	return m_flags & ios_write;
}

} // namespace csd
