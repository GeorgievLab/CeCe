
#pragma once

/* ************************************************************************ */

// C++
#include <vector>
#include <cstdint>

// wxWidgets
#include <wx/filename.h>

/* ************************************************************************ */

bool SavePNG(const wxFileName& filename, const std::vector<std::uint8_t>& data,
             unsigned width, unsigned height);

/* ************************************************************************ */
