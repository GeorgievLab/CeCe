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

// Declaration
#include "cece/module/Container.hpp"

// C++
#include <algorithm>

// CeCe
#include "cece/module/Module.hpp"

/* ************************************************************************ */

namespace cece {
namespace module {

/* ************************************************************************ */

void Container::init(AtomicBool& flag)
{
    // Update modules
    for (auto& module : getSortedListAsc())
    {
        if (!flag)
            break;

        module->init(flag);
    }
}

/* ************************************************************************ */

void Container::update()
{
    // Update modules
    for (auto& module : getSortedListAsc())
        module->update();
}

/* ************************************************************************ */

void Container::terminate()
{
    // Update modules
    for (auto& module : getSortedListDesc())
        module->terminate();
}

/* ************************************************************************ */

#ifdef CECE_ENABLE_RENDER
void Container::draw(const simulator::Visualization& visualization, render::Context& context)
{
    const RenderState& state = m_drawableState.getFront();

    // Copy list
    DynamicArray<ViewPtr<Module>> modules = state.modules;

    // Sort modules by rendering order
    std::sort(modules.begin(), modules.end(), [](const ViewPtr<Module>& lhs, const ViewPtr<Module>& rhs) {
        return lhs->getZOrder() < rhs->getZOrder();
    });

    // Render modules
    for (auto& module : modules)
        module->draw(visualization, context);
}
#endif

/* ************************************************************************ */

#ifdef CECE_ENABLE_RENDER
void Container::drawStoreState(const simulator::Visualization& visualization)
{
    RenderState& state = m_drawableState.getBack();
    state.modules.clear();

    for (auto& module : *this)
    {
        module->drawStoreState(visualization);
        // Store module to draw
        state.modules.push_back(module);
    }
}
#endif

/* ************************************************************************ */

#ifdef CECE_ENABLE_RENDER
void Container::drawSwapState()
{
    for (auto& module : *this)
        module->drawSwapState();

    m_drawableState.swap();
}
#endif

/* ************************************************************************ */

DynamicArray<ViewPtr<Module>>
Container::getSortedListAsc() const noexcept
{
    DynamicArray<ViewPtr<Module>> modules;

    // Copy modules (view pointer)
    for (const auto& module : *this)
        modules.push_back(module);

    // Sort modules by priority. Cannot be precomputed, because priority can change in previous iteration
    std::sort(modules.begin(), modules.end(),
        [](const ViewPtr<Module>& lhs, const ViewPtr<Module>& rhs) {
            return lhs->getPriority() > rhs->getPriority();
    });

    return modules;
}

/* ************************************************************************ */

DynamicArray<ViewPtr<Module>>
Container::getSortedListDesc() const noexcept
{
    DynamicArray<ViewPtr<Module>> modules;

    // Copy modules (view pointer)
    for (const auto& module : *this)
        modules.push_back(module);

    // Sort modules by priority. Cannot be precomputed, because priority can change in previous iteration
    std::sort(modules.begin(), modules.end(),
        [](const ViewPtr<Module>& lhs, const ViewPtr<Module>& rhs) {
            return lhs->getPriority() < rhs->getPriority();
    });

    return modules;
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */
