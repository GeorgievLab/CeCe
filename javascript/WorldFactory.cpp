
/* ************************************************************************ */

// Declaration
#include "javascript/WorldFactory.h"

// V8
#include <v8.h>

// Core
#include "javascript/World.h"

/* ************************************************************************ */

namespace javascript {

/* ************************************************************************ */

std::unique_ptr<simulator::World> WorldFactory::createWorldFromSource(const std::string& source) const
{
    v8::HandleScope handle_scope;

    // Create world data
    std::unique_ptr<javascript::WorldImplData> data(new javascript::WorldImplData());

    // Select V8 scope
    v8::Context::Scope context_scope(data->GetContext());

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

    // Create world
    std::unique_ptr<simulator::World> world(new simulator::World(std::move(data)));

    // Register world object
    v8::Handle<v8::Object> global = v8::Context::GetCurrent()->Global();
    global->Set(v8::String::New("world"), v8::External::New(world.get()));

    // Execute script
    compiled_script->Run();

    if (try_catch.HasCaught())
    {
        v8::String::Utf8Value error(try_catch.Exception());
        throw std::invalid_argument(*error);
    }

    return world;
}

/* ************************************************************************ */

}

/* ************************************************************************ */
