
#ifdef ENABLE_RENDER

/* ************************************************************************ */

#include <string>
#include <cstdio>

#ifdef JPEG_SUPPORT
// LibJPEG
#include <jpeglib.h>
#endif

// Declaration
#include "javascript/Canvas.hpp"

/* ************************************************************************ */

namespace javascript {

/* ************************************************************************ */

namespace {

/* ************************************************************************ */

#ifdef JPEG_SUPPORT
void write_jpeg_file(JSAMPLE* data, int width, int height, const std::string& filename, int quality)
{
    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;
    FILE* outfile;
    JSAMPROW row_pointer;

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);

    if ((outfile = fopen(filename.c_str(), "wb")) == NULL)
        v8::ThrowException(v8::String::New(("unable to open output file: " + filename).c_str()));

    jpeg_stdio_dest(&cinfo, outfile);

    cinfo.image_width = width;
    cinfo.image_height = height;
    cinfo.input_components = 3;
    cinfo.in_color_space = JCS_RGB;

    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo, quality, TRUE);

    // Start compressor
    jpeg_start_compress(&cinfo, TRUE);
    int row_stride = width * 3;

    while (cinfo.next_scanline < cinfo.image_height)
    {
        row_pointer = &data[cinfo.next_scanline * row_stride];
        (void) jpeg_write_scanlines(&cinfo, &row_pointer, 1);
    }

    jpeg_finish_compress(&cinfo);
    fclose(outfile);

    jpeg_destroy_compress(&cinfo);
}
#endif

/* ************************************************************************ */

/**
 * @brief Returns pointer.
 *
 * @param info
 *
 * @return
 */
template<typename T>
T* get_pointer(const v8::AccessorInfo& info) noexcept
{
    using namespace v8;

    Local<Object> self = info.Holder();
    Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
    return static_cast<T*>(wrap->Value());
}

/* ************************************************************************ */

v8::Handle<v8::Value> canvas_get_data(v8::Local<v8::String> property, const v8::AccessorInfo &info)
{
    using namespace v8;

    // Get context pointer
    auto context = get_pointer<render::Context>(info);

    // Get image data
    std::vector<std::uint8_t> data = context->getData();

    HandleScope handle_scope;

    // Create array
    Local<Array> array = Array::New(data.size());

    // TODO: better
    for (int i = 0; i < data.size(); ++i)
    {
        array->Set(i, Integer::New(data[i]));
    }

    return handle_scope.Close(array);
}

/* ************************************************************************ */

/**
 * @brief Save canvas data into file.
 *
 * @param args
 *
 * @return
 */
v8::Handle<v8::Value> canvas_save_to_file(const v8::Arguments& args)
{
    using namespace v8;

    HandleScope handle_scope;

    if (args.Length() < 1)
        ThrowException(String::New("missing argument: filename"));

    int width = 0;
    int height = 0;

    // Get filename
    String::Utf8Value filename_v8(args[0]);
    const std::string filename(*filename_v8);

    // Get extension
    const std::string ext = filename.substr(filename.find_last_of(".") + 1);

#ifdef JPEG_SUPPORT
    if (ext == "jpg" || ext == "jpeg")
    {
        // TODO
        JSAMPLE* data = nullptr;

        write_jpeg_file(data, width, height, filename, 75);
    }
    else
#endif
    if (true)
    {
        ThrowException(String::New("no support for required image type"));
    }

    return {};
}

/* ************************************************************************ */

}

/* ************************************************************************ */

v8::Local<v8::ObjectTemplate> create_canvas_template()
{
    using namespace v8;

    HandleScope handle_scope;

    Local<ObjectTemplate> tpl = ObjectTemplate::New();
    tpl->SetInternalFieldCount(1);
    tpl->SetAccessor(String::New("data"), canvas_get_data);
    tpl->Set("saveToFile", FunctionTemplate::New(canvas_save_to_file));

    return handle_scope.Close(tpl);
}

/* ************************************************************************ */

v8::Local<v8::Object> create_canvas(render::Context& context)
{
    using namespace v8;

    HandleScope handle_scope;

    // Get template
    Local<ObjectTemplate> tpl = create_canvas_template();

    // Create new instance
    Local<Object> obj = tpl->NewInstance();

    // Store pointer into JS object
    obj->SetInternalField(0, External::New(&context));

    // Return cell object
    return handle_scope.Close(obj);
}

/* ************************************************************************ */

}

/* ************************************************************************ */

#endif
