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
#include "Module.hpp"

// C++
#include <cstdio>

// LibPNG
#include <png.h>

// CeCe
#include "cece/core/Log.hpp"
#include "cece/core/UniquePtr.hpp"
#include "cece/core/Exception.hpp"
#include "cece/core/FileStream.hpp"
#include "cece/simulator/Simulation.hpp"
#include "cece/render/ImageData.hpp"

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace picture {

/* ************************************************************************ */

void Module::loadConfig(const config::Configuration& config)
{
    // Configure parent
    module::Module::loadConfig(config);

    // File name pattern
    setFilePattern(config.get("pattern", getFilePattern()));

    // Save iteration
    setSaveIteration(config.get("iteration", getSaveIteration()));

    // Store alpha channel.
    m_alpha = config.get("alpha", m_alpha);
}

/* ************************************************************************ */

void Module::storeConfig(config::Configuration& config) const
{
    module::Module::storeConfig(config);

    config.set("pattern", getFilePattern());
    config.set("iteration", getSaveIteration());
    config.set("alpha", m_alpha);
}

/* ************************************************************************ */

void Module::update()
{
    if (m_size == Zero)
        return;

    const auto stepNumber = getSimulation().getIteration();

    // Skip steps
    if (stepNumber % getSaveIteration() != 0)
        return;

    auto pattern = getFilePattern();
    auto pos = pattern.find("$1");

    // Replace placeholder
    if (pos != String::npos)
        pattern.replace(pos, 2, std::to_string(stepNumber));

    FilePath filename;

// Use working directory on Mac OS X
#if __APPLE__ && __MACH__
    // Get working directory
    const char* dir = getenv("WORKING_DIR");

    // Add directory
    if (dir)
        filename /= dir;
#endif

    filename /= pattern;

    // Write image
    save(filename);

    Log::info("Image saved: ", pattern);
}

/* ************************************************************************ */

void Module::draw(render::Context& context)
{
    // Skip first image, because it's not rendered yet
    if (getSimulation().getIteration() <= 1)
        return;

#ifdef CECE_THREAD_SAFE
    // Lock access
    MutexGuard guard(m_mutex);
#endif

    m_size = context.getSize();
    m_data.resize(m_size.getHeight() * m_size.getWidth() * (m_alpha ? 4 : 3));

    // Get image data
    context.getData(m_data.data(), m_alpha);
}

/* ************************************************************************ */

void Module::save(const FilePath& filename)
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
    png_set_IHDR(png, info, m_size.getWidth(), m_size.getHeight(),
        8, m_alpha ? PNG_COLOR_TYPE_RGBA : PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE
    );

    // Write header
    png_write_info(png, info);

    // Allocate memory for row pointers. It stores pointer to part of data buffer.
    DynamicArray<png_bytep> rowPtrs(m_size.getHeight());

    // Row size
    const auto rowSize = png_get_rowbytes(png, info);

    for (png_int_32 y = m_size.getHeight(); y >= 0; y--)
        rowPtrs[y] = m_data.data() + y * rowSize;

    // Write data
    png_write_image(png, rowPtrs.data());

    // Write end info
    png_write_end(png, nullptr);
}

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */
