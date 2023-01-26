#pragma once

#include <libcsd/box.h>
#include <libcsd/map.h>
#include <libcsd/routine.h>

namespace csd {

/**
 * @class thread
 * Run some code on a different stack.
 *
 * TODO: document & show example
 */
struct thread
{
	using fptr_t = void *(*) (void *);

	/* inherited */
	thread() = default;
	thread(fptr_t fptr);
	thread(fptr_t fptr, void *arg);
	thread(const thread& copied_thread) = delete;
	thread(thread&& moved_thread);
	~thread();

	pthread_t getid() const;
	int join();
	void *result();
	int detach() const;
	int cancel() const;
	void exit();
	int kill(int sig) const;
	bool operator==(const thread& other) const;
	void operator()(fptr_t fptr, void *arg);
	void operator()(fptr_t fptr);

  private:
	pthread_t m_id;
	void *m_ret;
	bool m_busy = false;

	void run(fptr_t fptr, void *arg);
};

/**
 * @class typed_thread<R, A>
 * Wrapper around a csd::thread, which takes a single argument and returns a
 * single argument, but with real C++ types.
 *
 *  int sum(list<int> numbers)
 *  {
 *      int sum = 0;
 *      for (const auto& num : numbers)
 *          sum += num;
 *      return sum;
 *  }
 *
 *  void f()
 *  {
 *      csd::typed_thread<int, list<int>> sum_thread;
 *      list<int> args = {1, 2, 3};
 *
 *      // Run the thread
 *      sum_thread(sum, &args);
 *
 *      println(sum_thread.result());  // 6
 *  }
 */
template <typename R, typename A>
struct typed_thread
{
  private:
	using function_t = routine<R(A)>;
	struct thread_data
	{
		function_t func;
		R *res;
		A *arg;

		thread_data()
			: func()
			, res(NULL)
			, arg(NULL)
		{ }

		thread_data(function_t func_, R *res_, A *arg_)
			: func(func_)
			, res(res_)
			, arg(arg_)
		{ }
	};

  public:
	typed_thread()
		: m_running(false)
	{ }

	typed_thread(function_t func, A *arg)
		: m_thread_data(func, &m_result, arg)
		, m_thread((thread::fptr_t) thread_entry, &m_thread_data)
		, m_running(true)
	{ }

	R result()
	{
		return *static_cast<R *>(m_thread.result());
	}

	const thread& underlying_thread() const
	{
		return m_thread;
	}

	void operator()(function_t func, A *arg)
	{
		m_thread_data = thread_data(func, &m_result, arg);
		m_running = true;
		m_thread((thread::fptr_t) thread_entry, &m_thread_data);
	}

  private:
	thread_data m_thread_data;
	thread m_thread;
	bool m_running;
	R m_result;

	static void *thread_entry(thread_data *arg)
	{
		*arg->res = arg->func(*arg->arg);
		return arg->res;
	}
};

} // namespace csd
