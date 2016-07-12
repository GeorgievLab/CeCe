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

// CeCe config
#include "cece/config.hpp"

/* ************************************************************************ */

// CeCe
#include "cece/object/Object.hpp"

#ifdef CECE_ENABLE_RENDER
#  include "cece/render/State.hpp"
#  include "cece/render/Object.hpp"
#  include "cece/render/Circle.hpp"
#endif

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace obstacle {

/* ************************************************************************ */

/**
 * @brief Circle obstacle.
 */
class Circle : public object::Object
{

// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param simulation Simulation object reference.
     */
    explicit Circle(simulator::Simulation& simulation, String typeName = "obstacle.Circle",
                   object::Object::Type type = object::Object::Type::Static) noexcept
        : object::Object(simulation, typeName, object::Object::Type::Static)
    {
        // Nothing to do
    }


// Public Operations
public:


    /**
     * @brief Configure obstacle.
     *
     * @param config
     * @param simulation
     */
    void configure(const config::Configuration& config, simulator::Simulation& simulation) override;


#ifdef CECE_ENABLE_RENDER

    /**
     * @brief Render obstacle.
     *
     * @param context Rendering context.
     */
    void draw(render::Context& context) override;


    /**
     * @brief Store current state for drawing.
     * State should be stored in back state because the front state is
     * used for rendering.
     * Drawing state should contain data that can be modified during update()
     * call and are used for rendering.
     */
    void drawStoreState() override;


    /**
     * @brief Swap render state.
     * Calling this function should be guarded by mutex for all modules
     * to ensure all modules are in same render state.
     * Function should be fast because otherwise it will block rendering.
     */
    void drawSwapState() override;

#endif

// Private Structures
private:

#ifdef CECE_ENABLE_RENDER
    struct RenderState
    {
        units::PositionVector position;
        units::Length radius;
        render::Color color;
    };
#endif

// Private Data Members
private:

#ifdef CECE_ENABLE_RENDER

    /// Shared object for drawing rectangle.
    render::ObjectSharedPtr<render::Circle> m_drawCircle;

    /// Render state.
    render::State<RenderState> m_drawableState;

#endif
};

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */

