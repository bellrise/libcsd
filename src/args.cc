/* libcsd/src/args.cc
   Copyright (c) 2024 bellrise */

#include <libcsd/args.h>

using namespace csd;

args::args() { }

void args::add_flag(char short_opt, str long_opt, str help)
{
	m_opts.append(opt{short_opt, long_opt, help, OPT_FLAG});
}

void args::add_argument(char short_opt, str long_opt, str help)
{
	m_opts.append(opt{short_opt, long_opt, help, OPT_SINGLE});
}

map<str, str> args::parse_args(int argc, char **argv)
{
	throw csd::invalid_operation_exception("args::parse_args is unimplemented");
}
