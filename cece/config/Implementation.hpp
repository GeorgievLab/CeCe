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

// CeCe
#include "cece/core/String.hpp"
#include "cece/core/StringView.hpp"
#include "cece/core/UniquePtr.hpp"
#include "cece/core/DynamicArray.hpp"

/* ************************************************************************ */

namespace cece {
namespace config {

/* ************************************************************************ */

/**
 * @brief Configuration implementation.
 */
class Implementation
{

// Public Ctors & Dtors
public:


    /**
     * @brief Destructor.
     */
    virtual ~Implementation();


// Public Accessors
public:


    /**
     * @brief Check if value with given name exists.
     *
     * @param name Value name.
     *
     * @return
     */
    virtual bool has(StringView name) const noexcept = 0;


    /**
     * @brief Returns list of configuration names.
     *
     * @return
     */
    virtual DynamicArray<String> getNames() const noexcept = 0;


    /**
     * @brief Returns string value.
     *
     * @param name Value name.
     *
     * @return
     */
    virtual String get(StringView name) const noexcept = 0;


    /**
     * @brief Returns if content string is set.
     *
     * @return
     */
    virtual bool hasContent() const noexcept = 0;


    /**
     * @brief Returns content string.
     *
     * @return
     */
    virtual String getContent() const noexcept = 0;


    /**
     * @brief Returns if sub-configuration exists.
     *
     * @param name Sub-configuration name.
     *
     * @return
     */
    virtual bool hasSubs(StringView name) const noexcept = 0;


    /**
     * @brief Returns all sub-configuration with given name.
     *
     * @param name Sub-configuration name.
     *
     * @return
     */
    virtual DynamicArray<UniquePtr<Implementation>> getSubs(StringView name) const noexcept = 0;


// Public Mutators
public:


    /**
     * @brief Set string value.
     *
     * @param name  Value name.
     * @param value Value to store.
     *
     * @return
     */
    virtual void set(StringView name, StringView value) noexcept = 0;


    /**
     * @brief Store content.
     *
     * @param value Content.
     */
    virtual void setContent(StringView value) noexcept = 0;


    /**
     * @brief Create new sub-configuration.
     *
     * @param name Sub-configuration name.
     *
     * @return
     */
    virtual UniquePtr<Implementation> addSub(StringView name) noexcept = 0;


    /**
     * @brief Returns list of sub-configuration names.
     *
     * @return
     */
    virtual DynamicArray<String> getSubNames() const noexcept = 0;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
