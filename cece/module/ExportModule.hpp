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

#pragma once

/* ************************************************************************ */

// C++
#include <utility>

// CeCe
#include "cece/core/FilePath.hpp"
#include "cece/core/CsvFile.hpp"
#include "cece/module/Module.hpp"

/* ************************************************************************ */

namespace cece {
namespace module {

/* ************************************************************************ */

/**
 * @brief Helper module for exporting other module data.
 */
class ExportModule : public module::Module
{

// Public Ctors & Dtors
public:


    using module::Module::Module;


// Public Accessors
public:


    /**
     * @brief Returns path to output file.
     *
     * @return
     */
    const FilePath& getFilePath() const noexcept
    {
        return m_file.getPath();
    }


// Public Mutators
public:


    /**
     * @brief Set path to output file.
     *
     * @param filePath
     */
    void setFilePath(FilePath filePath) noexcept
    {
        m_file.setPath(std::move(filePath));
    }


// Public Operations
public:


    /**
     * @brief Load module configuration.
     *
     * @param config Source configuration.
     */
    void loadConfig(const config::Configuration& config) override;


    /**
     * @brief Store module configuration.
     *
     * @param config Destination configuration.
     */
    void storeConfig(config::Configuration& config) const override;


    /**
     * @brief Initialize module.
     */
    void init() override;


    /**
     * @brief Terminate module.
     */
    void terminate() override;


// Protected Operations
protected:


    /**
     * @brief Write CSV file header.
     *
     * @param args
     */
    template<typename... Args>
    void writeHeader(Args&&... args) noexcept
    {
        m_file.writeHeader(std::forward<Args>(args)...);
    }


    /**
     * @brief Write CSV record.
     *
     * @param args
     */
    template<typename... Args>
    void writeRecord(Args&&... args) noexcept
    {
        m_file.writeRecord(std::forward<Args>(args)...);
    }


// Protected Data Members
protected:

    // Output CSV file.
    CsvFile m_file;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
