
#pragma once

/* ************************************************************************ */

#ifdef ENABLE_RENDER

/* ************************************************************************ */

// V8
#include <v8.h>

// Render
#include "render/Context.hpp"

/* ************************************************************************ */

namespace javascript {

/* ************************************************************************ */

/**
 * @brief Create template object for canvas.
 *
 * @return Template object.
 */
v8::Local<v8::ObjectTemplate> create_canvas_template();

/* ************************************************************************ */

/**
 * @brief Create wrapper object for canvas.
 *
 * @param context Render context.
 *
 * @return Wrapped object.
 */
v8::Local<v8::Object> create_canvas(render::Context& context);

/* ************************************************************************ */

}

/* ************************************************************************ */

#endif

/* ************************************************************************ */
