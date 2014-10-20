
/* ************************************************************************ */

// Declaration
#include "JsWorld.h"

// C++
#include <cassert>

// Core
#include "Yeast.h"

/* ************************************************************************ */

/**
 * @brief Returns a pointer to world.
 */
static JsWorld* GetWorld()
{
    v8::HandleScope handle_scope;

    // Get world object
    v8::Handle<v8::Object> global = v8::Context::GetCurrent()->Global();
    v8::Handle<v8::Value> worldValue = global->Get(v8::String::New("world"));
    assert(worldValue->IsExternal());
    v8::Handle<v8::External> worldHandle = worldValue.As<v8::External>();
    JsWorld* world = reinterpret_cast<JsWorld*>(worldHandle->Value());
    assert(world);
    return world;
}

/* ************************************************************************ */

/**
 * @brief Create a new yeast.
 */
static v8::Handle<v8::Value> CreateYeast(const v8::Arguments& args)
{
    v8::HandleScope handle_scope;

    // Get world object
    JsWorld* world = GetWorld();

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
        world->NewCell<Yeast>(x, y, radius);
    }
    else if (volume.value())
    {
        // Create a new yeast
        world->NewCell<Yeast>(x, y, volume);
    }
    else
    {
        v8::ThrowException(v8::String::New("Please set yeast option 'radius' or 'volume' to non-zero"));
    }

    return v8::Undefined();
}

/* ************************************************************************ */

/**
 * @brief Create a new barrier.
 */
static v8::Handle<v8::Value> CreateBarrier(const v8::Arguments& args)
{
    v8::HandleScope handle_scope;

    // Get world object
    JsWorld* world = GetWorld();

    // Arguments
    if (args.Length() < 4)
    {
        v8::ThrowException(v8::String::New("Missing arguments"));
    }

    v8::Local<v8::Number> x1val = args[0].As<v8::Number>();
    v8::Local<v8::Number> y1val = args[1].As<v8::Number>();
    v8::Local<v8::Number> x2val = args[2].As<v8::Number>();
    v8::Local<v8::Number> y2val = args[3].As<v8::Number>();

    MicroMeters x1(x1val->Value());
    MicroMeters y1(y1val->Value());
    MicroMeters x2(x2val->Value());
    MicroMeters y2(y2val->Value());

    world->NewBarrier(x1, y1, x2, y2);

    return v8::Undefined();
}

/* ************************************************************************ */

JsWorld::JsWorld() noexcept
{
    v8::HandleScope handle_scope;

    v8::Handle<v8::ObjectTemplate> global = v8::ObjectTemplate::New();

    // Global functions
    global->Set("yeast", v8::FunctionTemplate::New(CreateYeast));
    global->Set("barrier", v8::FunctionTemplate::New(CreateBarrier));
    global->Set("world", v8::External::New(this));

    // Create context
    m_context = v8::Context::New(nullptr, global);
}

/* ************************************************************************ */

JsWorld::~JsWorld()
{
    m_context.Dispose();
}

/* ************************************************************************ */
