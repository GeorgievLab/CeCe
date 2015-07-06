
#pragma once

/* ************************************************************************ */

#ifdef __MINGW64__
// Undefined reference to __impl_Py_InitModule4 on 64 bit MinGW compiler
#define MS_WIN64
// ::hypot issue
#include <cmath>
#endif

// Python
#include <Python.h>

// On Mac OS X there is a bug that defines those macros in C++ mode and
// replaces regular function declarations
#if __APPLE__ && __MACH__
#undef iscntrl
#undef iswcntrl
#undef isprint
#undef iswprint
#undef isspace
#undef iswspace
#undef isblank
#undef iswblank
#undef isgraph
#undef iswgraph
#undef ispunct
#undef iswpunct
#undef isalnum
#undef iswalnum
#undef isalpha
#undef iswalpha
#undef isupper
#undef iswupper
#undef islower
#undef iswlower
#undef isdigit
#undef iswdigit
#undef isxdigit
#undef iswxdigit
#undef toupper
#undef towupper
#undef tolower
#undef towlower
#endif

/* ************************************************************************ */
