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

#ifdef CECE_ENABLE_RENDER
#  include "cece/render/Context.hpp"
#  include "cece/render/Object.hpp"
#  include "cece/plugins/cell/DrawableYeast.hpp"
#endif

#ifdef CECE_THREAD_SAFE
#  include "cece/core/Mutex.hpp"
#endif

// Plugin
#include "cece/plugins/cell/CellBase.hpp"

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace cell {

/* ************************************************************************ */

/**
 * @brief Yeast representation.
 */
class Yeast : public CellBase
{

// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param simulation
     * @param type       Cell type.
     */
    explicit Yeast(simulator::Simulation& simulation, String typeName = "cell.Yeast",
                   object::Object::Type type = object::Object::Type::Dynamic) noexcept;


    /**
     * @brief Destructor.
     */
    ~Yeast();


// Public Accessors
public:


    /**
     * @brief Returns if yeast has bud.
     *
     * @return
     */
    bool hasBud() const noexcept
    {
        return m_hasBud;
    }


    /**
     * @brief Return bud volume.
     *
     * @return
     */
    units::Volume getVolumeBud() const noexcept
    {
        if (hasBud())
            return m_bud.volume;
        else
            return Zero;
    }


    /**
     * @brief Return bud current angle.
     *
     * @return
     */
    units::Angle getAngleBud() const noexcept
    {
        if (hasBud())
            return m_bud.angle;
        else
            return Zero;
    }


    /**
     * @brief Return volume when buds are produced.
     *
     * @return
     */
    units::Volume getVolumeBudCreate() const noexcept
    {
        return m_volumeBudCreate;
    }


    /**
     * @brief Return volume (bud) when buds are released.
     *
     * @return
     */
    units::Volume getVolumeBudRelease() const noexcept
    {
        return m_volumeBudRelease;
    }


// Public Accessors
public:


    /**
     * @brief Set bud volume.
     *
     * @param volume
     */
    void setVolumeBud(units::Volume volume) noexcept
    {
        if (!hasBud() && volume != Zero)
            budCreate();

        m_bud.volume = std::move(volume);
    }


    /**
     * @brief Set bud angle.
     *
     * @param angle
     */
    void setAngleBud(units::Angle angle) noexcept
    {
        if (!hasBud() && angle != Zero)
            budCreate();

        m_bud.angle = std::move(angle);
    }


    /**
     * @brief Set volume when buds are produced.
     *
     * @param volume
     */
    void setVolumeBudCreate(units::Volume volume) noexcept
    {
        m_volumeBudCreate = std::move(volume);
    }


    /**
     * @brief Set volume (bud) when buds are released.
     *
     * @param volume
     */
    void setVolumeBudRelease(units::Volume volume) noexcept
    {
        m_volumeBudRelease = std::move(volume);
    }


// Public Operations
public:


    /**
     * @brief Update yeast state.
     *
     * @param dt Time step.
     */
    void update(units::Time dt) override;


    /**
     * @brief Configure object.
     *
     * @param config
     * @param simulation
     */
    void configure(const config::Configuration& config, simulator::Simulation& simulation) override;


    /**
     * @brief Create new yeast bud.
     */
    void budCreate();


    /**
     * @brief Release bud yeast.
     */
    void budRelease();


#ifdef CECE_ENABLE_RENDER

    /**
     * @brief Render yeast.
     *
     * @param context
     */
    virtual void draw(render::Context& context) override;

#endif


// Protected Operations
protected:


    /**
     * @brief Update yeast physics shape.
     */
    void updateShape();


// Private Data Members
private:


    /// If yeast has a bud.
    bool m_hasBud = false;


    /**
     * @brief Yeast bud.
     */
    struct
    {
        /// Angle.
        units::Angle angle = Zero;

        /// Bud volume.
        units::Volume volume = Zero;

        /// Last shape update radius.
        units::Length lastRadius = Zero;
    } m_bud;

    /// Volume that is needed to bud creation.
    units::Volume m_volumeBudCreate = units::um3(12);

    /// Volume (bud) that is needed to bud release.
    units::Volume m_volumeBudRelease = units::um3(10);

#ifdef CECE_ENABLE_RENDER
    /// Render object for Yeast.
    /// Shared between all instances, it's same for all instances.
    render::ObjectSharedPtr<DrawableYeast> m_renderObject;
#endif

    /// Last shape update radius.
    units::Length m_lastRadius = Zero;

    /// If shape must be updated.
    bool m_shapeForceUpdate = false;

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
