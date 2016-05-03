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

// Plugin config
#include "cece/plugins/streamlines/config.hpp"

/* ************************************************************************ */

// CeCe
#include "cece/core/Vector.hpp"
#include "cece/core/Grid.hpp"

// Plugin
#include "cece/plugins/streamlines/Node.hpp"

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace streamlines {

/* ************************************************************************ */

class Dynamics;

/* ************************************************************************ */

/**
 * @brief Implementation of Lattice Boltzman.
 *
 * Memory and speedup improvement taken from OpenLB TR1:
 * @link http://optilb.com/openlb/wp-content/uploads/2011/12/olb-tr1.pdf
 */
class Lattice
{


// Public Types
public:


    /**
     * @brief Container type.
     */
    using ContainerType = core::Grid<Node>;


    /**
     * @brief Size type.
     */
    using SizeType = ContainerType::SizeType;


    /**
     * @brief Coordinates type.
     */
    using CoordinateType = ContainerType::CoordinateType;


    /**
     * @brief Size type.
     */
    using Size = Vector<SizeType>;


// Public Operators
public:


    /**
     * @brief Array operator.
     *
     * @param coord
     *
     * @return
     */
    Node& operator[](const CoordinateType& coord) noexcept
    {
        return get(coord);
    }


    /**
     * @brief Array operator.
     *
     * @param coord
     *
     * @return
     */
    const Node& operator[](const CoordinateType& coord) const noexcept
    {
        return get(coord);
    }


// Public Accessors
public:


    /**
     * @brief Returns lattice size.
     *
     * @return
     */
    Size getSize() const noexcept
    {
        return m_data.getSize();
    }


    /**
     * @brief Check if coordinates are in range.
     *
     * @param coord
     *
     * @return
     */
    bool inRange(const CoordinateType& coord) const noexcept
    {
        return coord.inRange(CoordinateType{Zero}, getSize());
    }


    /**
     * @brief Get item from front buffer.
     *
     * @param coord Item coordinate.
     *
     * @return
     */
    Node& get(const CoordinateType& coord) noexcept
    {
        return m_data[coord];
    }


    /**
     * @brief Get item from front buffer.
     *
     * @param coord Item coordinate.
     *
     * @return
     */
    const Node& get(const CoordinateType& coord) const noexcept
    {
        return m_data[coord];
    }


#ifndef CECE_PLUGIN_streamlines_SWAP_TRICK
    /**
     * @brief Get item from back buffer.
     *
     * @param coord Item coordinate.
     *
     * @return
     */
    Node& getBack(const CoordinateType& coord) noexcept
    {
        return m_dataBack[coord];
    }
#endif


#ifndef CECE_PLUGIN_streamlines_SWAP_TRICK
    /**
     * @brief Get item from back buffer.
     *
     * @param coord Item coordinate.
     *
     * @return
     */
    const Node& getBack(const CoordinateType& coord) const noexcept
    {
        return m_dataBack[coord];
    }
#endif


// Public Mutators
public:


    /**
     * @brief Resize grid.
     *
     * @param size
     */
    void setSize(Size size)
    {
        m_data.resize(size);
#ifndef CECE_PLUGIN_streamlines_SWAP_TRICK
        m_dataBack.resize(size);
#endif
    }


// Public Operations
public:


    /**
     * @brief Init lattice equilibrium.
     */
    void initEquilibrium();


    /**
     * @brief Compute lattice collisions.
     */
    void collide();


    /**
     * @brief Steam lattice data.
     */
    void stream();


    /**
     * @brief Call collide and stream functions.
     *
     * @param omega
     */
    void collideAndStream();


    /**
     * @brief Set dynamics for all cells.
     *
     * @param dynamics
     */
    void setDynamics(ViewPtr<Dynamics> dynamics);


    /**
     * @brief Fixup inner obstacle types.
     *
     * @param dynamics
     */
    void fixupObstacles(ViewPtr<Dynamics> dynamics) noexcept;


// Private Data Members
public:

    /// Current lattice data.
    core::Grid<Node> m_data;

#ifndef CECE_PLUGIN_streamlines_SWAP_TRICK
    /// Temporaty lattice data.
    core::Grid<Node> m_dataBack;
#endif
};

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */
