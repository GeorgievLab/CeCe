
/* ************************************************************************ */

// Declaration
#include "javascript/World.h"

// C++
#include <cassert>
#include <random>

// Core
#include "simulator/Cell.h"
#include "simulator/Yeast.h"

namespace {

/* ************************************************************************ */

/**
 * @brief Returns pointer.
 *
 * @param info
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

}

/* ************************************************************************ */

namespace javascript {

/* ************************************************************************ */

/**
 * @brief Returns a pointer to world.
 */
static simulator::World* get_world()
{
    v8::HandleScope handle_scope;

    // Get world object
    v8::Handle<v8::Object> global = v8::Context::GetCurrent()->Global();
    simulator::World* world = reinterpret_cast<simulator::World*>(global->GetPointerFromInternalField(0));
    assert(world);
    return world;
}

/* ************************************************************************ */

v8::Handle<v8::Value> GetGfp(v8::Local<v8::String> property, const v8::AccessorInfo &info)
{
    v8::Local<v8::Object> self = info.Holder();
    v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
    void* ptr = wrap->Value();
    int value = static_cast<simulator::Yeast*>(ptr)->getGfp();
    return v8::Integer::New(value);
}

/* ************************************************************************ */

void SetGfp(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::AccessorInfo& info)
{
    v8::Local<v8::Object> self = info.Holder();
    v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
    void* ptr = wrap->Value();
    static_cast<simulator::Yeast*>(ptr)->setGfp(value->Int32Value());
}

/* ************************************************************************ */

v8::Handle<v8::Value> GetRfp(v8::Local<v8::String> property, const v8::AccessorInfo &info)
{
    v8::Local<v8::Object> self = info.Holder();
    v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
    void* ptr = wrap->Value();
    int value = static_cast<simulator::Yeast*>(ptr)->getRfp();
    return v8::Integer::New(value);
}

/* ************************************************************************ */

void SetRfp(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::AccessorInfo& info)
{
    v8::Local<v8::Object> self = info.Holder();
    v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
    void* ptr = wrap->Value();
    static_cast<simulator::Yeast*>(ptr)->setRfp(value->Int32Value());
}

/* ************************************************************************ */

v8::Handle<v8::Value> GetYfp(v8::Local<v8::String> property, const v8::AccessorInfo &info)
{
    v8::Local<v8::Object> self = info.Holder();
    v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
    void* ptr = wrap->Value();
    int value = static_cast<simulator::Yeast*>(ptr)->getYfp();
    return v8::Integer::New(value);
}

/* ************************************************************************ */

void SetYfp(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::AccessorInfo& info)
{
    v8::Local<v8::Object> self = info.Holder();
    v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
    void* ptr = wrap->Value();
    static_cast<simulator::Yeast*>(ptr)->setYfp(value->Int32Value());
}

/* ************************************************************************ */

/**
 * @brief Create a new yeast.
 */
static v8::Handle<v8::Value> create_yeast(const v8::Arguments& args)
{
    using namespace v8;

    v8::HandleScope handle_scope;

    // Get world object
    simulator::World* world = get_world();

    MicroMeters x = 0_um;
    MicroMeters y = 0_um;
    MicroMeters radius;
    MicroMeters volume;
    unsigned int gfp = 0;
    unsigned int rfp = 0;
    unsigned int yfp = 0;
    simulator::Cell::BehaviourFn fn;

    // Cell object
    v8::Local<v8::ObjectTemplate> yeast_template = v8::ObjectTemplate::New();
    yeast_template->SetInternalFieldCount(1);
    yeast_template->SetAccessor(v8::String::New("gfp"), GetGfp, SetGfp);
    yeast_template->SetAccessor(v8::String::New("rfp"), GetRfp, SetRfp);
    yeast_template->SetAccessor(v8::String::New("yfp"), GetYfp, SetYfp);

    v8::Local<v8::Object> obj = yeast_template->NewInstance();

    // Get options
    if (args.Length() >= 1)
    {
        auto options = args[0].As<v8::Object>();

        // Keys
        auto keyX = v8::String::New("x");
        auto keyY = v8::String::New("y");
        auto keyRadius = v8::String::New("radius");
        auto keyVolume = v8::String::New("volume");
        auto gfpName = v8::String::New("gfp");
        auto rfpName = v8::String::New("rfp");
        auto yfpName = v8::String::New("yfp");

        // X
        if (options->Has(keyX))
        {
            v8::Local<v8::Value> val = options->Get(keyX);
            if (!val->IsNumber())
                v8::ThrowException(v8::String::New("Yeast option 'x' must be a number"));

            x = MicroMeters(val->NumberValue());
        }

        // Y
        if (options->Has(keyY))
        {
            v8::Local<v8::Value> val = options->Get(keyY);
            if (!val->IsNumber())
                v8::ThrowException(v8::String::New("Yeast option 'y' must be a number"));

            y = MicroMeters(val->NumberValue());
        }

        // Radius
        if (options->Has(keyRadius))
        {
            v8::Local<v8::Value> val = options->Get(keyRadius);
            if (!val->IsNumber())
                v8::ThrowException(v8::String::New("Yeast option 'radius' must be a number"));

            radius = MicroMeters(val->NumberValue());
        }

        // Volume
        if (options->Has(keyVolume))
        {
            v8::Local<v8::Value> val = options->Get(keyVolume);
            if (!val->IsNumber())
                v8::ThrowException(v8::String::New("Yeast option 'volume' must be a number"));

            volume = MicroMeters3(val->NumberValue());
        }

        // GFP
        if (options->Has(gfpName))
        {
            v8::Local<v8::Value> val = options->Get(gfpName);
            if (!val->IsNumber())
                v8::ThrowException(v8::String::New("Yeast option 'gfp' must be a number"));

            gfp = val->Int32Value();
        }

        // RFP
        if (options->Has(rfpName))
        {
            v8::Local<v8::Value> val = options->Get(rfpName);
            if (!val->IsNumber())
                v8::ThrowException(v8::String::New("Yeast option 'yfp' must be a number"));

            rfp = val->Int32Value();
        }

        // YFP
        if (options->Has(yfpName))
        {
            v8::Local<v8::Value> val = options->Get(yfpName);
            if (!val->IsNumber())
                v8::ThrowException(v8::String::New("Yeast option 'gfp' must be a number"));

            yfp = val->Int32Value();
        }

        // Function defined
        if (args.Length() >= 2)
        {
            // Store program
            obj->Set(String::New("program"), Local<Function>::Cast(args[1]));
        }
    }

    if (volume.value() && radius.value())
    {
        v8::ThrowException(v8::String::New("Yeast option 'radius' and 'volume' cannot be used together"));
    }

    if (!volume.value() && !radius.value())
    {
        radius = 10_um;
    }

    simulator::Cell* cell = nullptr;

    if (radius.value())
    {
        // Create a new yeast
        cell = world->newCell<simulator::Yeast>(x, y, std::move(fn), radius);
    }
    else if (volume.value())
    {
        // Create a new yeast
        cell = world->newCell<simulator::Yeast>(x, y, std::move(fn), volume);
    }
    else
    {
        v8::ThrowException(v8::String::New("Please set yeast option 'radius' or 'volume' to non-zero"));
    }

    cell->setGfp(gfp);
    cell->setRfp(rfp);
    cell->setYfp(yfp);

    obj->SetInternalField(0, v8::External::New(cell));

    return v8::External::Wrap(cell);
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

    initContext();
}

/* ************************************************************************ */

World::~World()
{
    m_context.Dispose();
}

/* ************************************************************************ */

void World::reset()
{
    simulator::World::reset();

    using namespace v8;

    // Reset context
    m_context.Dispose();
    initContext();

    load(std::move(m_source));
}

/* ************************************************************************ */

void World::load(std::string source)
{
    m_source = std::move(source);

    using namespace v8;

    HandleScope handle_scope;

    // Select V8 scope
    Context::Scope context_scope(m_context);

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
    world_tpl->Set("yeast", v8::FunctionTemplate::New(create_yeast));
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
