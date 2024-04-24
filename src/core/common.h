#pragma once

#include "logger.hpp"

#ifndef __linux__
#define FORCEINLINE __forceinline
#else
#define FORCEINLINE __attribute__((always_inline))
#endif
#define IN
#define OUT
#define EXPAND( x ) x
#define __propertyGetSet(getter, setter) __declspec(property (get=getter, put=setter))
#define __propertyGet(getter) __declspec(property (get=getter))
#define GET_MACRO(_1, _2, NAME, ...) NAME
#define __property(...) EXPAND( GET_MACRO(__VA_ARGS__, __propertyGetSet, __propertyGet)(__VA_ARGS__) )

#ifdef __GNUC__
#error GCC is not supported, please use Clang or MSVC instead.
#endif