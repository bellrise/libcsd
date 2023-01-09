/* <libcsd/detail.h>
    Copyright (c) 2022 bellrise */

#pragma once

namespace csd {

/* Utilities for the csd library */

template <typename T>
concept IsMovable = requires (T a, T v)
{
	a = static_cast<T&&>(v);
};

template <typename T>
concept IsComparable = requires (T a, T b)
{
	a == b;
};

}
