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
#include "cece/module/Container.hpp"

// C++
#include <algorithm>

// CeCe
#include "cece/module/Module.hpp"

/* ************************************************************************ */

namespace cece {
namespace module {

/* ************************************************************************ */

namespace {

/* ************************************************************************ */

/**
 * @brief Find parameter in container.
 *
 * @param data
 *
 * @return
 */
template<typename Container>
auto find(Container& data, StringView name) noexcept -> decltype(&(data.begin()->second))
{
    auto it = std::find_if(data.begin(), data.end(),
        [name](const Pair<String, UniquePtr<Module>>& p) {
            return p.first == name;
        }
    );

    return it != data.end() ? &(it->second) : nullptr;
}

/* ************************************************************************ */

}

/* ************************************************************************ */

Container::~Container() = default;

/* ************************************************************************ */

bool Container::exists(StringView name) const noexcept
{
    return find(m_modules, name) != nullptr;
}

/* ************************************************************************ */

ViewPtr<Module> Container::get(StringView name) const noexcept
{
    auto ptr = find(m_modules, name);

    if (ptr)
        return *ptr;

    return nullptr;
}

/* ************************************************************************ */

ViewPtr<Module> Container::add(String name, UniquePtr<Module> module)
{
    auto ptr = find(m_modules, name);

    if (ptr)
    {
        *ptr = std::move(module);
        return *ptr;
    }
    else
    {
        m_modules.emplace_back(std::move(name), std::move(module));
        return m_modules.back().second;
    }
}

/* ************************************************************************ */

void Container::clear()
{
    m_modules.clear();
}

/* ************************************************************************ */

void Container::update(simulator::Simulation& simulation, units::Time dt)
{
    DynamicArray<ViewPtr<Module>> modules;

    // Copy modules (view pointer)
    for (const auto& module : m_modules)
        modules.push_back(module.second);

    // Sort modules by priority. Cannot be precomputed, because priority can change in previous iteration
    std::sort(modules.begin(), modules.end(),
        [](const ViewPtr<Module>& lhs, const ViewPtr<Module>& rhs) {
            return lhs->getPriority() > rhs->getPriority();
    });

    // Update modules
    for (auto& module : modules)
        module->update(simulation, dt);
}

/* ************************************************************************ */

#ifdef CECE_ENABLE_RENDER
void Container::draw(const simulator::Simulation& simulation, render::Context& context)
{
    DynamicArray<ViewPtr<Module>> modules;

    // Copy modules (view pointer)
    for (const auto& module : m_modules)
        modules.push_back(module.second);

    // Sort modules by rendering order
    std::sort(modules.begin(), modules.end(), [](const ViewPtr<Module>& lhs, const ViewPtr<Module>& rhs) {
        return lhs->getZOrder() < rhs->getZOrder();
    });

    // Render modules
    for (auto& module : modules)
        module->draw(simulation, context);
}
#endif

/* ************************************************************************ */

}
}

/* ************************************************************************ */
