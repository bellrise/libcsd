/* <libcsd/args.h>
   Copyright (c) 2024 bellrise */

#pragma once

#include <libcsd/list.h>
#include <libcsd/map.h>
#include <libcsd/str.h>

namespace csd {

/**
 * @class args
 * Command-line argument parser.
 */
struct args
{
	args();

	void add_flag(char short_opt, str long_opt, str help);
	void add_argument(char short_opt, str long_opt, str help);

	map<str, str> parse_args(int argc, char **argv);

  private:
	struct opt
	{
		char short_opt;
		str long_opt;
		str help;
	};

	list<opt> m_opts;
};

} // namespace csd
