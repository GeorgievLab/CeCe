/* ************************************************************************ */
/* Georgiev Lab (c) 2016                                                    */
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
#include "Image.hpp"

#ifdef CECE_CLI_ENABLE_IMAGE_CAPTURE
#include <png.h>
#endif

// CeCe
#include "cece/core/FileStream.hpp"
#include "cece/core/Exception.hpp"

/* ************************************************************************ */

namespace cece {
namespace cli {

/* ************************************************************************ */

#ifdef CECE_CLI_ENABLE_IMAGE_CAPTURE
void storeImage(const FilePath& filename, DynamicArray<unsigned char> data,
    unsigned int width, unsigned int height)
{
    OutFileStream file(filename.string(), OutFileStream::binary);

    if (!file.is_open())
        throw InvalidArgumentException("Cannot open output file: " + filename.string());

    // Create write struct
    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);

    if (!png)
        throw RuntimeException("Internal PNG error");

    // Create info struct
    png_infop info = png_create_info_struct(png);

    if (!info)
    {
        png_destroy_write_struct(&png, nullptr);
        throw RuntimeException("Internal PNG error");
    }

    // Set error function. In case of error, program jumps here
    if (setjmp(png_jmpbuf(png)))
    {
        png_destroy_write_struct(&png, &info);
        throw RuntimeException("Internal PNG error");
    }

    // Set write/flush function
    png_set_write_fn(png, &file, [](png_structp png, png_bytep buf, png_size_t size) noexcept {
        OutFileStream* file = reinterpret_cast<OutFileStream*>(png_get_io_ptr(png));
        file->write(reinterpret_cast<const char*>(buf), size);
    }, [](png_structp png) noexcept {
        OutFileStream* file = reinterpret_cast<OutFileStream*>(png_get_io_ptr(png));
        file->flush();
    });

    // Set info
    png_set_IHDR(png, info, width, height,
        8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE
    );

    // Write header
    png_write_info(png, info);

    // Allocate memory for row pointers. It stores pointer to part of data buffer.
    DynamicArray<png_bytep> rowPtrs(height);

    // Row size
    const auto rowSize = png_get_rowbytes(png, info);

    for (png_int_32 y = 0; y < height; y++)
        rowPtrs[height - y - 1] = data.data() + y * rowSize;

    // Write data
    png_write_image(png, rowPtrs.data());

    // Write end info
    png_write_end(png, nullptr);
}
#endif

/* ************************************************************************ */

}
}

/* ************************************************************************ */
