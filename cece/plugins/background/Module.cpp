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
#include "cece/plugins/background/Module.hpp"

// LibPNG
#include <png.h>

// CeCe
#include "cece/core/VectorRange.hpp"
#include "cece/core/InStream.hpp"
#include "cece/render/Color.hpp"
#include "cece/simulator/Simulation.hpp"

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace background {

/* ************************************************************************ */

constexpr png_size_t PNGSIGSIZE = 8;

/* ************************************************************************ */

void Module::loadConfig(const config::Configuration& config)
{
    // Configure parent
    module::Module::loadConfig(config);

    // Get image name
    setImageName(config.get("image"));
}

/* ************************************************************************ */

void Module::storeConfig(config::Configuration& config) const
{
    module::Module::storeConfig(config);

    config.set("image", getImageName());
}

/* ************************************************************************ */

void Module::init()
{
    // Get image resource
    auto file = getSimulation().getResource(getImageName());

    if (!file)
        throw RuntimeException("Image '" + getImageName() + "' not found");

    // Read and check header
    png_byte header[PNGSIGSIZE];
    file->read(reinterpret_cast<char*>(header), sizeof(header));
    if (png_sig_cmp(header, 0, PNGSIGSIZE))
        throw RuntimeException("Asset is not a PNG file");

    // Create read structure
    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);

    if (!png)
        throw RuntimeException("Internal PNG error");

    // Create info structure
    png_infop info = png_create_info_struct(png);

    if (!info)
    {
        png_destroy_read_struct(&png, nullptr, nullptr);
        throw RuntimeException("Internal PNG error");
    }

    // Set error function. In case of error, program jumps here
    if (setjmp(png_jmpbuf(png)))
    {
        png_destroy_read_struct(&png, &info, nullptr);
        throw RuntimeException("Internal PNG error");
    }

    // Set read function
    png_set_read_fn(png, file.get(), [](png_structp png, png_bytep buf, png_size_t size) noexcept {
        InStream* file = reinterpret_cast<InStream*>(png_get_io_ptr(png));
        file->read(reinterpret_cast<char*>(buf), size);
    });

    // Skip signature read that we already read
    png_set_sig_bytes(png, PNGSIGSIZE);

    // Read PNG info
    png_read_info(png, info);

    // Store image size
    m_size = Vector<unsigned int>(png_get_image_width(png, info), png_get_image_height(png, info));

    // Number of channels
    m_channels = png_get_channels(png, info);

    // Detect color type
    switch (png_get_color_type(png, info))
    {
        default:
            break;

        case PNG_COLOR_TYPE_PALETTE:
            png_set_palette_to_rgb(png);
            break;
    }

    // Alpha channel
    if (png_get_valid(png, info, PNG_INFO_tRNS))
    {
        png_set_tRNS_to_alpha(png);
    }

    // Read data
    if (setjmp(png_jmpbuf(png)))
    {
        png_destroy_read_struct(&png, &info, nullptr);
        throw RuntimeException("PNG reading error");
    }

    // Allocate memory for row pointers. It stores pointer to part of data buffer.
    DynamicArray<png_bytep> rowPtrs(m_size.getHeight());

    // Row size
    const auto rowSize = png_get_rowbytes(png, info);

    // Allocate memory for data
    m_data.resize(m_size.getHeight() * rowSize);

    for (png_int_32 y = m_size.getHeight(); y >= 0; y--)
        rowPtrs[y] = m_data.data() + y * rowSize;

    // Read image data
    png_read_image(png, rowPtrs.data());
}

/* ************************************************************************ */

void Module::draw(render::Context& context)
{
    if (!m_drawable)
    {
        m_drawable.create(context, m_size);

        // Copy data
        for (auto&& c : range(m_size))
        {
            render::Color pixel;

            // Pointer
            const auto ptr = m_data.data() + m_channels * (c.getY() * m_size.getWidth() + c.getX());

            switch (m_channels)
            {
            case 1:
                pixel = render::Color::fromUchar(ptr[0], ptr[0], ptr[0]);
                break;

            case 2:
                pixel = render::Color::fromUchar(ptr[0], ptr[0], ptr[0], ptr[1]);
                break;

            case 3:
                pixel = render::Color::fromUchar(ptr[0], ptr[1], ptr[2]);
                break;

            case 4:
                pixel = render::Color::fromUchar(ptr[0], ptr[1], ptr[2], ptr[3]);
                break;
            }

            m_drawable->set(c, pixel);
        }
    }

    // Draw color grid
    context.matrixPush();
    context.matrixScale(getSimulation().getWorldSize() / units::Length(1));
    context.colorPush();
    context.enableAlpha();
    m_drawable->draw(context);
    context.disableAlpha();
    context.colorPop();
    context.matrixPop();
}

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */
