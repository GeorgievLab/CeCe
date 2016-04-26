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
#include <algorithm>

// GTest
#include <gtest/gtest.h>

// CeCe
#include "cece/core/UniquePtr.hpp"
#include "cece/core/String.hpp"
#include "cece/core/Exception.hpp"
#include "cece/core/StaticArray.hpp"
#include "cece/core/PtrContainer.hpp"

/* ************************************************************************ */

using namespace cece;

/* ************************************************************************ */

TEST(PtrContainerTest, ctorEmpty)
{
    PtrContainer<int> data;
    EXPECT_EQ(0, data.getCount());
}

/* ************************************************************************ */

TEST(PtrContainerTest, read)
{
    PtrContainer<int> data;
    data.create(5);
    EXPECT_EQ(1, data.getCount());

    EXPECT_EQ(5, *data[0]);

    data.create(0);
    data.create(566);
    EXPECT_EQ(3, data.getCount());

    EXPECT_EQ(5, *data[0]);
    EXPECT_EQ(0, *data[1]);
    EXPECT_EQ(566, *data[2]);
}

/* ************************************************************************ */

TEST(PtrContainerTest, get)
{
    PtrContainer<int> data;
    data.create(0);
    data.create(3);
    EXPECT_EQ(2, data.getCount());

    EXPECT_EQ(0, *data.get(0));
    EXPECT_EQ(3, *data.get(1));
    EXPECT_THROW(data.get(2), OutOfRangeException);
}

/* ************************************************************************ */

TEST(PtrContainerTest, write)
{
    PtrContainer<int> data;
    data.add(makeUnique<int>(10));
    EXPECT_EQ(1, data.getCount());

    EXPECT_EQ(10, *data.get(0));
}

/* ************************************************************************ */

TEST(PtrContainerTest, remove)
{
    PtrContainer<int> data;
    auto ptr1 = data.create(1);
    auto ptr2 = data.create(2);
    auto ptr3 = data.create(3);

    EXPECT_EQ(1, *ptr1);
    EXPECT_EQ(2, *ptr2);
    EXPECT_EQ(3, *ptr3);
    EXPECT_EQ(3, data.getCount());

    data.remove(ptr2);
    data.remove(ptr1);

    EXPECT_EQ(1, data.getCount());
    EXPECT_EQ(*ptr3, *data.get(0));
    EXPECT_EQ(3, *data.get(0));
}

/* ************************************************************************ */

TEST(PtrContainerTest, clear)
{
    PtrContainer<int> data;
    data.create(1);
    data.create(2);
    data.create(3);
    EXPECT_EQ(3, data.getCount());

    data.clear();

    EXPECT_EQ(0, data.getCount());
}

/* ************************************************************************ */

TEST(PtrContainerTest, iterate)
{
    const StaticArray<String, 3> names{{"name1", "name2", "name3"}};

    PtrContainer<String> data;
    ASSERT_EQ(0, data.getCount());

    for (const auto& name : names)
        data.create(name);

    // Values
    EXPECT_TRUE(std::equal(
        std::begin(data), std::end(data),
        std::begin(names),
        [](const UniquePtr<String>& item, const String& name) {
            return *item == name;
        }));
}

/* ************************************************************************ */
