/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/* Author: Jiří Fatka <fatkaj@ntis.zcu.cz>                                  */
/* ************************************************************************ */

// Declaration
#include "Module.hpp"

// C++
#include <cstdio>
#include <png.h>

// Simulation
#include "core/Log.hpp"
#include "core/UniquePtr.hpp"
#include "core/Exception.hpp"
#include "core/DynamicArray.hpp"
#include "simulator/Simulation.hpp"

/* ************************************************************************ */

namespace plugin {
namespace picture {

/* ************************************************************************ */

void Module::configure(const simulator::Configuration& config, simulator::Simulation& simulation)
{
    // File name pattern
    setFilePattern(config.get("pattern", getFilePattern()));

    // Save iteration
    setSaveIteration(config.get("iteration", getSaveIteration()));
}

/* ************************************************************************ */

void Module::update(units::Duration dt, simulator::Simulation& simulation)
{
    const auto stepNumber = simulation.getIteration();

    // Skip first image, because it's not rendered yet
    if (stepNumber <= 1)
        return;

    // Skip steps
    if (stepNumber % getSaveIteration() != 0)
        return;

    auto pattern = getFilePattern();
    auto pos = pattern.find("$1");

    // Replace placeholder
    if (pos != String::npos)
        pattern.replace(pos, 2, std::to_string(stepNumber));

    // Write image
    save(pattern);

    Log::info("Image saved: ", pattern);
}

/* ************************************************************************ */

void Module::draw(render::Context& context, const simulator::Simulation& simulation)
{
    // Skip first image, because it's not rendered yet
    if (simulation.getIteration() == 0)
        return;

    // Get image data
    m_image = context.getData();
}

/* ************************************************************************ */

void Module::save(const FilePath& filename)
{
    // Open C file
    auto file = makeUniqueCustom(
        fopen(filename.string().c_str(), "wb"),
        fclose
    );

    if (!file)
        throw RuntimeException("Could not open file '" + filename.string() + "' for writing");

    // Initialize write structure
    auto png = makeUniqueCustom(
        png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL),
        [] (png_structp ptr) { png_destroy_write_struct(&ptr, (png_infopp) NULL); }
    );

    if (!png)
        throw RuntimeException("Could not allocate write struct");

    // Initialize info structure
    auto info = makeUniqueCustom(
        png_create_info_struct(png.get()),
        [&png] (png_infop ptr) { png_free_data(png.get(), ptr, PNG_FREE_ALL, -1); }
    );

    if (!png)
        throw RuntimeException("Could not allocate info struct");

    // Setup Exception handling
    if (setjmp(png_jmpbuf(png.get())))
        throw RuntimeException("Error during png creation");

    const int width = m_image.size.getWidth();
    const int height = m_image.size.getHeight();

    // Initialize IO
    png_init_io(png.get(), file.get());

    // Write header (8 bit colour depth)
    png_set_IHDR(png.get(), info.get(), width, height, 8,
        PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    // Write info
    png_write_info(png.get(), info.get());

    static_assert(sizeof(png_byte) == sizeof(uint8_t), "Size mismatch");

    // Allocate memory for one row (3 bytes per pixel - RGB)
    DynamicArray<png_byte> row(3 * width * sizeof(png_byte));
    auto beg = m_image.data.begin();

    // Write image data
    for (int y = 0; y < height; y++)
    {
        // Copy data
        std::copy(beg, std::next(beg, row.size()), row.begin());

        // Write row
        png_write_row(png.get(), row.data());

        // Next row
        std::advance(beg, row.size());
    }

    // End write
    png_write_end(png.get(), NULL);
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */
