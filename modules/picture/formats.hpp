
#pragma once

#if !ENABLE_RENDER
#error Unable to build module without ENABLE_RENDER
#endif

/* ************************************************************************ */

// C++
#include <vector>
#include <string>
#include <cstdint>

// Simulator
#include "render/Context.hpp"

/* ************************************************************************ */

namespace module {
namespace picture {

/* ************************************************************************ */

/**
 * @brief Save rendering context into image file. Type of the image is
 * defined by extension.
 *
 * @param context
 * @param filename
 *
 * @return
 */
bool save_image(const render::Context& context, const std::string& filename);

/* ************************************************************************ */

#if PNG_SUPPORT
/**
 * @brief Save rendering context in PNG image file.
 *
 * @param context
 * @param filename
 *
 * @return
 */
bool save_png(const render::Context& context, const std::string& filename);
#endif

/* ************************************************************************ */

}
}

/* ************************************************************************ */
