
/* ************************************************************************ */

// Declaration
#include "javascript/World.h"

// C++
#include <cassert>
#include <random>

// Core
#include "simulator/Cell.h"
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

        // GFP
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
            v8::Handle<v8::Function> callback = v8::Local<v8::Function>::Cast(args[1]->ToObject());

            fn = [obj, callback](simulator::Cell& cell) {
                //callback->Call(obj, 0, nullptr);
            };
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

WorldImplData::WorldImplData() noexcept
{
    std::srand(std::time(0));

    v8::HandleScope handle_scope;

    v8::Handle<v8::ObjectTemplate> global = v8::ObjectTemplate::New();

    // Global functions
    global->Set("yeast", v8::FunctionTemplate::New(create_yeast));
    global->Set("rand", v8::FunctionTemplate::New(js_rand));
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
