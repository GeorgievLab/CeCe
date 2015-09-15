/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
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

// Simulator
#include "CellBase.hpp"
#if ENABLE_RENDER
#include "render/Context.hpp"
#include "render/Object.hpp"
#include "DrawableCell.hpp"
#endif

#if THREAD_SAFE
#include "core/Mutex.hpp"
#endif

/* ************************************************************************ */

namespace plugin {
namespace cell {

/* ************************************************************************ */

/**
 * @brief Cell representation.
 */
class Cell : public CellBase
{


// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param simulation
     * @param type       Cell type.
     */
    explicit Cell(simulator::Simulation& simulation,
                  simulator::Object::Type type = simulator::Object::Type::Dynamic) noexcept;


// Public Accessors
public:


    /**
     * @brief Return object class name.
     *
     * @return
     */
    StringView getClassName() const noexcept override
    {
        return "cell.Cell";
    }


    /**
     * @brief Returns cell radius.
     *
     * @return
     */
    units::Length getRadius() const noexcept
    {
        return calcSphereRadius(getVolume());
    }


// Public Operations
public:


    /**
     * @brief Update cell state.
     *
     * @param dt Time step.
     */
    void update(units::Duration dt) override;


    /**
     * @brief Configure object.
     *
     * @param config
     * @param simulation
     */
    void configure(const simulator::Configuration& config,
                   simulator::Simulation& simulation) override;


#if ENABLE_RENDER
    /**
     * @brief Render cell.
     *
     * @param context
     */
    void draw(render::Context& context) override;
#endif


// Private Data Members
private:

#if ENABLE_RENDER
    render::ObjectSharedPtr<DrawableCell> m_renderObject;
#endif

#if THREAD_SAFE
    /// Access mutex.
    mutable Mutex m_mutex;
#endif

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */

