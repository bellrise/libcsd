/* <libcsd/routine.h>
   Copyright (c) 2022-2023 bellrise */

#pragma once

#include <libcsd/error.h>
#include <stddef.h>

/**
 * @class routine<R, P...>
 * Thin wrapper around a function pointer. A function or lambda can be assigned
 * to a routine, and called later. Note that only
 *
 *  int add_ints(int a, int b)
 *  {
 *      return a + b;
 *  }
 *
 *  void f()
 *  {
 *      routine adder = add_ints;
 *      println("The function takes", adder.n_args, "arguments");
 *
 *      auto res = adder(1, 2);
 *      println(res); // 3
 *  }
 *
 * The example above is a simple store, which may not be as useful in this
 * context. The better way of utilizing this is the same as libcsd does,
 * as method paramter. Suppose you have a type Widget which you can set
 * a callback on.
 *
 *  struct Widget
 *  {
 *      // This routine type returns `void` and takes one paramter,
 *      // a `const Widget&`. Note that routines can take any amount
 *      // of arguments.
 *      using onclick_callback_t = routine<void, const Widget&>;
 *
 *      void onclick(onclick_callback_t callback)
 *      {
 *          // We can simply set the m_onclick field with the passed pointer
 *          m_onclick = callback;
 *      }
 *
 *  private:
 *      onclick_callback_t m_onclick;
 *
 *      void on_widget_clicked()
 *      {
 *          // Called when something happens, we can call the user-passed
 *          // callback by simply using the () operator.
 *          m_onclick(*this);
 *      }
 *  };
 *
 * And in user code:
 *
 *  Widget w;
 *  w.onclick([] (const Widget& w) {
 *      println("Widget was clicked!");
 *  });
 *
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
			throw csd::nullptr_exception("routine missing function pointer");
		return m_routine(args...);
	}

	bool has_pointer() const
	{
		return m_routine != nullptr;
	}

private:
	type m_routine;
};
