
/* ************************************************************************ */

// Declaration
#include "javascript/World.h"

// C++
#include <cassert>
#include <random>

// Core
#include "simulator/Cell.h"

// JavaScript
#include "javascript/Cell.hpp"

/* ************************************************************************ */

namespace javascript {

/* ************************************************************************ */

World* get_world()
{
    using namespace v8;

    HandleScope handle_scope;

    // Get world object
    Handle<Object> global = Context::GetCurrent()->Global();
    javascript::World* world = reinterpret_cast<javascript::World*>(global->GetPointerFromInternalField(0));
    assert(world);
    return world;
}

/* ************************************************************************ */

/**
 * @brief Rand function.
 */
static v8::Handle<v8::Value> js_rand(const v8::Arguments& args)
{
    return v8::Integer::New(std::rand());
}

/* ************************************************************************ */

World::World() noexcept
{
    std::srand(time(0));
}

/* ************************************************************************ */

World::~World()
{
    if (m_isolate)
        m_isolate->Dispose();
}

/* ************************************************************************ */

void World::reset()
{
    simulator::World::reset();

    // Reload source
    load(std::move(m_source));
}

/* ************************************************************************ */

void World::load(std::string source)
{
    using namespace v8;

    // Store source code
    m_source = std::move(source);

    // Reset simulation
    simulator::World::clean();

    // Dispose old isolate
    if (m_isolate)
        m_isolate->Dispose();

    // Create new isolate
    m_isolate = v8::Isolate::New();

    {
        Isolate::Scope isolate_scope(m_isolate);

        // Initialize context
        initContext();

        // Select V8 scope
        Context::Scope context_scope(m_context);
        HandleScope handle_scope;

        // Create V8 source
        Handle<String> code = String::New(m_source.data(), m_source.length());

        TryCatch try_catch;

        // Compile the script and check for errors.
        auto script = Script::Compile(code);

        if (try_catch.HasCaught() || script.IsEmpty())
        {
            String::Utf8Value error(try_catch.Exception());
            throw std::invalid_argument(*error);
        }

        runScript(script);
    }
}

/* ************************************************************************ */

void World::update(float step) noexcept
{
    using namespace v8;

    simulator::World::update(step);
}

/* ************************************************************************ */

void World::initContext()
{
    using namespace v8;

    HandleScope handle_scope;

    // Create global object -> world
    Handle<ObjectTemplate> world_tpl = ObjectTemplate::New();
    // First field is a pointer to this
    world_tpl->SetInternalFieldCount(1);

    //world_tpl->SetAccessor(String::New("width"), &World::GetWidth);
    //world_tpl->SetAccessor(String::New("height"), &World::GetHeight);

    // Global functions
    world_tpl->Set("Cell", v8::FunctionTemplate::New(create_cell));
    world_tpl->Set("rand", v8::FunctionTemplate::New(js_rand));
    //global->Set("barrier", v8::FunctionTemplate::New(createBarrier));

    // Create context
    m_context = Context::New(nullptr, world_tpl);

    {
        Context::Scope scope(m_context);

        // Set pointer to global object
        Handle<Object> world = m_context->Global();
        assert(!world.IsEmpty());
        world->SetPointerInInternalField(0, this);
    }
}

/* ************************************************************************ */

void World::runScript(v8::Handle<v8::Script> script)
{
    using namespace v8;

    HandleScope handle_scope;
    TryCatch try_catch;

    // Execute script
    script->Run();

    if (try_catch.HasCaught())
    {
        String::Utf8Value error(try_catch.Exception());
        throw std::invalid_argument(*error);
    }
}

/* ************************************************************************ */

}

/* ************************************************************************ */
