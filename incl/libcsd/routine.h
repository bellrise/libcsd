/* <libcsd/routine.h>
   Copyright (c) 2022 bellrise */

#pragma once

#include <libcsd/error.h>
#include <stddef.h>

/**
 * @class routine<R, P...>
 * Thin wrapper around a function pointer.
 */
template <typename R, typename... P>
struct routine
{
	static constexpr size_t n_args = sizeof...(P);

	using type = R (*) (P...);
	using return_type = R;

	routine()
		: m_routine(nullptr)
	{ }

	routine(type routine)
		: m_routine(routine)
	{ }

	template <typename F>
	routine(F routine)
		: m_routine(routine)
	{ }

	R operator()(P... args) const
	{
		if (m_routine == nullptr)
			throw nullptr_exception("routine missing function pointer");
		return m_routine(args...);
	}

	bool has_pointer() const
	{
		return m_routine != nullptr;
	}

private:
	type m_routine;
};
