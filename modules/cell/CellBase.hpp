
#pragma once

/* ************************************************************************ */

// C++
#include <cmath>

// Simulator
#include "core/compatibility.hpp"
#include "core/Units.hpp"
#include "simulator/Object.hpp"

#ifdef ENABLE_RENDER
#include "render/Context.hpp"
#endif

/* ************************************************************************ */

namespace module {
namespace cell {

/* ************************************************************************ */

/**
 * @brief Type for string number of fluorescent proteins.
 */
using FluorescentProteinCount = unsigned int;

/* ************************************************************************ */

/**
 * @brief Base class for cells.
 */
class CellBase : public simulator::Object
{


// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     */
#if _MSC_VER
	explicit CellBase(simulator::Simulation& simulation, simulator::Object::Type type = simulator::Object::Type::Static)
		: simulator::Object(simulation, type)
	{
		// Nothing to do
	}
#else
    using simulator::Object::Object;
#endif


// Public Accessors
public:


    /**
     * @brief Returns cell volume.
     *
     * @return
     */
    core::units::Volume getVolume() const NOEXCEPT
    {
        return m_volume;
    }


    /**
     * @brief Returns a number of GFP proteins.
     *
     * @return
     */
    FluorescentProteinCount getGfp() const NOEXCEPT
    {
        return m_gfp;
    }


    /**
     * @brief Returns a number of RFP proteins.
     *
     * @return
     */
    FluorescentProteinCount getRfp() const NOEXCEPT
    {
        return m_rfp;
    }


    /**
     * @brief Returns a number of YFP proteins.
     *
     * @return
     */
    FluorescentProteinCount getYfp() const NOEXCEPT
    {
        return m_yfp;
    }


// Public Mutators
public:


    /**
     * @brief Set cell volume.
     *
     * @param volume
     */
	void setVolume(core::units::Volume volume) NOEXCEPT
    {
        m_volume = volume;
    }


    /**
     * @brief Set a number of GFP proteins in the cell.
     *
     * @param gfp
     */
    void setGfp(FluorescentProteinCount gfp) NOEXCEPT
    {
        m_gfp = gfp;
    }


    /**
     * @brief Set a number of RFP proteins in the cell.
     *
     * @param rfp
     */
    void setRfp(FluorescentProteinCount rfp) NOEXCEPT
    {
        m_rfp = rfp;
    }


    /**
     * @brief Set a number of YFP proteins in the cell.
     *
     * @param yfp
     */
    void setYfp(FluorescentProteinCount yfp) NOEXCEPT
    {
        m_yfp = yfp;
    }


// Public Operations
public:


    /**
     * @brief Configure object.
     *
     * @param config
     * @param simulation
     */
    void configure(const simulator::ConfigurationBase& config,
                   simulator::Simulation& simulation) override;


    /**
     * @brief Calculate radius for sphere shapes - from cell volume.
     *
     * @param volume Cell volume.
     *
     * @return Radius.
     */
	static core::units::Length calcSphereRadius(core::units::Volume volume) NOEXCEPT
    {
        // 3th root of ((3 / 4 * pi) * volume)
		return core::units::Length(0.62035f * std::pow(volume, 0.3333333f));
    }


// Private Data Members
private:

    /// Cell volume.
	core::units::Volume m_volume = core::units::um(100);

    /// Number of GFP proteins.
    FluorescentProteinCount m_gfp = 0;

    /// Number of RFP proteins.
    FluorescentProteinCount m_rfp = 0;

    /// Number of YFP proteins.
    FluorescentProteinCount m_yfp = 0;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */

