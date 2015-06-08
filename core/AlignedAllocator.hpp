
#pragma once

/* ************************************************************************ */

/**
 * @see http://stackoverflow.com/a/12942652
 */

/* ************************************************************************ */

// C++
#include <cstddef>
#include <memory>
#include <type_traits>

// Simulator
#include "core/compatibility.hpp"

/* ************************************************************************ */

#ifndef _MSC_VER
inline namespace core {
#endif

/* ************************************************************************ */

namespace memory {

/* ************************************************************************ */

/**
 * @brief Allocate aligned memory.
 *
 * @param align Memory alignment.
 * @param size  Number of required bytes.
 */
void* DLL_EXPORT allocate_aligned_memory(std::size_t align, std::size_t size);

/* ************************************************************************ */

/**
 * @brief Deallocate memory allocated by allocate_aligned_memory.
 *
 * @param ptr
 */
void DLL_EXPORT deallocate_aligned_memory(void* ptr) NOEXCEPT;

/* ************************************************************************ */

}

/* ************************************************************************ */

/**
 * @brief Aligned allocator.
 *
 * @tparam T
 * @tparam Align
 */
#ifdef _MSC_VER
template <typename T, std::size_t Align = ((__alignof(T) > sizeof(void*)) ? __alignof(T) : sizeof(void*))>
#else
template <typename T, std::size_t Align = ((alignof(T) > sizeof(void*)) ? alignof(T) : sizeof(void*))>
#endif
class AlignedAllocator;

/* ************************************************************************ */

/**
 * @brief Aligned allocator - specialization for void.
 *
 * @tparam Align
 */
template<std::size_t Align>
class AlignedAllocator<void, Align>
{

// Public Types
public:

    using pointer = void*;
    using const_pointer = const void*;
    using value_type = void;

    template <class U> struct rebind { typedef AlignedAllocator<U, Align> other; };
};


/* ************************************************************************ */

/**
 * @brief Aligned allocator.
 *
 * @tparam T
 * @tparam Align
 */
template <typename T, std::size_t Align>
class AlignedAllocator
{

// Public Types
public:

    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using size_type = size_t;
    using difference_type = ptrdiff_t;

    using propagate_on_container_move_assignment = std::true_type;

    template <class U>
    struct rebind { typedef AlignedAllocator<U, Align> other; };


/// Public Ctors & Dtors
public:


    /**
     * @brief Default constructor.
     */
    AlignedAllocator() = default;


    /**
     * @brief Constructor.
     *
     * @param src
     */
    template <class U>
    AlignedAllocator(const AlignedAllocator<U, Align>& src) NOEXCEPT
    {
        // Nothing to do
    }


/// Public Accessors
public:


    /**
     * @brief Returns maximum size.
     *
     * @return
     */
    size_type max_size() const NOEXCEPT
    {
        return (size_type(~0) - size_type(Align)) / sizeof(T);
    }


/// Public Operations
public:


    /**
     * @brief Returns address of the given reference.
     *
     * @param x
     *
     * @return
     */
    pointer address(reference x) const NOEXCEPT
    {
        return std::addressof(x);
    }


    /**
     * @brief Returns address of the given reference.
     *
     * @param x
     *
     * @return
     */
    const_pointer address(const_reference x) const NOEXCEPT
    {
        return std::addressof(x);
    }


    /**
     * @brief Allocate aligned memory.
     *
     * @param n Number of required elements.
     *
     * @return
     */
    pointer allocate(size_type n, typename AlignedAllocator<void, Align>::const_pointer = 0)
    {
        void* ptr = memory::allocate_aligned_memory(Align, n * sizeof(T));

        if (ptr == nullptr)
            throw std::bad_alloc();

        return reinterpret_cast<pointer>(ptr);
    }


    /**
     * @brief Free allocated memory.
     *
     * @param p
     */
    void deallocate(pointer p, size_type) NOEXCEPT
    {
        return memory::deallocate_aligned_memory(p);
    }


    /**
     * @brief Construct object in place.
     *
     * @param p
     * @param args
     */
    template <class U, class ...Args>
    void construct(U* p, Args&&... args)
    {
        ::new(reinterpret_cast<void*>(p)) U(std::forward<Args>(args)...);
    }


    /**
     * @brief Destroy object in the given memory.
     *
     * @param p Pointer to object.
     */
#ifdef _MSC_VER
    void destroy(pointer p)
#else
    void destroy(pointer p) noexcept(noexcept(p->~T()))
#endif
    {
        p->~T();
    }
};

/* ************************************************************************ */

/**
 * @brief Compare allocators.
 *
 * @param t The first allocator.
 * @param u The second allocator.
 *
 * @return
 */
template <typename T, std::size_t TAlign, typename U, std::size_t UAlign>
inline bool operator==(const AlignedAllocator<T, TAlign>& t, const AlignedAllocator<U, UAlign>& u) NOEXCEPT
{
    return TAlign == UAlign;
}

/* ************************************************************************ */

/**
 * @brief Compare allocators.
 *
 * @param t The first allocator.
 * @param u The second allocator.
 *
 * @return
 */
template <typename T, std::size_t TAlign, typename U, std::size_t UAlign>
inline bool operator!=(const AlignedAllocator<T, TAlign>& t, const AlignedAllocator<U, UAlign>& u) NOEXCEPT
{
    return operator!=(t, u);
}

/* ************************************************************************ */

#ifndef _MSC_VER
}
#endif

/* ************************************************************************ */
