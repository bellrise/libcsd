/* libcsd/src/file.cc
   Copyright (c) 2023 bellrise */

#include <libcsd/file.h>

namespace csd {

file::file(const str& path, const str& mode)
    : m_path(path)
    , m_mode(mode)
{ }

file::~file()
{
	if (m_state)
		fclose(m_fp);
}

void file::write(str s)
{
	if (!is_writable())
		throw csd::invalid_operation_exception("not writable");

	if (!m_state) {
		m_fp = fopen(m_path.unsafe_ptr(), m_mode.unsafe_ptr());
	}

	fputs(s.unsafe_ptr(), m_fp);
}

str file::readlines()
{
	int c;
	str s;

	if (!is_readable())
		throw csd::invalid_operation_exception("not readable");

	while ((c = fgetc(m_fp)) != EOF)
		s.append(c);

	return s;
}

bool file::is_readable()
{
	list<str> modes = {"r", "r+", "w+", "a+"};

	for (auto& m : modes) {
		if (m == m_mode)
			return true;
	}

	return false;
}

bool file::is_writable()
{
	list<str> modes = {"r+", "w", "w+", "a", "a+"};

	for (auto& m : modes) {
		if (m == m_mode)
			return true;
	}

	return false;
}

} // namespace csd
