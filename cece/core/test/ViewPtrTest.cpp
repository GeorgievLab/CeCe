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

// GTest
#include <gtest/gtest.h>

// CeCe
#include "cece/core/ViewPtr.hpp"

/* ************************************************************************ */

using namespace cece;

/* ************************************************************************ */

TEST(ViewPtrTest, ctorDefault)
{
    ViewPtr<int> ptr;
    EXPECT_EQ(nullptr, ptr);
    EXPECT_FALSE(ptr);
}

/* ************************************************************************ */

TEST(ViewPtrTest, ctorRaw)
{
    int val = 5;

    ViewPtr<int> ptr(&val);
    ASSERT_NE(nullptr, ptr);
    EXPECT_TRUE(ptr);
    EXPECT_EQ(&val, ptr);
    EXPECT_EQ(5, *ptr);
}

/* ************************************************************************ */

TEST(ViewPtrTest, ctorCopy)
{
    int val = 5;
    ViewPtr<int> ptr(&val);

    ViewPtr<int> ptrCopy(ptr);
    EXPECT_NE(nullptr, ptrCopy);
    EXPECT_EQ(ptr, ptrCopy);
}

/* ************************************************************************ */

TEST(ViewPtrTest, ctorCopyChild)
{
    struct A {};
    struct B : public A {};

    B val;
    ViewPtr<B> ptr(&val);

    ViewPtr<A> ptrCopy(ptr);
    EXPECT_NE(nullptr, ptrCopy);
    EXPECT_EQ(ptr, ptrCopy);
}

/* ************************************************************************ */

TEST(ViewPtrTest, ctorUnique)
{
    UniquePtr<int> pval{new int{5}};

    ViewPtr<int> ptr(pval);
    EXPECT_NE(nullptr, ptr);
    EXPECT_EQ(pval, ptr);
}

/* ************************************************************************ */

TEST(ViewPtrTest, ctorUniqueChild)
{
    struct A {};
    struct B : public A {};

    UniquePtr<B> pval{new B};

    ViewPtr<A> ptr(pval);
    EXPECT_NE(nullptr, ptr);
    EXPECT_EQ(pval, ptr);
}

/* ************************************************************************ */

TEST(ViewPtrTest, get)
{
    int val = 5;
    ViewPtr<int> ptr(&val);
    ASSERT_TRUE(ptr);
    EXPECT_EQ(5, *ptr.get());
}

/* ************************************************************************ */

TEST(ViewPtrTest, reset)
{
    int val1 = 5;
    int val2 = 3;
    ViewPtr<int> ptr(&val1);
    EXPECT_TRUE(ptr);
    EXPECT_EQ(&val1, ptr);

    ptr.reset(&val2);
    EXPECT_TRUE(ptr);
    EXPECT_EQ(&val2, ptr);

    ptr.reset();
    EXPECT_FALSE(ptr);
    EXPECT_EQ(nullptr, ptr);
}

/* ************************************************************************ */

TEST(ViewPtrTest, release)
{
    int val = 5;
    ViewPtr<int> ptr(&val);
    ASSERT_TRUE(ptr);
    ptr.release(); // No memory leak
    ASSERT_FALSE(ptr);
}

/* ************************************************************************ */

TEST(ViewPtrTest, access)
{
    struct A { int val; int value() const { return val; } };

    A val{5};

    ViewPtr<A> ptr(&val);
    ASSERT_NE(nullptr, ptr);
    EXPECT_EQ(5, ptr->val);
    EXPECT_EQ(5, ptr->value());
}

/* ************************************************************************ */
