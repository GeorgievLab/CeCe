/* ************************************************************************ */
/* Georgiev Lab (c) 2016                                                    */
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
#include "cece/plugins/streamlines-channel/Module.hpp"

// CeCe
#include "cece/core/Assert.hpp"
#include "cece/core/Log.hpp"
#include "cece/core/UnitIo.hpp"
#include "cece/config/Configuration.hpp"

// Plugin
#include "cece/plugins/streamlines-channel/Descriptor.hpp"
#include "cece/plugins/streamlines-channel/BgkDynamics.hpp"
#include "cece/plugins/streamlines-channel/ZouHeDynamics.hpp"

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace streamlines_channel {

/* ************************************************************************ */

void Module::init(AtomicBool& termFlag)
{
    // Initialize model
    Descriptor::initModel(convertLength(getHeight()));

    // Initialize streamlines
    streamlines::Module::init(termFlag);
}

/* ************************************************************************ */

void Module::loadConfig(const config::Configuration& config)
{
    // Configure parent
    streamlines::Module::loadConfig(config);

    // Channel height
    setHeight(config.get("height", getHeight()));
}

/* ************************************************************************ */

void Module::storeConfig(config::Configuration& config) const
{
    streamlines::Module::storeConfig(config);

    // Store channel height
    config.set("height", getHeight());
}

/* ************************************************************************ */

UniquePtr<streamlines::Dynamics> Module::createFluidDynamics() const
{
    return makeUnique<BgkDynamics>(calculateOmega());
}

/* ************************************************************************ */

UniquePtr<streamlines::Dynamics> Module::createBorderDynamics(LayoutPosition pos) const
{
    const auto omega = calculateOmega();

    switch (pos)
    {
    case LayoutPosTop:
        return makeUnique<ZouHeDynamics>(omega, ZouHeDynamics::Position::Top);

    case LayoutPosBottom:
        return makeUnique<ZouHeDynamics>(omega, ZouHeDynamics::Position::Bottom);

    case LayoutPosLeft:
        return makeUnique<ZouHeDynamics>(omega, ZouHeDynamics::Position::Left);

    case LayoutPosRight:
        return makeUnique<ZouHeDynamics>(omega, ZouHeDynamics::Position::Right);

    default:
        Assert(false && "No way!");
        break;
    }

    return nullptr;
}

/* ************************************************************************ */

void Module::printInfo()
{
    streamlines::Module::printInfo();

    Log::info("[streamlines] Channel height: ", getHeight(), " um");
    Log::info("[streamlines] Horizontal weights: ",
        Descriptor::getWeightHorizontal(0), ", ",
        Descriptor::getWeightHorizontal(2), ", ",
        Descriptor::getWeightHorizontal(1)
    );
    Log::info("[streamlines] Vertical weights: ",
        Descriptor::getWeightVertical(0), ", ",
        Descriptor::getWeightVertical(2), ", ",
        Descriptor::getWeightVertical(1)
    );
}

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */
