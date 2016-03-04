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

#pragma once

/* ************************************************************************ */

// CeCe
#include "cece/core/String.hpp"
#include "cece/core/FilePath.hpp"
#include "cece/core/DynamicArray.hpp"
#include "cece/module/Module.hpp"
#include "cece/render/Context.hpp"

#ifdef CECE_THREAD_SAFE
#  include "cece/core/Mutex.hpp"
#endif

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace picture {

/* ************************************************************************ */

/**
 * @brief Module for storing pictures.
 */
class Module : public module::Module
{

// Public Ctors & Dtors
public:


    using module::Module::Module;


// Public Accessors
public:


    /**
     * @brief Return pattern for generated file name.
     *
     * @return
     */
    const String& getFilePattern() const noexcept
    {
        return m_filePattern;
    }


    /**
     * @brief Get number of iteration when image should be saved.
     *
     * @return
     */
    unsigned int getSaveIteration() const noexcept
    {
        return m_saveIteration;
    }


// Public Mutators
public:


    /**
     * @brief Set file name pattern. Currently is supported only with
     * placeholder $1 that is replaced by step number.
     *
     * @param fileName Pattern to destination file.
     */
    void setFilePattern(String fileName) noexcept
    {
        m_filePattern = std::move(fileName);
    }


    /**
     * @brief Set number of step when image should be saved.
     *
     * @param iteration
     */
    void setSaveIteration(unsigned int iteration) noexcept
    {
        m_saveIteration = iteration;
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
     * @param config Output configuration.
     */
    void storeConfig(config::Configuration& config) const override;


    /**
     * @brief Update module state.
     */
    void update() override;


    /**
     * @brief Render module.
     *
     * @param context Rendering context.
     */
    void draw(render::Context& context) override;


// Protected Operations
protected:


    /**
     * @brief Save image.
     */
    void save(const FilePath& filename);


// Private Data Members
private:


    /// Pattern for file name.
    String m_filePattern = "image_$1.png";

    /// Save image each n-th step.
    unsigned int m_saveIteration = 1;

    /// Image data.
    DynamicArray<unsigned char> m_data;

    /// Image size.
    Vector<unsigned int> m_size = Zero;

    /// If alpha channel should be stored too.
    bool m_alpha = false;

#ifdef CECE_THREAD_SAFE
    /// Access mutex.
    mutable Mutex m_mutex;
#endif

};

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */
