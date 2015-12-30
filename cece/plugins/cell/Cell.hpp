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

// CeCe config
#include "cece/config.hpp"

/* ************************************************************************ */

// CeCe
#include "cece/render/Context.hpp"
#include "cece/render/Object.hpp"

#ifdef CECE_THREAD_SAFE
#  include "cece/core/Mutex.hpp"
#endif

// Plugin
#include "cece/plugins/cell/CellBase.hpp"
#include "cece/plugins/cell/DrawableCell.hpp"

/* ************************************************************************ */

namespace cece {
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
     * @param typeName
     * @param type       Cell type.
     */
    explicit Cell(simulator::Simulation& simulation, String typeName = "cell.Cell",
                  object::Object::Type type = object::Object::Type::Dynamic) noexcept;


// Public Accessors
public:


    /**
     * @brief Returns cell radius.
     *
     * @return
     */
    units::Length getRadius() const noexcept
    {
        return calcRadius(getVolume());
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
    void configure(const config::Configuration& config, simulator::Simulation& simulation) override;


#ifdef CECE_ENABLE_RENDER

    /**
     * @brief Render cell.
     *
     * @param context
     */
    void draw(render::Context& context) override;

#endif


// Private Data Members
private:

#ifdef CECE_ENABLE_RENDER
    /// Cell drawable.
    render::ObjectSharedPtr<DrawableCell> m_renderObject;
#endif

#ifdef CECE_THREAD_SAFE
    /// Access mutex.
    mutable Mutex m_mutex;
#endif

};

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */

