/* <libcsd/error.h>
   Copyright (c) 2022-2023 bellrise */

#pragma once

#include <libcsd/str.h>

namespace csd {

/**
 * @exception any_exception
 * The base class for all libcsd exceptions, which you can catch
 * in a try-catch block, and then call .message() to get a detailed
 * error message. This snippet is an example on how to do so:
 *
 *  try {
 *      // problematic code
 *  } catch (const csd::any_exception& e) {
 *      println(e.message());
 *  }
 */
struct any_exception
{
	virtual str message() const = 0;
	virtual ~any_exception(){};
};

/**
 * @exception index_exception
 * Thrown on an out of bounds access.
 */
struct index_exception : public any_exception
{
	index_exception(const str& str_index);
	index_exception(int index);
	index_exception(int index, int min, int max);
	str message() const override;

    private:
	bool m_has_bounds;
	str m_str_index;
	int m_index;
	int m_min;
	int m_max;
};

/**
 * @exception unpack_exception
 * Tried to unpack a value from maybe<T>, but the value wasn't present.
 */
struct unpack_exception : public any_exception
{
	str message() const override;
};

/**
 * @exception nullptr_exception
 * Expected a value, but found a null pointer.
 */
struct nullptr_exception : public any_exception
{
	nullptr_exception();
	nullptr_exception(const str& message);

	str message() const override;

    private:
	str m_message;
};

/**
 * @exception memory_exception
 * No more space can be made or allocation failed.
 */
struct memory_exception : public any_exception
{
	memory_exception(const str& message);

	str message() const override;

    private:
	str m_message;
};

/**
 * @exception invalid_operation_exception
 * An operation that cannot be carried out.
 */
struct invalid_operation_exception : public any_exception
{
	invalid_operation_exception(const str& message);

	str message() const override;

    private:
	str m_message;
};

} // namespace csd
