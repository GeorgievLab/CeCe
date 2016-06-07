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

// CLI config
#include "config.hpp"

/* ************************************************************************ */

// CeCe
#include "cece/core/DynamicArray.hpp"
#include "cece/core/FilePath.hpp"

/* ************************************************************************ */

namespace cece {
namespace cli {

/* ************************************************************************ */

#ifdef CECE_CLI_ENABLE_IMAGE_CAPTURE
/**
 * @brief Save image.
 *
 * @param filename Path to image file.
 * @param data     Image data.
 * @param width    Image width.
 * @param height   Image height.
 */
void storeImage(const FilePath& filename, DynamicArray<unsigned char> data,
    unsigned int width, unsigned int height);
#endif

/* ************************************************************************ */

}
}

/* ************************************************************************ */
