
/* ************************************************************************ */

// Declaration
#include "JsWorldLoader.h"

// C++
#include <fstream>
#include <stdexcept>

// V8
#include <v8.h>

// Core
#include "JsWorld.h"

/* ************************************************************************ */

std::unique_ptr<World> JsWorldLoader::CreateWorldFromFile(const std::string& filename) const
{
    std::string source;

    {
        std::ifstream file(filename, std::ios::in);

        std::string line;
        while (std::getline(file, line))
        {
            // Read source
            source.append(line);
        }
    }

    return CreateWorldFromSource(source);
}

/* ************************************************************************ */

std::unique_ptr<World> JsWorldLoader::CreateWorldFromSource(const std::string& source) const
{
    v8::HandleScope handle_scope;

    std::unique_ptr<JsWorld> world(new JsWorld());

    // Select V8 scope
    v8::Context::Scope context_scope(world->GetContext());

    // Create V8 source
    v8::Handle<v8::String> script = v8::String::New(source.data(), source.length());

    v8::TryCatch try_catch;

    // Compile the script and check for errors.
    v8::Handle<v8::Script> compiled_script = v8::Script::Compile(script);

    if (try_catch.HasCaught() || compiled_script.IsEmpty())
    {
        v8::String::Utf8Value error(try_catch.Exception());
        throw std::invalid_argument(*error);
    }

    // Execute script
    compiled_script->Run();

    if (try_catch.HasCaught())
    {
        v8::String::Utf8Value error(try_catch.Exception());
        throw std::invalid_argument(*error);
    }

    return std::unique_ptr<World>{world.release()};
}

/* ************************************************************************ */
