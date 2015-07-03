
#pragma once

/* ************************************************************************ */

// C++
#include <cmath>

// Simulator
#include "core/compatibility.hpp"
#include "core/Units.hpp"
#include "core/Map.hpp"
#include "core/String.hpp"
#include "simulator/Object.hpp"

#ifdef ENABLE_RENDER
#include "render/Context.hpp"
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
    using GrowthRate = typename units::Delta<units::Volume, units::Time>::type;


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
    units::Volume getVolume() const NOEXCEPT
    {
        return m_volume;
    }


    /**
     * @brief Returns cell growth rate.
     *
     * @return
     */
    GrowthRate getGrowthRate() const NOEXCEPT
    {
        return m_growthRate;
    }


    /**
     * @brief Returns a number moulecules of given name.
     *
     * @param name Molecule name.
     *
     * @return Number of molecules.
     */
    MoleculeCount getMoleculeCount(const String& name) const NOEXCEPT
    {
        auto it = m_molecules.find(name);
        return it != m_molecules.end() ? it->second : MoleculeCount();
    }


// Public Mutators
public:


    /**
     * @brief Set cell volume.
     *
     * @param volume
     */
    void setVolume(units::Volume volume) NOEXCEPT
    {
        m_volume = volume;
    }


    /**
     * @brief Set cell growth rate.
     *
     * @param rate
     */
    void setGrowthRate(GrowthRate rate) NOEXCEPT
    {
        m_growthRate = rate;
    }


    /**
     * @brief Set a number of molecules of given name.
     *
     * @param name  Molecule name.
     * @param count Number of molecules.
     */
    void setMoleculeCount(const String& name, MoleculeCount count) NOEXCEPT
    {
        m_molecules[name] = count;
    }


    /**
     * @brief Add molecules of given name.
     *
     * @param name Molecule name.
     * @param diff Number of molecules.
     */
    void changeMoleculeCount(const String& name, MoleculeCountDifference diff) NOEXCEPT
    {
        // Get mutable reference
        auto& value = m_molecules[name];

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
    void addMolecules(const String& name, MoleculeCount count) NOEXCEPT
    {
        changeMoleculeCount(name, count);
    }


    /**
     * @brief Remove molecules of given name.
     *
     * @param name  Molecule name.
     * @param count Number of molecules to remove.
     */
    void removeMolecules(const String& name, MoleculeCount count) NOEXCEPT
    {
        changeMoleculeCount(name, -count);
    }


// Public Operations
public:


    /**
     * @brief Configure object.
     *
     * @param config
     * @param simulation
     */
    void configure(const simulator::Configuration& config,
                   simulator::Simulation& simulation) override;


    /**
     * @brief Calculate radius for sphere shapes - from cell volume.
     *
     * @param volume Cell volume.
     *
     * @return Radius.
     */
    static units::Length calcSphereRadius(units::Volume volume) NOEXCEPT
    {
        // 3th root of ((3 / 4 * pi) * volume)
        return units::Length(0.62035f * std::pow(volume.value(), 0.3333333f));
    }


// Private Data Members
private:

    /// Cell volume.
    units::Volume m_volume = units::um3(100);

    /// Cell growth rate.
    GrowthRate m_growthRate = GrowthRate(1e-3);

    /// Map of molecules.
    Map<String, MoleculeCount> m_molecules;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */

