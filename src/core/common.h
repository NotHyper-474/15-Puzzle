#pragma once

#include "logger.hpp"

#if __clang__ || _MSC_VER
#define ALLOWS_PROPERTIES 1
#endif

#ifndef __linux__
#define FORCEINLINE __forceinline
#else
#define FORCEINLINE __attribute__((always_inline))
#endif
#define IN
#define OUT
#define EXPAND( x ) x
#define GET_MACRO(_1, _2, NAME, ...) NAME

#if ALLOWS_PROPERTIES
#define __propertyGetSet(getter, setter) __declspec(property (get=getter, put=setter))
#define __propertyGet(getter) __declspec(property (get=getter))
#define __property(...) EXPAND( GET_MACRO(__VA_ARGS__, __propertyGetSet, __propertyGet)(__VA_ARGS__) )

#elif defined(__GNUC__)
#pragma message ("GCC doesn't support properties, please use Clang or (if you're on Windows) MSVC if you need to use them")
#endif