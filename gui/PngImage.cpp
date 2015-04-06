
/* ************************************************************************ */

// Declaration
#include "PngImage.h"

// C++
#include <memory>
#include <algorithm>

#ifdef PNG_SUPPORT
#include <png.h>
#endif

/* ************************************************************************ */

#ifdef PNG_SUPPORT
bool SavePNG(const wxFileName& filename, const std::vector<std::uint8_t>& data,
             unsigned width, unsigned height)
{
    auto fp_clean = [](FILE* f) {
        fclose(f);
    };

    // Open file for writing (binary mode)
    std::unique_ptr<FILE, decltype(fp_clean)> fp(
        fopen(filename.GetFullPath().c_str(), "wb"),
        fp_clean
    );

    if (!fp)
      return false;

    auto png_ptr_clean = [](png_struct* ptr) {
        png_destroy_write_struct(&ptr, (png_infopp) NULL);
    };

    // Initialize write structure
    std::unique_ptr<png_struct, decltype(png_ptr_clean)> png_ptr(
        png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL),
        png_ptr_clean
    );

    if (!png_ptr)
        return false;

    auto info_ptr_clean = [&png_ptr](png_info* info) {
        png_free_data(png_ptr.get(), info, PNG_FREE_ALL, -1);
    };

    // Initialize info structure
    std::unique_ptr<png_info, decltype(info_ptr_clean)> info_ptr(
        png_create_info_struct(png_ptr.get()),
        info_ptr_clean
    );

    if (!info_ptr)
        return false;

   // Setup Exception handling
   if (setjmp(png_jmpbuf(png_ptr.get())))
       return false;

    // Initialize output
    png_init_io(png_ptr.get(), fp.get());

    // Write header (8 bit colour depth)
    png_set_IHDR(png_ptr.get(), info_ptr.get(), width, height,
         8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
         PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    // Write info
    png_write_info(png_ptr.get(), info_ptr.get());

    // Allocate memory for one row (3 bytes per pixel - RGB)
    std::vector<png_byte> row(3 * width);

    // Write image data
    for (auto start = data.begin(); start < data.end(); std::advance(start, row.size()))
    {
        // Copy row info buffer
        std::copy(start, std::next(start, row.size()), row.begin());

        // Write row
        png_write_row(png_ptr.get(), row.data());
    }

    // End write
    png_write_end(png_ptr.get(), NULL);

    return true;
}
#endif

/* ************************************************************************ */
