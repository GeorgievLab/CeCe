/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */

// Declaration
#include "AlignedAllocator.hpp"

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

/* ************************************************************************ */
