/* <libcsd/routine.h>
   Copyright (c) 2022-2023 bellrise */

#pragma once

#include <libcsd/bytes.h>
#include <libcsd/detail.h>
#include <libcsd/error.h>
#include <stddef.h>

template <typename...>
struct routine;

/**
 * @class routine<R(Args...)>
 * Thin wrapper around a function pointer. A function or lambda can be assigned
 * to a routine, and called later.
 *
 *  int add_ints(int a, int b)
 *  {
 *      return a + b;
 *  }
 *
 *  void f()
 *  {
 *      routine<int(int, int)> adder = add_ints;
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
 *      using onclick_callback_t = routine<void(const Widget&)>;
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
 *  w.onclick([] (const Widget& widget) {
 *      println("Widget", widget.name(), "was clicked!");
 *  });
 *
 */
template <typename R, typename... Args>
struct routine<R(Args...)>
{
	using return_type = R;
	using type = R (*)(Args...);

	routine()
	    : m_emplace(nullptr)
	    , m_invoke(nullptr)
	{ }

	template <csd::Signature<R, Args...> F>
	routine(F routine)
	    : m_emplace(reinterpret_cast<emplace_type>(emplace_impl<F>))
	    , m_invoke(reinterpret_cast<invoke_type>(invoke_impl<F>))
	{
		m_container.alloc(sizeof(routine));
		m_emplace((char *) m_container.raw_ptr(),
			  reinterpret_cast<char *>(&routine));
	}

	bool has_routine() const
	{
		return !!m_container.size();
	}

	R operator()(Args... args) const
	{
		if (!m_container.size())
			throw csd::nullptr_exception(
			    "routine missing function pointer");
		return m_invoke((char *) m_container.raw_ptr(), args...);
	}

    private:
	using emplace_type = void (*)(void *, void *);
	using invoke_type = R (*)(void *, Args...);

	bytes m_container;
	emplace_type m_emplace;
	invoke_type m_invoke;

	template <typename F>
	static void emplace_impl(F *container, F *func)
	{
		new (container) F(*func);
	}

	template <typename F>
	static R invoke_impl(F *func, Args... args)
	{
		return (*func)(args...);
	}
};
