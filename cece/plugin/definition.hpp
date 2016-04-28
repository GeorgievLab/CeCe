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

// CeCe Config
#include "cece/config.hpp"

/* ************************************************************************ */

// CeCe
#include "cece/export.hpp"
#include "cece/plugin/Config.hpp"

/* ************************************************************************ */

#ifdef CECE_ENABLE_RENDER
#  define CECE_ENABLE_RENDER_VALUE 1
#else
#  define CECE_ENABLE_RENDER_VALUE 0
#endif

/* ************************************************************************ */

#ifdef CECE_ENABLE_BOX2D_PHYSICS
#  define CECE_ENABLE_BOX2D_PHYSICS_VALUE 1
#else
#  define CECE_ENABLE_BOX2D_PHYSICS_VALUE 0
#endif

/* ************************************************************************ */

#ifdef CECE_THREAD_SAFE
#  define CECE_THREAD_SAFE_VALUE 1
#else
#  define CECE_THREAD_SAFE_VALUE 0
#endif

/* ************************************************************************ */

/**
 * @brief Define function name for extern plugins.
 *
 * @param prefix Function prefix.
 * @param name   Plugin name.
 */
#define CECE_PLUGIN_PROTOTYPE_NAME_EXTERN(prefix, name) prefix

/* ************************************************************************ */

/**
 * @brief Define function name for builtin plugins.
 *
 * @param prefix Function prefix.
 * @param name   Plugin name.
 */
#define CECE_PLUGIN_PROTOTYPE_NAME_BUILTIN(prefix, name) prefix ## _ ## name

/* ************************************************************************ */

/**
 * @brief Function name.
 *
 * @param name Plugin name.
 */
#if CECE_PLUGIN_BUILTIN
#define CECE_PLUGIN_PROTOTYPE_NAME(prefix, name) CECE_PLUGIN_PROTOTYPE_NAME_BUILTIN(prefix, name)
#else
#define CECE_PLUGIN_PROTOTYPE_NAME(prefix, name) CECE_PLUGIN_PROTOTYPE_NAME_EXTERN(prefix, name)
#endif

/* ************************************************************************ */

/**
 * @brief Prototype of function for creating plugin API object.
 *
 * @param name Plugin name.
 */
#define CECE_PLUGIN_CREATE_PROTOTYPE(name) \
    extern "C" CECE_EXPORT cece::plugin::Api* CECE_PLUGIN_PROTOTYPE_NAME(create, name)()

/* ************************************************************************ */

/**
 * @brief Declare function for creating plugin API object.
 *
 * @param name Plugin name.
 */
#define CECE_DECLARE_PLUGIN_CREATE(name) CECE_PLUGIN_CREATE_PROTOTYPE(name)

/* ************************************************************************ */

/**
 * @brief Define function for creating plugin API object.
 *
 * @param name      Plugin name.
 * @param className Plugin API class name.
 */
#define CECE_DEFINE_PLUGIN_CREATE(name, className) \
    CECE_PLUGIN_CREATE_PROTOTYPE(name) \
    { \
        return new className{}; \
    }

/* ************************************************************************ */

/**
 * @brief Prototype of function for returning plugin configuration.
 *
 * @param name Plugin name.
 */
#define CECE_PLUGIN_GET_CONFIG_PROTOTYPE(name) \
    extern "C" CECE_EXPORT cece::plugin::Config* CECE_PLUGIN_PROTOTYPE_NAME(get_config, name)()

/* ************************************************************************ */

/**
 * @brief Declare function for returning plugin configuration.
 *
 * @param name Plugin name.
 */
#define CECE_DECLARE_GET_CONFIG_VERSION(name) CECE_PLUGIN_GET_CONFIG_PROTOTYPE(name)

/* ************************************************************************ */

/**
 * @brief Define function for returning plugin configuration.
 *
 * @param name Plugin name.
 */
#define CECE_DEFINE_GET_CONFIG_VERSION(name) \
    CECE_PLUGIN_GET_CONFIG_PROTOTYPE(name) \
    { \
        static cece::plugin::Config config = { \
            cece::config::PLUGIN_API_VERSION,   /* apiVersion */ \
            sizeof(cece::config::RealType),     /* realSize */ \
            CECE_ENABLE_RENDER_VALUE,           /* renderEnabled */ \
            CECE_ENABLE_BOX2D_PHYSICS_VALUE,    /* builtinPhysics */ \
            CECE_THREAD_SAFE_VALUE,             /* threadSafe */ \
            cece::config::DIMENSION             /* dimension */ \
        }; \
        return &config; \
    }

/* ************************************************************************ */

/**
 * @brief Declare plugin functions.
 *
 * @param name Plugin name.
 */
#define CECE_DECLARE_PLUGIN(name) \
    CECE_DECLARE_PLUGIN_CREATE(name); \
    CECE_DECLARE_GET_CONFIG_VERSION(name)

/* ************************************************************************ */

/**
 * @brief Define plugin functions.
 *
 * @param name      Plugin name.
 * @param className Plugin API class name.
 */
#define CECE_DEFINE_PLUGIN(name, className) \
    CECE_DEFINE_PLUGIN_CREATE(name, className) \
    CECE_DEFINE_GET_CONFIG_VERSION(name)

/* ************************************************************************ */
