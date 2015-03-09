
/* ************************************************************************ */

// Declaration
#include "javascript/World.hpp"

// C++
#include <cassert>
#include <random>
#include <sstream>

// Core
#include "simulator/Cell.hpp"

// JavaScript
#include "javascript/Cell.hpp"
#include "javascript/Yeast.hpp"
#include "javascript/Barrier.hpp"
#include "javascript/Canvas.hpp"

/* ************************************************************************ */

namespace javascript {

/* ************************************************************************ */

static v8::Handle<v8::Value> log_callback(const v8::Arguments& args)
{
    using namespace v8;

    // Get world pointer
    World* world = get_world();

    HandleScope handle_scope;

    auto log = world->getLogFunction();
    if (!log)
        return {};

    std::ostringstream oss;

    for (int i = 0; i < args.Length(); ++i)
    {
        // Convert to string
        String::Utf8Value arg(args[i]->ToString());

        // Write string
        oss << *arg;
    }

    // Log value
    log(oss.str());

    return {};
}

/* ************************************************************************ */

static v8::Handle<v8::Value> unit_um(const v8::Arguments& args)
{
    using namespace v8;

    if (args.Length() == 0)
        ThrowException(String::New("Missing argument"));

    return args[0];
}

/* ************************************************************************ */

static v8::Handle<v8::Value> unit_mm(const v8::Arguments& args)
{
    using namespace v8;

    if (args.Length() == 0)
        ThrowException(String::New("Missing argument"));

    return args[0];
}

/* ************************************************************************ */

static v8::Handle<v8::Value> unit_um3(const v8::Arguments& args)
{
    using namespace v8;

    if (args.Length() == 0)
        ThrowException(String::New("Missing argument"));

    return args[0];
}

/* ************************************************************************ */

static v8::Handle<v8::Value> unit_mm3(const v8::Arguments& args)
{
    using namespace v8;

    if (args.Length() == 0)
        ThrowException(String::New("Missing argument"));

    return args[0];
}

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

static v8::Handle<v8::Value> world_step_number(v8::Local<v8::String> property, const v8::AccessorInfo &info)
{
    using namespace v8;

    auto value = get_world()->getStepNumber();

    return Integer::New(value);
}

/* ************************************************************************ */

World::World() noexcept
{
    // Nothing to do
}

/* ************************************************************************ */

World::~World()
{
    if (m_isolate)
        m_isolate->Dispose();
}

/* ************************************************************************ */

#ifdef ENABLE_RENDER

void World::setContext(render::Context& context)
{
    m_renderContext = &context;
}

#endif

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

    // Set global variables
    world_tpl->SetAccessor(String::New("stepNumber"), world_step_number);

    // Global functions
    world_tpl->Set("Cell", FunctionTemplate::New(create_cell));
    world_tpl->Set("Yeast", FunctionTemplate::New(create_yeast));
    world_tpl->Set("Barrier", FunctionTemplate::New(create_barrier));

    // Units
    world_tpl->Set("um", FunctionTemplate::New(unit_um));
    world_tpl->Set("mm", FunctionTemplate::New(unit_mm));
    world_tpl->Set("um3", FunctionTemplate::New(unit_um3));
    world_tpl->Set("mm3", FunctionTemplate::New(unit_mm3));

    Local<ObjectTemplate> console_tpl = ObjectTemplate::New();
    console_tpl->Set("log", FunctionTemplate::New(log_callback));

    // Create context
    m_context = Context::New(nullptr, world_tpl);

    {
        Context::Scope scope(m_context);

        // Set pointer to global object
        Handle<Object> world = m_context->Global();
        assert(!world.IsEmpty());
        world->SetPointerInInternalField(0, this);

        world->Set(String::New("console"), console_tpl->NewInstance());

#ifdef ENABLE_RENDER
        assert(m_renderContext);
        world->Set(String::New("canvas"), create_canvas(*m_renderContext));
#endif
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
