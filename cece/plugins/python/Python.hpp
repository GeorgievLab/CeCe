/* ************************************************************************ */
/* Georgiev Lab (c) 2015-2016                                               */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/*                                                                          */
/* This file is part of CeCe.                                               */
/*                                                                          */
/* CeCe is free software: you can redistribute it and/or modify             */
/* it under the terms of the GNU General Public License as published by     */
/* the Free Software Foundation, either version 3 of the License, or        */
/* (at your option) any later version.                                      */
/*                                                                          */
/* CeCe is distributed in the hope that it will be useful,                  */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of           */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            */
/* GNU General Public License for more details.                             */
/*                                                                          */
/* You should have received a copy of the GNU General Public License        */
/* along with CeCe.  If not, see <http://www.gnu.org/licenses/>.            */
/*                                                                          */
/* ************************************************************************ */

#pragma once

/* ************************************************************************ */

#if defined(__MINGW32__) || defined(__MINGW64__)
// Undefined reference to __impl_Py_InitModule4 on 64 bit MinGW compiler
#ifdef __MINGW64__
#define MS_WIN64
#endif
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
