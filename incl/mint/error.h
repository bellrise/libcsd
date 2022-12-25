/* <mint/error.h>
   Copyright (c) 2022 bellrise */

#pragma once

struct str;

/**
 * @exception any_exception
 * The base class for all libmint exceptions, which you can catch
 * in a try-catch block, and then call .message() to get a detailed
 * error message. This snippet is an example on how to do so:
 *
 *  try {
 *      // problematic code
 *  } catch (const any_exception &e) {
 *      println(e.message());
 *  }
 */
struct any_exception
{
	virtual str message() const = 0;
	virtual ~any_exception() { };
};

/**
 * @exception index_exception
 * Thrown on an out of bounds access.
 */
struct index_exception : public any_exception
{
	index_exception(int index);
	index_exception(int index, int min, int max);

	str message() const override;

private:
	bool m_has_bounds;
	int m_index;
	int m_min;
	int m_max;
};
