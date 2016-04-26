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
#include "cece/core/String.hpp"
#include "cece/core/StaticArray.hpp"
#include "cece/core/Parameters.hpp"

/* ************************************************************************ */

using namespace cece;

/* ************************************************************************ */

TEST(ParametersTest, ctorEmpty)
{
    Parameters params;
    EXPECT_EQ(0, params.getCount());
}

/* ************************************************************************ */

TEST(ParametersTest, ctorList)
{
    Parameters params{
        {"name1", "value1"},
        {"name2", "value2"}
    };
    EXPECT_EQ(2, params.getCount());
}

/* ************************************************************************ */

TEST(ParametersTest, read)
{
    const Parameters params{
        {"name1", "value1"},
        {"name2", "value2"},
        {"name3", "value3"}
    };
    EXPECT_EQ(3, params.getCount());

    EXPECT_EQ("value1", params["name1"]);
    EXPECT_EQ("value2", params["name2"]);
    EXPECT_EQ("value3", params["name3"]);

    EXPECT_THROW(params["name4"], MissingParameterException);
    EXPECT_THROW(params["name5"], MissingParameterException);
}

/* ************************************************************************ */

TEST(ParametersTest, get)
{
    const Parameters params{
        {"name1", "value1"},
        {"name2", "value2"}
    };
    EXPECT_EQ(2, params.getCount());

    EXPECT_EQ("value1", params.get("name1"));
    EXPECT_EQ("value2", params.get("name2"));
    EXPECT_EQ("value3", params.get("name3", "value3"));

    EXPECT_THROW(params["name3"], MissingParameterException);
    EXPECT_THROW(params["name4"], MissingParameterException);
    EXPECT_THROW(params["name5"], MissingParameterException);
}

/* ************************************************************************ */

TEST(ParametersTest, write)
{
    Parameters params;
    params["name1"] = "value1";
    params["name4"] = "value4";

    EXPECT_EQ("value1", params["name1"]);
    EXPECT_EQ("value4", params["name4"]);
    EXPECT_EQ(2, params.getCount());

    // Params is not-const, so params[key] will create an entry instead of throw.
    EXPECT_FALSE(params.exists("name2"));
    EXPECT_FALSE(params.exists("name3"));
}

/* ************************************************************************ */

TEST(ParametersTest, set)
{
    Parameters params;
    params.set("name1", "value1");
    params.set("name4", "value4");

    EXPECT_EQ("value1", params.get("name1"));
    EXPECT_EQ("value4", params.get("name4"));
    EXPECT_EQ(2, params.getCount());

    // Params is not-const, so params[key] will create an entry instead of throw.
    EXPECT_FALSE(params.exists("name2"));
    EXPECT_FALSE(params.exists("name3"));
}

/* ************************************************************************ */

TEST(ParametersTest, merge)
{
    const Parameters params1{
        {"name1", "value1"},
        {"name2", "value2"}
    };

    Parameters params2{
        {"name3", "value3"},
        {"name4", "value4"}
    };

    EXPECT_EQ(2, params1.getCount());
    EXPECT_EQ(2, params2.getCount());

    // Append parameters
    params2.append(params1);

    EXPECT_EQ(4, params2.getCount());
    EXPECT_EQ("value1", params2.get("name1"));
    EXPECT_EQ("value2", params2.get("name2"));
    EXPECT_EQ("value3", params2.get("name3"));
    EXPECT_EQ("value4", params2.get("name4"));
}

/* ************************************************************************ */

TEST(ParametersTest, iterate)
{
    const StaticArray<String, 2> names{{"name1", "name2"}};
    const StaticArray<String, 2> values{{"value1", "value2"}};

    const Parameters params{
        {"name1", "value1"},
        {"name2", "value2"}
    };

    ASSERT_EQ(2, params.getCount());

    // Names
    EXPECT_TRUE(std::equal(
        std::begin(params), std::end(params),
        std::begin(names),
        [](const Parameters::Record& item, const String& name) {
            return item.name == name;
        }));

    // Values
    EXPECT_TRUE(std::equal(
        std::begin(params), std::end(params),
        std::begin(values),
        [](const Parameters::Record& item, const String& value) {
            return item.value == value;
        }));
}

/* ************************************************************************ */
