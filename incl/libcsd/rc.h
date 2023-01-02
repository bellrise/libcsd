/* <libcsd/rc.h>
   Copyright (c) 2022 bellrise */

#pragma once

#include <atomic>

namespace csd {

/* We really should use the C++ standard library for this part, as atomics
   are differently implemented on random platforms and <stdatomic.h> just
   maps to <atomic> anyway. */

#if defined __GNUC__
# define __csd_force_inline    inline __attribute__((always_inline))
#else
# error "the reference counter implementation will not work"
#endif

using __arc_t = long;

/* The atomic reference counter. */
using __arc = std::atomic<__arc_t>;

void __csd_force_inline __arc_init(__arc &arc, __arc_t value)
{
	arc.store(value);
}

__arc_t __csd_force_inline __arc_inc(__arc &arc)
{
	return arc.fetch_add(1) + 1;
}

__arc_t __csd_force_inline __arc_dec(__arc &arc)
{
	return arc.fetch_sub(1) - 1;
}

__arc_t __csd_force_inline __arc_load(__arc &arc)
{
	return arc.load();
}

}
