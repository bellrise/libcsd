/* <libcsd/path.h>
    Copyright (c) 2023 bellrise */

#pragma once

#include <libcsd/list.h>
#include <libcsd/str.h>

namespace csd {

struct path
{
	path();
	path(const path& copied_path);
	path(path&& moved_path);
	~path();

	path(const str& path);
	path(const char *path);

	path& add(const str& part);

	path operator/(const str& part);
	path operator/(const char *part);

	str to_str() const;
	str filename() const;
	path directory() const;
	path absolute() const;

	path& operator=(const path& copied_path);
	path& operator=(path&& moved_path);

    private:
	list<str> m_parts;
	bool m_root;

	void set(str path);
	void copy_from(const path& copied_path);
	void move_from(path&& moved_path);

	static str get_user_home();
	static str get_cwd();
};

} // namespace csd
