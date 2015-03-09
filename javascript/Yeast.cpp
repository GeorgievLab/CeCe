
/* ************************************************************************ */

// Declaration
#include "javascript/Yeast.hpp"

// Core
#include "core/Units.hpp"

// Simulator
#include "simulator/Yeast.hpp"

// JavaScript
#include "javascript/World.hpp"

/* ************************************************************************ */

namespace javascript {

/* ************************************************************************ */

namespace {

/* ************************************************************************ */

/**
 * @brief Returns pointer.
 *
 * @param info
 */
template<typename T>
T* get_pointer(const v8::Local<v8::Object>& object) noexcept
{
    using namespace v8;

    Local<External> wrap = Local<External>::Cast(object->GetInternalField(0));
    return static_cast<T*>(wrap->Value());
}

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

    return get_pointer<T>(info.Holder());
}

/* ************************************************************************ */

v8::Handle<v8::Value> get_id(v8::Local<v8::String> property, const v8::AccessorInfo &info)
{
    using namespace v8;

    return Integer::New(get_pointer<simulator::Yeast>(info)->getId());
}

/* ************************************************************************ */

v8::Handle<v8::Value> get_x(v8::Local<v8::String> property, const v8::AccessorInfo &info)
{
    using namespace v8;

    return Integer::New(get_pointer<simulator::Yeast>(info)->getPosition().x.value());
}

/* ************************************************************************ */

void set_x(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::AccessorInfo& info)
{
    using namespace v8;

    auto cell = get_pointer<simulator::Yeast>(info);

    auto pos = cell->getPosition();
    pos.x = Length(value->Int32Value());
    cell->setPosition(pos);
}

/* ************************************************************************ */

v8::Handle<v8::Value> get_y(v8::Local<v8::String> property, const v8::AccessorInfo &info)
{
    using namespace v8;

    return Integer::New(get_pointer<simulator::Yeast>(info)->getPosition().y.value());
}

/* ************************************************************************ */

void set_y(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::AccessorInfo& info)
{
    using namespace v8;

    auto cell = get_pointer<simulator::Yeast>(info);

    auto pos = cell->getPosition();
    pos.y = Length(value->Int32Value());
    cell->setPosition(pos);
}

/* ************************************************************************ */

v8::Handle<v8::Value> get_z(v8::Local<v8::String> property, const v8::AccessorInfo &info)
{
    using namespace v8;

    return Integer::New(get_pointer<simulator::Yeast>(info)->getPosition().z.value());
}

/* ************************************************************************ */

void set_z(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::AccessorInfo& info)
{
    using namespace v8;

    auto cell = get_pointer<simulator::Yeast>(info);

    auto pos = cell->getPosition();
    pos.z = Length(value->Int32Value());
    cell->setPosition(pos);
}

/* ************************************************************************ */

v8::Handle<v8::Value> get_gfp(v8::Local<v8::String> property, const v8::AccessorInfo& info)
{
    using namespace v8;

    return Integer::New(get_pointer<simulator::Yeast>(info)->getGfp());
}

/* ************************************************************************ */

void set_gfp(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::AccessorInfo& info)
{
    using namespace v8;

    get_pointer<simulator::Yeast>(info)->setGfp(value->Int32Value());
}

/* ************************************************************************ */

v8::Handle<v8::Value> get_rfp(v8::Local<v8::String> property, const v8::AccessorInfo &info)
{
    using namespace v8;

    return Integer::New(get_pointer<simulator::Yeast>(info)->getRfp());
}

/* ************************************************************************ */

void set_rfp(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::AccessorInfo& info)
{
    using namespace v8;

    get_pointer<simulator::Yeast>(info)->setRfp(value->Int32Value());
}

/* ************************************************************************ */

v8::Handle<v8::Value> get_yfp(v8::Local<v8::String> property, const v8::AccessorInfo &info)
{
    using namespace v8;

    return Integer::New(get_pointer<simulator::Yeast>(info)->getYfp());
}

/* ************************************************************************ */

void set_yfp(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::AccessorInfo& info)
{
    using namespace v8;

    get_pointer<simulator::Yeast>(info)->setYfp(value->Int32Value());
}

/* ************************************************************************ */

v8::Handle<v8::Value> get_volume(v8::Local<v8::String> property, const v8::AccessorInfo &info)
{
    using namespace v8;

    return Integer::New(get_pointer<simulator::Yeast>(info)->getVolume().value());
}

/* ************************************************************************ */

void set_volume(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::AccessorInfo& info)
{
    using namespace v8;

    get_pointer<simulator::Yeast>(info)->setVolume(Length(value->Int32Value()));
}

/* ************************************************************************ */

v8::Handle<v8::Value> get_bud(v8::Local<v8::String> property, const v8::AccessorInfo &info)
{
    using namespace v8;

    auto ptr = get_pointer<simulator::Yeast>(info);
    auto bud = ptr->getBud();

    if (bud)
    {
        HandleScope handle_scope;

        // Cell object template
        Local<ObjectTemplate> yeast_template = create_yeast_template();

        // Create new instance
        Local<Object> obj = yeast_template->NewInstance();

        // Store pointer into JS object
        obj->SetInternalField(0, External::New(bud));

        return handle_scope.Close(obj);
    }

    return Null();
}

/* ************************************************************************ */

v8::Handle<v8::Value> bud_create(const v8::Arguments& args)
{
    get_pointer<simulator::Yeast>(args.Holder())->budCreate(Volume(500));
    return v8::Handle<v8::Value>{};
}

/* ************************************************************************ */

v8::Handle<v8::Value> bud_release(const v8::Arguments& args)
{
    auto ptr = get_pointer<simulator::Yeast>(args.Holder());
    if (ptr) ptr->budRelease();
    return v8::Handle<v8::Value>{};
}

/* ************************************************************************ */

}

/* ************************************************************************ */

v8::Local<v8::ObjectTemplate> create_yeast_template()
{
    using namespace v8;

    HandleScope handle_scope;

    Local<ObjectTemplate> yeast_template = ObjectTemplate::New();
    yeast_template->SetInternalFieldCount(1);
    yeast_template->SetAccessor(String::New("id"), get_id);
    yeast_template->SetAccessor(String::New("x"), get_x, set_x);
    yeast_template->SetAccessor(String::New("y"), get_y, set_y);
    yeast_template->SetAccessor(String::New("z"), get_z, set_z);
    yeast_template->SetAccessor(String::New("gfp"), get_gfp, set_gfp);
    yeast_template->SetAccessor(String::New("rfp"), get_rfp, set_rfp);
    yeast_template->SetAccessor(String::New("yfp"), get_yfp, set_yfp);
    yeast_template->SetAccessor(String::New("volume"), get_volume, set_volume);
    yeast_template->SetAccessor(String::New("bud"), get_bud);

    yeast_template->Set(String::New("budCreate"), FunctionTemplate::New(bud_create));
    yeast_template->Set(String::New("budRelease"), FunctionTemplate::New(bud_release));

    return handle_scope.Close(yeast_template);
}

/* ************************************************************************ */

/**
 * @brief Create a new cell.
 */
v8::Handle<v8::Value> create_yeast(const v8::Arguments& args)
{
    using namespace v8;

    HandleScope handle_scope;

    // Cell object template
    Local<ObjectTemplate> yeast_template = create_yeast_template();

    // Create new instance
    Local<Object> obj = yeast_template->NewInstance();

    // Get world object
    auto world = get_world();

    // Create new cell
    simulator::Yeast* cell = world->newCell<simulator::Yeast>();

    // Store pointer into JS object
    obj->SetInternalField(0, External::New(cell));

    // Get options
    if (args.Length() >= 1)
    {
        // Get options
        auto options = args[0].As<Object>();

        {
            // Cell position
            Position position{0_um, 0_um, 0_um};

            // X
            if (options->Has(String::New("x")))
            {
                Local<Value> val = options->Get(String::New("x"));
                if (!val->IsNumber())
                    ThrowException(String::New("Yeast option 'x' must be a number"));

                position.x = Length(val->NumberValue());
            }

            // Y
            if (options->Has(String::New("y")))
            {
                Local<Value> val = options->Get(String::New("y"));
                if (!val->IsNumber())
                    ThrowException(String::New("Yeast option 'y' must be a number"));

                position.y = Length(val->NumberValue());
            }

            // Y
            if (options->Has(String::New("z")))
            {
                Local<Value> val = options->Get(String::New("z"));
                if (!val->IsNumber())
                    ThrowException(String::New("Yeast option 'z' must be a number"));

                position.z = Length(val->NumberValue());
            }

            // Set new position
            cell->setPosition(position);
        }

        // Radius
        if (options->Has(String::New("radius")))
        {
            Local<Value> val = options->Get(String::New("radius"));
            if (!val->IsNumber())
                ThrowException(String::New("Yeast option 'radius' must be a number"));

            // TODO: set radius
            //cell->setVolume()
            //radius = MicroMeters(val->NumberValue());
        }

        // Volume
        if (options->Has(String::New("volume")))
        {
            Local<Value> val = options->Get(String::New("volume"));
            if (!val->IsNumber())
                ThrowException(String::New("Yeast option 'volume' must be a number"));

            cell->setVolume(Length(val->NumberValue()));
        }

        // GFP
        if (options->Has(String::New("gfp")))
        {
            Local<Value> val = options->Get(String::New("gfp"));
            if (!val->IsNumber())
                v8::ThrowException(String::New("Yeast option 'gfp' must be a number"));

            cell->setGfp(val->Int32Value());
        }

        // RFP
        if (options->Has(String::New("rfp")))
        {
            Local<Value> val = options->Get(String::New("rfp"));
            if (!val->IsNumber())
                v8::ThrowException(String::New("Yeast option 'rfp' must be a number"));

            cell->setRfp(val->Int32Value());
        }

        // YFP
        if (options->Has(String::New("yfp")))
        {
            Local<Value> val = options->Get(String::New("yfp"));
            if (!val->IsNumber())
                v8::ThrowException(String::New("Yeast option 'yfp' must be a number"));

            cell->setYfp(val->Int32Value());
        }

        // Function defined
        if (args.Length() >= 2)
        {
            Persistent<Function> callback = Persistent<Function>::New(Handle<Function>::Cast(args[1]));
            Persistent<Object> object = Persistent<Object>::New(obj);
            Isolate* isolate = Isolate::GetCurrent();

            cell->setBehaviour([callback, object, isolate](simulator::Cell& cell) {
                Isolate::Scope isolate_scope(isolate);
                HandleScope handle_scope;
                callback->Call(object, 0, nullptr);
            });
        }
    }

    // Return cell object
    return handle_scope.Close(obj);
}

/* ************************************************************************ */

}

/* ************************************************************************ */
