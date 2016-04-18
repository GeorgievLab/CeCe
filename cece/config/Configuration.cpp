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
#include "cece/config/Configuration.hpp"

// C++
#include <algorithm>

// CeCe
#include "cece/core/Parameters.hpp"
#include "cece/config/MemoryImplementation.hpp"

/* ************************************************************************ */

namespace cece {
namespace config {

/* ************************************************************************ */

Configuration::Configuration(ViewPtr<Parameters> parameters) noexcept
    : m_impl(makeUnique<MemoryImplementation>())
    , m_parameters(parameters)
{
    // Nothign to do
}

/* ************************************************************************ */

DynamicArray<Configuration> Configuration::getConfigurations(StringView name) const noexcept
{
    if (!hasConfiguration(name))
        return {};

    DynamicArray<Configuration> res;

    for (auto&& ptr : m_impl->getSubs(name))
        res.emplace_back(std::move(ptr), m_parameters);

    return res;
}

/* ************************************************************************ */

void Configuration::copyFrom(const Configuration& config)
{
    for (const auto& name : config.getNames())
        set(name, config.get(name));

    setContent(config.getContent());

    // Copy subconfigurations
    for (const auto& name : config.getConfigurationNames())
    {
        for (auto&& cfg : config.getConfigurations(name))
            addConfiguration(name).copyFrom(cfg);
    }
}

/* ************************************************************************ */

Configuration Configuration::toMemory() const
{
    Configuration config;
    config.copyFrom(*this);
    return config;
}

/* ************************************************************************ */

String Configuration::replaceParameters(String str) const
{
    if (!m_parameters)
        return str;

    String::size_type start = 0;

    // Find parameter begin part
    while ((start = str.find("{$")) != String::npos)
    {
        // {$var}
        // 012345

        // Find ending character
        const auto end = str.find('}', start + 2);

        if (end == String::npos)
            throw InvalidArgumentException("Missing closing parameter character '}' in '" + str + "'");

        // Copy name
        const String name = str.substr(start + 2, (end - start + 1) - 3);

        // Check name characters
        const auto valid = std::find_if(std::begin(name), std::end(name), [] (String::value_type c) {
            return !isalnum(c);
        }) == name.end();

        if (!valid)
            throw InvalidArgumentException("Parameter name '" + name + "' contains invalid characters");

        // Try to find parameter
        const auto value = m_parameters->get(name);

        // Replace parameter with value
        str.replace(start, end - start + 1, value);
    }

    return str;
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */
