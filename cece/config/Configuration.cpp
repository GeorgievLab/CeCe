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
#include "cece/config/Configuration.hpp"

// CeCe
#include "cece/config/MemoryImplementation.hpp"

/* ************************************************************************ */

namespace cece {
namespace config {

/* ************************************************************************ */

Configuration::Configuration(FilePath path) noexcept
    : m_impl(makeUnique<MemoryImplementation>())
    , m_filePath(std::move(path))
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
        res.emplace_back(std::move(ptr), m_filePath);

    return res;
}

/* ************************************************************************ */

FilePath Configuration::buildFilePath(const FilePath& filename) const noexcept
{
    // Remove filename
    auto sourceFile = getSourcePath();

    // Return file path
    return sourceFile.parent_path() / filename;
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
    Configuration config{getSourcePath()};
    config.copyFrom(*this);
    return config;
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */
