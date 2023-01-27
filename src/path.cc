/* libcsd/src/path.cc
   Copyright (c) 2023 bellrise */

#include <dirent.h>
#include <libcsd/path.h>
#include <pwd.h>
#include <sys/stat.h>
#include <unistd.h>

namespace csd {

path::path()
	: m_parts()
	, m_root(false)
{ }

path::path(const path& copied_path)
{
	copy_from(copied_path);
}

path::path(path&& moved_path)
{
	move_from(csd::move(moved_path));
}

path::path(const str& path)
	: m_parts()
	, m_root(false)
{
	set(path);
}

path::path(const char *path)
	: m_parts()
	, m_root(false)
{
	set(path);
}

path::~path() { }

path& path::add(const str& part)
{
	m_parts.append(part);
	return *this;
}

path path::operator/(const str& part)
{
	path res = *this;
	res.add(part);
	return res;
}

path path::operator/(const char *part)
{
	path res = *this;
	res.add(part);
	return res;
}

str path::to_str() const
{
	str res = m_root ? "/" : "./";

	if (m_parts.len() == 0)
		return res;

	for (int i = 0; i < m_parts.len() - 1; i++) {
		res += m_parts[i];
		res += '/';
	}

	res += m_parts[-1];
	return res;
}

str path::filename() const
{
	if (m_parts.len() == 0)
		return m_root ? "/" : ".";
	return m_parts[-1];
}

path path::directory() const
{
	if (m_parts.len() == 0)
		return m_root ? "/" : ".";

	path new_path = *this;
	new_path.m_parts.remove(-1);
	return new_path;
}

path path::absolute() const
{
	if (m_root)
		return *this;

	path absolute_path = get_cwd();
	absolute_path.m_parts.extend(m_parts);
	return absolute_path;
}

bool path::is_file() const
{
	struct stat info;

	if (lstat(absolute().to_str().view().ptr, &info))
		return false;
	return S_ISREG(info.st_mode);
}

bool path::is_dir() const
{
	struct stat info;

	if (lstat(absolute().to_str().view().ptr, &info))
		return false;
	return S_ISDIR(info.st_mode);
}

bool path::exists() const
{
	return !access(absolute().to_str().view().ptr, F_OK);
}

list<path> path::dir_contents() const
{
	struct dirent *dir_thing;
	DIR *dir;
	list<path> contents;

	dir = opendir(absolute().to_str().view().ptr);

	while ((dir_thing = readdir(dir))) {
		if (str(dir_thing->d_name) == ".." || str(dir_thing->d_name) == ".") {
			continue;
		}

		path one = absolute();
		one.add(dir_thing->d_name);
		contents.append(csd::move(one));
	}

	closedir(dir);
	return contents;
}

path& path::operator=(const path& copied_path)
{
	copy_from(copied_path);
	return *this;
}

path& path::operator=(path&& moved_path)
{
	move_from(csd::move(moved_path));
	return *this;
}

list<str> path::resolve_parts(str path)
{
	list<str> resolved;

	if (path.len() == 0)
		m_parts = list<str>();

	if (path.contains("~") && path[0] != '~') {
		throw csd::invalid_argument_exception(
			"path cannot have a `~` anywhere other than the beginning "
			"of the path");
	}

	path.replace("~", get_user_home());

	if (path == ".") {
		return list<str>();
	} else if (path.begins_with("./")) {
		path = path.substr(2);
	} else if (path.begins_with("/")) {
		path = path.substr(1);
		m_root = true;
	}

	resolved = split_str(path, "/");
	resolved.filter([](const str& p) {
		return p != '.' && !p.empty();
	});

	return resolved;
}

void path::set(str path)
{
	list<str> parts = resolve_parts(path);
	for (auto&& part : parts)
		add(part);
}

str path::get_user_home()
{
	struct passwd *passwd;
	passwd = getpwuid(getuid());
	return passwd->pw_dir;
}

str path::get_cwd()
{
	bytes path;
	path.alloc(PATH_MAX);
	path.zero();

	getcwd((char *) path.raw_ptr(), PATH_MAX);
	return path.as_str();
}

void path::copy_from(const path& copied_path)
{
	m_parts = copied_path.m_parts;
	m_root = copied_path.m_root;
}

void path::move_from(path&& moved_path)
{
	m_parts = csd::move(moved_path.m_parts);
	m_root = moved_path.m_root;

	moved_path.m_parts.clear();
	moved_path.m_root = false;
}

} // namespace csd
