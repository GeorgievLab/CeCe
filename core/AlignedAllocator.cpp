
/* ************************************************************************ */

// Declaration
#include "AlignedAllocator.hpp"

/* ************************************************************************ */

namespace core {

/* ************************************************************************ */

bool is_power_of_two(size_t x) noexcept
{
    size_t powerOfTwo = 1ul;

    while (powerOfTwo < x && powerOfTwo < 2147483648ul)
        powerOfTwo *= 2;

    return (x == powerOfTwo);
}

/* ************************************************************************ */

void* detail::allocate_aligned_memory(size_t align, size_t size)
{
    assert(align >= sizeof(void*));
    assert(is_power_of_two(align));

    if (size == 0)
        return nullptr;

    void* ptr = nullptr;
    int rc = posix_memalign(&ptr, align, size);

    if (rc != 0)
        return nullptr;

    return ptr;
}

/* ************************************************************************ */

void detail::deallocate_aligned_memory(void *ptr) noexcept
{
    return free(ptr);
}

/* ************************************************************************ */

}

/* ************************************************************************ */
