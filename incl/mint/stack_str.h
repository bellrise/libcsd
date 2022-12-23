/* <mint/stack_str.h>
   Copyright (c) 2022 bellrise */

#pragma once
#include <mint/str.h>
#include <mint/print.h>
#include <string.h>


/**
 * @class stack_str<N>
 * Stack-allocated string of a maximum length of N - 1. Usually small strings
 * should be stored on the stack to make them quickly accessible. "Small" in
 * this context usually means no more than 256 bytes per string. Note that if
 * you store a stack_str<N> in a dynamic array which uses the heap, it will be
 * allocated there, and not on the stack.
 *
 *  stack_str<32> tmp = "hello";
 *  print(tmp);
 */
template <size_t N>
struct stack_str : public str
{
	stack_str()
		: str(nullptr)
	{
		m_ptr = m_stack;
		m_space = N + 1;
		m_len = 0;
	}

	/**
	 * As the stack string does not allocate any memory, it acts like a regular
	 * variable during moving, which means it gets copied into the new place.
	 * Note that the stack strings can be of different size, the constructor
	 * will figure it out.
	 */
	template <size_t M>
	stack_str(stack_str<M> &&string)
		: stack_str(reinterpret_cast<const stack_str<M>&>(string))
	{ }

	template <size_t M>
	stack_str(const stack_str<M> &string)
	{
		size_t to_copy = m_len > string.m_len ? string.m_len : m_len;

		m_ptr = m_stack;
		m_space = N + 1;
		m_len = to_copy;

		memset(m_stack, 0, N + 1);
		memcpy(m_stack, string.m_stack, to_copy);
		m_stack[N] = 0;
	}

	stack_str(const char *string)
	{
		m_ptr = m_stack;
		m_space = N + 1;
		m_len = strlen(string);
		copy_from_raw(string, m_len);
	}

	virtual ~stack_str() override
	{
		/* Note: we trick the ~str() destructor that is called after us that
		   there is really nothing to free. */
		m_ptr = nullptr;
		m_space = 0;
	}

	str to_str() const override
	{
		return str(m_stack, m_len);
	}

	template <size_t M>
	stack_str &operator=(const stack_str<M> &other)
	{
		return this;
	}

private:
	char m_stack[N+1];
	size_t m_len;

	size_t resize([[maybe_unused]] size_t nbytes) override
	{
		return N + 1;
	}
};
