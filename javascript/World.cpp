
/* ************************************************************************ */

// Declaration
#include "javascript/World.h"

// C++
#include <cassert>

// Core
#include "simulator/Yeast.h"

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
    v8::Handle<v8::Value> worldValue = global->Get(v8::String::New("world"));
    assert(worldValue->IsExternal());
    v8::Handle<v8::External> worldHandle = worldValue.As<v8::External>();
    simulator::World* world = reinterpret_cast<simulator::World*>(worldHandle->Value());
    assert(world);
    return world;
}

/* ************************************************************************ */

/**
 * @brief Create a new yeast.
 */
static v8::Handle<v8::Value> create_yeast(const v8::Arguments& args)
{
    v8::HandleScope handle_scope;

    // Get world object
    simulator::World* world = get_world();

    MicroMeters x = 0_um;
    MicroMeters y = 0_um;
    MicroMeters radius;
    MicroMeters volume;

    // Get options
    if (args.Length() >= 1)
    {
        auto options = args[0].As<v8::Object>();

        // Keys
        auto keyX = v8::String::New("x");
        auto keyY = v8::String::New("y");
        auto keyRadius = v8::String::New("radius");
        auto keyVolume = v8::String::New("volume");

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
    }

    if (volume.value() && radius.value())
    {
        v8::ThrowException(v8::String::New("Yeast option 'radius' and 'volume' cannot be used together"));
    }

    if (!volume.value() && !radius.value())
    {
        radius = 10_um;
    }

    if (radius.value())
    {
        // Create a new yeast
        world->newCell<simulator::Yeast>(x, y, radius);
    }
    else if (volume.value())
    {
        // Create a new yeast
        world->newCell<simulator::Yeast>(x, y, volume);
    }
    else
    {
        v8::ThrowException(v8::String::New("Please set yeast option 'radius' or 'volume' to non-zero"));
    }

    return v8::Undefined();
}

/* ************************************************************************ */

WorldImplData::WorldImplData() noexcept
{
    v8::HandleScope handle_scope;

    v8::Handle<v8::ObjectTemplate> global = v8::ObjectTemplate::New();

    // Global functions
    global->Set("yeast", v8::FunctionTemplate::New(create_yeast));
    //global->Set("barrier", v8::FunctionTemplate::New(createBarrier));

    // Create context
    m_context = v8::Context::New(nullptr, global);
}

/* ************************************************************************ */

WorldImplData::~WorldImplData()
{
    m_context.Dispose();
}

/* ************************************************************************ */

}

/* ************************************************************************ */
