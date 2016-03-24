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
#include "cece/plugins/cell/CellBase.hpp"

// CeCe
#include "cece/core/Units.hpp"
#include "cece/config/Configuration.hpp"

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace cell {

/* ************************************************************************ */

void CellBase::configure(const config::Configuration& config, simulator::Simulation& simulation)
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

#ifdef CECE_ENABLE_RENDER
    // Identification color
    setIdentificationColor(config.get("identification-color", getIdentificationColor()));
    setGfpSaturation(config.get("saturation-gfp", getGfpSaturation()));
    setRfpSaturation(config.get("saturation-rfp", getRfpSaturation()));
    setYfpSaturation(config.get("saturation-yfp", getYfpSaturation()));
    setCfpSaturation(config.get("saturation-cfp", getCfpSaturation()));
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

#ifdef CECE_ENABLE_RENDER
render::Color CellBase::calcFluorescentColor(const units::Volume volume) const noexcept
{
    const auto gfp = getMoleculeCount("GFP");
    const auto rfp = getMoleculeCount("RFP");
    const auto yfp = getMoleculeCount("YFP");
    const auto cfp = getMoleculeCount("CFP");
    const auto bfp = getMoleculeCount("BFP");

    const auto rfpValue = rfp / (getRfpSaturation() * volume);
    const auto gfpValue = gfp / (getGfpSaturation() * volume);
    const auto yfpValue = yfp / (getYfpSaturation() * volume);
    const auto cfpValue = cfp / (getYfpSaturation() * volume);
    const auto bfpValue = bfp / (getYfpSaturation() * volume);

    const auto red   = rfpValue + yfpValue;
    const auto green = gfpValue + yfpValue + cfpValue;
    const auto blue  = cfpValue + bfpValue;

    return render::Color(red, green, blue, 1);
}
#endif

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */
