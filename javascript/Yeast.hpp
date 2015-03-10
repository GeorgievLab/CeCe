
#pragma once

/* ************************************************************************ */

// V8
#include <v8.h>

/* ************************************************************************ */

namespace javascript {

/* ************************************************************************ */

/**
 * @brief Create template for yeast object.
 *
 * @param Template.
 */
v8::Local<v8::ObjectTemplate> create_yeast_template();

/* ************************************************************************ */

/**
 * @brief Create template for yeast bud object.
 *
 * @param Template.
 */
v8::Local<v8::ObjectTemplate> create_yeast_bud_template();

/* ************************************************************************ */

/**
 * @brief Create a yeast object.
 */
v8::Handle<v8::Value> create_yeast(const v8::Arguments& args);

/* ************************************************************************ */

}

/* ************************************************************************ */
