/* <mint/detail.h>
    Copyright (c) 2022 bellrise */

#pragma once

#include <mint/print.h>
namespace mint {

/* Utilities for the mint library */

template <typename T>
concept IsMovable = requires (T a, T v)
{
	a = static_cast<T&&>(v);
};

}
