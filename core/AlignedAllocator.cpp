
/* ************************************************************************ */

// Declaration
#include "AlignedAllocator.hpp"

// C++
#include <cassert>
#include <cstdlib>

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

    void* ptr = nullptr;
    int rc = posix_memalign(&ptr, align, size);

    if (rc != 0)
        return nullptr;

    return ptr;
}

/* ************************************************************************ */

void deallocate_aligned_memory(void* ptr) noexcept
{
    return free(ptr);
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */
