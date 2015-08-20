/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/* Author: Jiří Fatka <fatkaj@ntis.zcu.cz>                                  */
/* ************************************************************************ */

// Declaration
#include "CellBase.hpp"

// Simulator
#include "core/Units.hpp"

/* ************************************************************************ */

namespace plugin {
namespace cell {

/* ************************************************************************ */

void CellBase::configure(const simulator::Configuration& config,
                         simulator::Simulation& simulation)
{
    Object::configure(config, simulation);

    // Initial volume
    setVolume(config.get("volume", getVolume()));

    // Maximum volume
    setVolumeMax(config.get("volume-max", getVolumeMax()));

    // Growth rate
    setGrowthRate(config.get("growth-rate", getGrowthRate()));

	// Molecule initializations
    for (auto&& cfg : config.getConfigurations("molecule"))
        setMoleculeCount(cfg.get("name"), cfg.get<MoleculeCount>("amount"));

#if ENABLE_RENDER
    // Identification color
    setIdentificationColor(config.get("identification-color", getIdentificationColor()));
#endif

#if ENABLE_RENDER
    setGfpSaturation(config.get("saturation-gfp", getGfpSaturation()));
#endif

#if ENABLE_RENDER
    setRfpSaturation(config.get("saturation-rfp", getRfpSaturation()));
#endif

#if ENABLE_RENDER
    setYfpSaturation(config.get("saturation-yfp", getYfpSaturation()));
#endif
}

/* ************************************************************************ */

void CellBase::update(units::Time dt)
{
    // Volume change
    const units::Volume dV = getGrowthRate() * (getVolumeMax() - getVolume()) * dt;

    // Update volume
    setVolume(getVolume() + dV);

    Object::update(dt);
}

/* ************************************************************************ */

#if ENABLE_RENDER
render::Color CellBase::calcFluorescentColor() const noexcept
{
    const auto volume = getVolume();
    const auto gfp = getMoleculeCount("GFP");
    const auto rfp = getMoleculeCount("RFP");
    const auto yfp = getMoleculeCount("YFP");

    const auto rfpValue = rfp / (getRfpSaturation() * volume);
    const auto gfpValue = gfp / (getGfpSaturation() * volume);
    const auto yfpValue = yfp / (getYfpSaturation() * volume);

    return {rfpValue + yfpValue, gfpValue + yfpValue, 0, 1};
}
#endif

/* ************************************************************************ */

}
}

/* ************************************************************************ */
