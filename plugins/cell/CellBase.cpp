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

#if ENABLE_RENDER
    setCfpSaturation(config.get("saturation-cfp", getCfpSaturation()));
#endif

#if ENABLE_RENDER && CONFIG_PLUGIN_cell_BFP_COLOR
    setBfpSaturation(config.get("saturation-bfp", getBfpSaturation()));
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
render::Color CellBase::calcFluorescentColor(const units::Volume volume) const noexcept
{
    const auto gfp = getMoleculeCount("GFP");
    const auto rfp = getMoleculeCount("RFP");
    const auto yfp = getMoleculeCount("YFP");
    const auto cfp = getMoleculeCount("CFP");
#if CONFIG_PLUGIN_cell_BFP_COLOR
    const auto bfp = getMoleculeCount("BFP");
#endif

    const auto rfpValue = rfp / (getRfpSaturation() * volume);
    const auto gfpValue = gfp / (getGfpSaturation() * volume);
    const auto yfpValue = yfp / (getYfpSaturation() * volume);
    const auto cfpValue = cfp / (getYfpSaturation() * volume);
#if CONFIG_PLUGIN_cell_BFP_COLOR
    const auto bfpValue = bfp / (getYfpSaturation() * volume);
#endif

    const auto red   = rfpValue + yfpValue;
    const auto green = gfpValue + yfpValue + cfpValue;
#if CONFIG_PLUGIN_cell_BFP_COLOR
    const auto blue  = cfpValue + bfpValue;
#else
    const auto blue  = cfpValue;
#endif

    return {red, green, blue, 1};
}
#endif

/* ************************************************************************ */

}
}

/* ************************************************************************ */
