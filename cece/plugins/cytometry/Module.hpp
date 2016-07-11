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

// CeCe
#include "cece/config.hpp"
#include "cece/core/Units.hpp"
#include "cece/core/VectorUnits.hpp"
#include "cece/core/Set.hpp"
#include "cece/module/ExportModule.hpp"

#ifdef CECE_ENABLE_RENDER
#  include "cece/render/Object.hpp"
#  include "cece/render/Color.hpp"
#  include "cece/render/Rectangle.hpp"
#endif

/* ************************************************************************ */

namespace cece { namespace config { class Configuration; } }

#ifdef CECE_ENABLE_RENDER
namespace cece { namespace object { class Object; } }
namespace cece { namespace render { class Context; } }
namespace cece { namespace simulator { class Visualization; } }
#endif

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace cytometry {

/* ************************************************************************ */

/**
 * @brief Module for cytometry measurements.
 */
class Module : public module::ExportModule
{

// Public Ctors & Dtors
public:


    using module::ExportModule::ExportModule;


// Public Accessors & Mutators
public:


    /**
     * @brief Returns measurement window name.
     *
     * @return
     */
    const String& getName() const noexcept
    {
        return m_name;
    }


    /**
     * @brief Set measurement window name.
     *
     * @param name Window name.
     */
    void setName(String name) noexcept
    {
        m_name = std::move(name);
    }


    /**
     * @brief Returns measurement area center position.
     *
     * @return
     */
    const units::PositionVector& getPosition() const noexcept
    {
        return m_position;
    }


    /**
     * @brief Set measurement area center position.
     *
     * @param position New position.
     */
    void setPosition(units::PositionVector position) noexcept
    {
        m_position = position;
    }


    /**
     * @brief Returns measurement area size.
     *
     * @return
     */
    const units::SizeVector& getSize() const noexcept
    {
        return m_size;
    }


    /**
     * @brief Set measurement area size.
     *
     * @param size New size.
     */
    void setSize(units::SizeVector size) noexcept
    {
        m_size = size;
    }


#ifdef CECE_ENABLE_RENDER
    /**
     * @brief Returns area color.
     *
     * @return
     */
    const render::Color& getColor() const noexcept
    {
        return m_color;
    }


    /**
     * @brief Set area color.
     *
     * @param color
     */
    void setColor(render::Color color) noexcept
    {
        m_color = color;
    }


    /**
     * @brief Returns layer name.
     *
     * @return
     */
    const String& getLayerName() const noexcept
    {
        return m_layerName;
    }


    /**
     * @brief Set layer name.
     *
     * @param name Layer name.
     */
    void setLayerName(String name) noexcept
    {
        m_layerName = std::move(name);
    }
#endif


// Public Operations
public:


    /**
     * @brief Load module configuration.
     *
     * @param config Source configuration.
     */
    void loadConfig(const config::Configuration& config) override;


    /**
     * @brief Store module configuration.
     *
     * @param config Output configuration.
     */
    void storeConfig(config::Configuration& config) const override;


    /**
     * @brief Initialize module.
     */
    void init() override;


    /**
     * @brief Update module state.
     */
    void update() override;


#ifdef CECE_ENABLE_RENDER

    /**
     * @brief Render module.
     *
     * @param visualization Visualization context.
     * @param context       Rendering context.
     */
    void draw(const simulator::Visualization& visualization, render::Context& context) override;

#endif


// Private Data Members
private:

    /// Measurement name.
    String m_name;

    /// Measurement area center position.
    units::PositionVector m_position = Zero;

    /// Measurement area size.
    units::SizeVector m_size = Zero;

    /// Last measured objects.
    Set<ViewPtr<object::Object>> m_objects;

#ifdef CECE_ENABLE_RENDER
    /// Renderable object.
    render::ObjectPtr<render::Rectangle> m_drawable;

    /// Rectangle color.
    render::Color m_color = render::colors::WHITE;

    /// Layer name.
    String m_layerName;
#endif
};

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */
