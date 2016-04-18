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

// Declaration
#include "cece/core/AlignedAllocator.hpp"

// C++
#include <cassert>
#include <cstdlib>

#ifdef _WIN32
#include <malloc.h>
#endif

/* ************************************************************************ */

#if defined(__MINGW32__) && !defined(__MINGW64_VERSION_MAJOR)
#define _aligned_malloc __mingw_aligned_malloc
#define _aligned_free  __mingw_aligned_free
#endif

/* ************************************************************************ */

namespace cece {
inline namespace core {
namespace memory {

/* ************************************************************************ */

bool is_power_of_two(std::size_t x) noexcept
{
    size_t powerOfTwo = 1ul;

    while (powerOfTwo < x && powerOfTwo < 2147483648ul)
        powerOfTwo *= 2;

    return (x == powerOfTwo);
}

/* ************************************************************************ */

void* allocate_aligned_memory(std::size_t align, std::size_t size)
{
    assert(align >= sizeof(void*));
    assert(is_power_of_two(align));

    if (size == 0)
        return nullptr;

#ifdef _WIN32
    void* ptr = _aligned_malloc(size, align);
#else
    void* ptr = nullptr;
    int rc = posix_memalign(&ptr, align, size);

    if (rc != 0)
        return nullptr;
#endif

    return ptr;
}

/* ************************************************************************ */

void deallocate_aligned_memory(void* ptr) noexcept
{
#ifdef _WIN32
    _aligned_free(ptr);
#else
    free(ptr);
#endif
}

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */
