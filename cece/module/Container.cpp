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
template<typename ContainerT>
auto find(ContainerT& data, StringView name) noexcept -> decltype(&(data.begin()->module))
{
    auto it = std::find_if(data.begin(), data.end(),
        [name](const Container::Record& p) {
            return p.name == name;
        }
    );

    return it != data.end() ? &(it->module) : nullptr;
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
        m_modules.emplace_back(Record{std::move(name), std::move(module)});
        return m_modules.back().module;
    }
}

/* ************************************************************************ */

void Container::clear()
{
    m_modules.clear();
}

/* ************************************************************************ */

void Container::init(AtomicBool& termFlag)
{
    // Update modules
    for (auto& module : getSortedListAsc())
    {
        if (termFlag)
            break;

        module->init(termFlag);
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
void Container::draw(render::Context& context)
{
    DynamicArray<ViewPtr<Module>> modules;

    // Copy modules (view pointer)
    for (const auto& module : m_modules)
        modules.push_back(module.module);

    // Sort modules by rendering order
    std::sort(modules.begin(), modules.end(), [](const ViewPtr<Module>& lhs, const ViewPtr<Module>& rhs) {
        return lhs->getZOrder() < rhs->getZOrder();
    });

    // Render modules
    for (auto& module : modules)
        module->draw(context);
}
#endif

/* ************************************************************************ */

DynamicArray<ViewPtr<Module>>
Container::getSortedListAsc() const noexcept
{
    DynamicArray<ViewPtr<Module>> modules;

    // Copy modules (view pointer)
    for (const auto& module : m_modules)
        modules.push_back(module.module);

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
    for (const auto& module : m_modules)
        modules.push_back(module.module);

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
