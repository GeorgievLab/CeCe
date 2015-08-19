/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/* Author: Jiří Fatka <fatkaj@ntis.zcu.cz>                                  */
/* ************************************************************************ */

#pragma once

/* ************************************************************************ */

// C++
#include <cmath>

// Simulator
#include "core/Units.hpp"
#include "core/Map.hpp"
#include "core/String.hpp"
#include "core/StringView.hpp"
#include "simulator/Object.hpp"

#if ENABLE_RENDER
#include "render/Color.hpp"
#endif

/* ************************************************************************ */

namespace plugin {
namespace cell {

/* ************************************************************************ */

/**
 * @brief Base class for cells.
 */
class CellBase : public simulator::Object
{


// Public Types
public:


    /// Type for string number of molecules.
    using MoleculeCount = unsigned int;

    /// Signed version of molecule count.
    using MoleculeCountDifference = typename std::make_signed<MoleculeCount>::type;

    /// Cell growth rate type.
    using GrowthRate = units::Unit<units::List<>, units::List<units::BaseTime>>;


// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param simulation
     * @param type
     */
    explicit CellBase(simulator::Simulation& simulation, simulator::Object::Type type = simulator::Object::Type::Dynamic)
        : simulator::Object(simulation, type)
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


#if ENABLE_RENDER
    /**
     * @brief Returns cell identification color.
     *
     * @return
     */
    const render::Color& getIdentificationColor() const noexcept
    {
        return m_identificationColor;
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


#if ENABLE_RENDER
    /**
     * @brief Set cell identification color.
     *
     * @param color New color.
     */
    void setIdentificationColor(render::Color color) noexcept
    {
        m_identificationColor = std::move(color);
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
    void configure(const simulator::Configuration& config,
                   simulator::Simulation& simulation) override;


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
    static units::Length calcSphereRadius(units::Volume volume) noexcept
    {
        // 3th root of ((3 / 4 * pi) * volume)
        return units::Length(0.62035f * std::pow(volume.value(), 0.3333333f));
    }


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

#if ENABLE_RENDER
    /// Cell identification color.
    render::Color m_identificationColor = render::colors::TRANSPARENT;
#endif
};

/* ************************************************************************ */

}
}

/* ************************************************************************ */

