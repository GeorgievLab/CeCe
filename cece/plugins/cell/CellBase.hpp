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

// C++
#include <cmath>

// CeCe
#include "cece/core/constants.hpp"
#include "cece/core/Units.hpp"
#include "cece/core/UnitsCtors.hpp"
#include "cece/core/Map.hpp"
#include "cece/core/String.hpp"
#include "cece/core/StringView.hpp"
#include "cece/object/Object.hpp"

#ifdef CECE_ENABLE_RENDER
#include "cece/render/Color.hpp"
#endif

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace cell {

/* ************************************************************************ */

/**
 * @brief Base class for cells.
 */
class CellBase : public object::Object
{


// Public Types
public:


    /// Type for string number of molecules.
    using MoleculeCount = unsigned int;

    /// Signed version of molecule count.
    using MoleculeCountDifference = typename std::make_signed<MoleculeCount>::type;

    /// Cell growth rate type.
    using GrowthRate = units::Inverse<units::Time>::type;

#ifdef CECE_ENABLE_RENDER
    /// Type of fluorescent saturation.
    using FluorescentSaturation = units::Inverse<units::Volume>::type;
#endif


// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param simulation
     * @param typeName
     * @param type
     */
    explicit CellBase(simulator::Simulation& simulation, String typeName = "cell.CellBase",
        object::Object::Type type = object::Object::Type::Dynamic)
        : object::Object(simulation, std::move(typeName), type)
    {
        // Nothing to do
    }


// Public Accessors
public:


    /**
     * @brief Returns cell volume.
     *
     * @return
     */
    units::Volume getVolume() const noexcept
    {
        return m_volume;
    }


    /**
     * @brief Returns cell maximum volume.
     *
     * @return
     */
    units::Volume getVolumeMax() const noexcept
    {
        return m_volumeMax;
    }


    /**
     * @brief Returns cell growth rate.
     *
     * @return
     */
    GrowthRate getGrowthRate() const noexcept
    {
        return m_growthRate;
    }


    /**
     * @brief Returns cell radius.
     *
     * @note Only for cells with circle shape.
     *
     * @return
     */
    units::Length getRadius() const
    {
        return getShapes()[0].getCircle().radius;
    }


    /**
     * @brief Returns map of molecules.
     *
     * @return
     */
    const Map<String, MoleculeCount>& getMolecules() const noexcept
    {
        return m_molecules;
    }


    /**
     * @brief Returns a number moulecules of given name.
     *
     * @param name Molecule name.
     *
     * @return Number of molecules.
     */
    MoleculeCount getMoleculeCount(const StringView& name) const noexcept
    {
        auto it = m_molecules.find(name.getData());
        return it != m_molecules.end() ? it->second : MoleculeCount();
    }


#ifdef CECE_ENABLE_RENDER

    /**
     * @brief Returns cell identification color.
     *
     * @return
     */
    const render::Color& getIdentificationColor() const noexcept
    {
        return m_identificationColor;
    }


    /**
     * @brief Returns GFP saturation.
     *
     * @return
     */
    FluorescentSaturation getGfpSaturation() const noexcept
    {
        return m_gfpSaturation;
    }


    /**
     * @brief Returns RFP saturation.
     *
     * @return
     */
    FluorescentSaturation getRfpSaturation() const noexcept
    {
        return m_rfpSaturation;
    }


    /**
     * @brief Returns YFP saturation.
     *
     * @return
     */
    FluorescentSaturation getYfpSaturation() const noexcept
    {
        return m_yfpSaturation;
    }


    /**
     * @brief Returns CFP saturation.
     *
     * @return
     */
    FluorescentSaturation getCfpSaturation() const noexcept
    {
        return m_cfpSaturation;
    }


    /**
     * @brief Returns BFP saturation.
     *
     * @return
     */
    FluorescentSaturation getBfpSaturation() const noexcept
    {
        return m_bfpSaturation;
    }

#endif


// Public Mutators
public:


    /**
     * @brief Set cell volume.
     *
     * @param volume
     */
    void setVolume(units::Volume volume) noexcept
    {
        m_volume = volume;
    }


    /**
     * @brief Set cell maximum volume.
     *
     * @param volume
     */
    void setVolumeMax(units::Volume volume) noexcept
    {
        m_volumeMax = volume;
    }


    /**
     * @brief Set cell growth rate.
     *
     * @param rate
     */
    void setGrowthRate(GrowthRate rate) noexcept
    {
        m_growthRate = rate;
    }


    /**
     * @brief Set a number of molecules of given name.
     *
     * @param name  Molecule name.
     * @param count Number of molecules.
     */
    void setMoleculeCount(const StringView& name, MoleculeCount count) noexcept
    {
        m_molecules[name.getData()] = count;
    }


    /**
     * @brief Add molecules of given name.
     *
     * @param name Molecule name.
     * @param diff Number of molecules.
     */
    void changeMoleculeCount(const StringView& name, MoleculeCountDifference diff) noexcept
    {
        // Get mutable reference
        auto& value = m_molecules[name.getData()];

        // We need to compare signed versions.
        // If not, this condition is always true.
        if (MoleculeCountDifference(value) + diff < 0)
            value = MoleculeCount(0);
        else
            value += diff;
    }


    /**
     * @brief Add molecules of given name.
     *
     * @param name  Molecule name.
     * @param count Number of molecules to add.
     */
    void addMolecules(const StringView& name, MoleculeCount count) noexcept
    {
        changeMoleculeCount(name, count);
    }


    /**
     * @brief Remove molecules of given name.
     *
     * @param name  Molecule name.
     * @param count Number of molecules to remove.
     */
    void removeMolecules(const StringView& name, MoleculeCount count) noexcept
    {
        changeMoleculeCount(name, -count);
    }


#ifdef CECE_ENABLE_RENDER

    /**
     * @brief Set cell identification color.
     *
     * @param color New color.
     */
    void setIdentificationColor(render::Color color) noexcept
    {
        m_identificationColor = std::move(color);
    }


    /**
     * @brief Set GFP saturation.
     *
     * @param saturation
     */
    void setGfpSaturation(FluorescentSaturation saturation) noexcept
    {
        m_gfpSaturation = saturation;
    }


    /**
     * @brief Set RFP saturation.
     *
     * @param saturation
     */
    void setRfpSaturation(FluorescentSaturation saturation) noexcept
    {
        m_rfpSaturation = saturation;
    }


    /**
     * @brief Set YFP saturation.
     *
     * @param saturation
     */
    void setYfpSaturation(FluorescentSaturation saturation) noexcept
    {
        m_yfpSaturation = saturation;
    }


    /**
     * @brief Set CFP saturation.
     *
     * @param saturation
     */
    void setCfpSaturation(FluorescentSaturation saturation) noexcept
    {
        m_cfpSaturation = saturation;
    }


    /**
     * @brief Set BFP saturation.
     *
     * @param saturation
     */
    void setBfpSaturation(FluorescentSaturation saturation) noexcept
    {
        m_bfpSaturation = saturation;
    }

#endif


// Public Operations
public:


    /**
     * @brief Kill current cell.
     */
    void kill()
    {
        destroy();
    }


    /**
     * @brief Configure object.
     *
     * @param config
     * @param simulation
     */
    void configure(const config::Configuration& config, simulator::Simulation& simulation) override;


    /**
     * @brief Update cell state.
     *
     * @param dt Time step.
     */
    void update(units::Time dt) override;


    /**
     * @brief Calculate radius for sphere shapes - from cell volume.
     *
     * @param volume Cell volume.
     *
     * @return Radius.
     */
    static units::Length calcRadius(units::Volume volume) noexcept
    {
        // 3th root of ((3 / 4 * pi) * volume)
        //return units::Length(0.62035f * std::pow(volume.value(), 0.3333333f));
        // Simulation is in 2D
        return units::Length(std::sqrt(volume.value() / constants::PI));
    }


#ifdef CECE_ENABLE_RENDER

    /**
     * @brief Calculate color of fluorescent proteins.
     *
     * @param volume
     *
     * @return
     */
    render::Color calcFluorescentColor(units::Volume volume) const noexcept;


    /**
     * @brief Calculate color of fluorescent proteins.
     *
     * @return
     */
    render::Color calcFluorescentColor() const noexcept
    {
        return calcFluorescentColor(getVolume());
    }

#endif


// Private Data Members
private:

    /// Cell volume.
    units::Volume m_volume = units::um3(100);

    /// Cell maximum volume.
    units::Volume m_volumeMax = units::um3(100);

    /// Cell growth rate.
    GrowthRate m_growthRate = Zero;

    /// Map of molecules.
    Map<String, MoleculeCount> m_molecules;

#ifdef CECE_ENABLE_RENDER
    /// GFP saturation.
    FluorescentSaturation m_gfpSaturation{20};

    /// RFP saturation.
    FluorescentSaturation m_rfpSaturation{20};

    /// YFP saturation.
    FluorescentSaturation m_yfpSaturation{20};

    /// CFP saturation.
    FluorescentSaturation m_cfpSaturation{20};

    /// BFP saturation.
    FluorescentSaturation m_bfpSaturation{20};

    /// Cell identification color.
    render::Color m_identificationColor = render::colors::TRANSPARENT;
#endif

};

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */

