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

// C++
#include <cstdint>

// GTest
#include <gtest/gtest.h>

// CeCe
#include "cece/core/AlignedAllocator.hpp"

/* ************************************************************************ */

using namespace cece;
using namespace cece::core::memory;

/* ************************************************************************ */

TEST(AlignedAllocatorTest, byte8)
{
    for (int i = 0; i < 10000; ++i)
    {
        void* ptr = allocate_aligned_memory(8, 100);
        EXPECT_EQ(0, reinterpret_cast<uintptr_t>(ptr) & 0x7);
        deallocate_aligned_memory(ptr);
    }
}

/* ************************************************************************ */

TEST(AlignedAllocatorTest, byte16)
{
    for (int i = 0; i < 10000; ++i)
    {
        void* ptr = allocate_aligned_memory(16, 100);
        EXPECT_EQ(0, reinterpret_cast<uintptr_t>(ptr) & 0xF);
        deallocate_aligned_memory(ptr);
    }
}

/* ************************************************************************ */

TEST(AlignedAllocatorTest, byte32)
{
    for (int i = 0; i < 10000; ++i)
    {
        void* ptr = allocate_aligned_memory(32, 100);
        EXPECT_EQ(0, reinterpret_cast<uintptr_t>(ptr) & 0x1F);
        deallocate_aligned_memory(ptr);
    }
}

/* ************************************************************************ */

TEST(AlignedAllocatorTest, allocator)
{
    AlignedAllocator<int, 32> allocator;

    for (int i = 0; i < 10000; ++i)
    {
        auto ptr = allocator.allocate(10);
        EXPECT_EQ(0, reinterpret_cast<uintptr_t>(ptr) & 0x1F);
        allocator.deallocate(ptr, 10);
    }
}

/* ************************************************************************ */
