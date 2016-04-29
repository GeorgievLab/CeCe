/* ************************************************************************ */
/* Georgiev Lab (c) 2016                                                    */
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

// CeCe
#include "cece/core/Real.hpp"

// Plugins
#include "cece/plugins/streamlines/Descriptor.hpp"

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace streamlines_channel {

/* ************************************************************************ */

/**
 * @brief Utility class.
 */
class Descriptor : public streamlines::Descriptor
{

// Public Accessors
public:


    /**
     * @brief Returns horizontal weights.
     *
     * @return
     */
    static const StaticArray<RealType, SIZE>& getWeightsHorizontal() noexcept
    {
        return s_weightsHorizontal;
    }


    /**
     * @brief Returns vertical weights.
     *
     * @return
     */
    static const StaticArray<RealType, SIZE>& getWeightsVertical() noexcept
    {
        return s_weightsVertical;
    }


    /**
     * @brief Returns horizontal weight.
     *
     * @param iPop Direction index.
     *
     * @return
     */
    static RealType getWeightHorizontal(DirectionType iPop) noexcept
    {
        return s_weightsHorizontal[iPop];
    }


    /**
     * @brief Returns vertical weight.
     *
     * @param iPop Direction index.
     *
     * @return
     */
    static RealType getWeightVertical(DirectionType iPop) noexcept
    {
        return s_weightsVertical[iPop];
    }


    /**
     * @brief Returns horizontal - vertical split coefficient.
     *
     * Value should be identical with `getWeightHorizontalSum`.
     *
     * @return
     */
    static RealType getSplitCoefficient() noexcept
    {
        return s_splitCoefficient;
    }


    /**
     * @brief Returns summation of horizontal weights.
     *
     * @return
     */
    static RealType getWeightHorizontalSum() noexcept;


    /**
     * @brief Returns summation of vertical weights.
     *
     * @return
     */
    static RealType getWeightVerticalSum() noexcept;


    /**
     * @brief Returns summation of horizontal weights.
     *
     * @param indices A list of required weights.
     *
     * @return
     */
    template<std::size_t N>
    static RealType getWeightHorizontalSum(StaticArray<DirectionType, N> indices) noexcept
    {
        RealType sum = 0.0;

        for (auto i : indices)
            sum += s_weightsHorizontal[i];

        return sum;
    }


    /**
     * @brief Returns summation of vertical weights.
     *
     * @param indices A list of required weights.
     *
     * @return
     */
    template<std::size_t N>
    static RealType getWeightVerticalSum(StaticArray<DirectionType, N> indices) noexcept
    {
        RealType sum = 0.0;

        for (auto i : indices)
            sum += s_weightsVertical[i];

        return sum;
    }


// Public Operations
public:


    /**
     * @brief Initialize model.
     *
     * @param height Channel height.
     */
    static void initModel(RealType height);


// Private Data Members
private:

    /// Split coefficient.
    static RealType s_splitCoefficient;

    /// Model horizontal weights.
    static StaticArray<RealType, SIZE> s_weightsHorizontal;

    /// Model vertical weights.
    static StaticArray<RealType, SIZE> s_weightsVertical;

};

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */
