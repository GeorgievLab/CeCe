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

/**
 * @see http://stackoverflow.com/a/12942652
 */

/* ************************************************************************ */

// C++
#include <cstddef>
#include <memory>
#include <type_traits>

/* ************************************************************************ */

namespace cece {
inline namespace core {
namespace memory {

/* ************************************************************************ */

/**
 * @brief Allocate aligned memory.
 *
 * @param align Memory alignment.
 * @param size  Number of required bytes.
 */
void* allocate_aligned_memory(std::size_t align, std::size_t size);

/* ************************************************************************ */

/**
 * @brief Deallocate memory allocated by allocate_aligned_memory.
 *
 * @param ptr
 */
void deallocate_aligned_memory(void* ptr) noexcept;

/* ************************************************************************ */

}

/* ************************************************************************ */

/**
 * @brief Aligned allocator.
 *
 * @tparam T
 * @tparam Align
 */
template <typename T, std::size_t Align = ((alignof(T) > sizeof(void*)) ? alignof(T) : sizeof(void*))>
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
    AlignedAllocator(const AlignedAllocator<U, Align>& src) noexcept
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
    size_type max_size() const noexcept
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
    pointer address(reference x) const noexcept
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
    const_pointer address(const_reference x) const noexcept
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
    void deallocate(pointer p, size_type) noexcept
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
    void destroy(pointer p) noexcept(noexcept(p->~T()))
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
inline bool operator==(const AlignedAllocator<T, TAlign>& t, const AlignedAllocator<U, UAlign>& u) noexcept
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
inline bool operator!=(const AlignedAllocator<T, TAlign>& t, const AlignedAllocator<U, UAlign>& u) noexcept
{
    return operator!=(t, u);
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */
