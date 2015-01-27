
/* ************************************************************************ */

// Declaration
#include "javascript/Barrier.hpp"

// JavaScript
#include "javascript/World.h"

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
T* get_pointer(const v8::AccessorInfo& info) noexcept
{
    using namespace v8;

    Local<Object> self = info.Holder();
    Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
    return static_cast<T*>(wrap->Value());
}

/* ************************************************************************ */

v8::Handle<v8::Value> get_x(v8::Local<v8::String> property, const v8::AccessorInfo &info)
{
    using namespace v8;

    return Integer::New(get_pointer<simulator::Barrier>(info)->getPosition().x.value());
}

/* ************************************************************************ */

v8::Handle<v8::Value> get_y(v8::Local<v8::String> property, const v8::AccessorInfo &info)
{
    using namespace v8;

    return Integer::New(get_pointer<simulator::Barrier>(info)->getPosition().y.value());
}

/* ************************************************************************ */

v8::Handle<v8::Value> get_z(v8::Local<v8::String> property, const v8::AccessorInfo &info)
{
    using namespace v8;

    return Integer::New(get_pointer<simulator::Barrier>(info)->getPosition().z.value());
}

/* ************************************************************************ */

v8::Handle<v8::Value> get_width(v8::Local<v8::String> property, const v8::AccessorInfo &info)
{
    using namespace v8;

    return Integer::New(get_pointer<simulator::Barrier>(info)->getWidth().value());
}

/* ************************************************************************ */

v8::Handle<v8::Value> get_depth(v8::Local<v8::String> property, const v8::AccessorInfo &info)
{
    using namespace v8;

    return Integer::New(get_pointer<simulator::Barrier>(info)->getDepth().value());
}

/* ************************************************************************ */

v8::Handle<v8::Value> get_height(v8::Local<v8::String> property, const v8::AccessorInfo &info)
{
    using namespace v8;

    return Integer::New(get_pointer<simulator::Barrier>(info)->getHeight().value());
}

/* ************************************************************************ */

/**
 * @brief Create template for barrier object.
 *
 * @param Template.
 */
v8::Local<v8::ObjectTemplate> create_barrier_template()
{
    using namespace v8;

    HandleScope handle_scope;

    Local<ObjectTemplate> tpl = ObjectTemplate::New();
    tpl->SetInternalFieldCount(1);
    tpl->SetAccessor(String::New("x"), get_x);
    tpl->SetAccessor(String::New("y"), get_y);
    tpl->SetAccessor(String::New("z"), get_z);
    tpl->SetAccessor(String::New("width"), get_width);
    tpl->SetAccessor(String::New("depth"), get_depth);
    tpl->SetAccessor(String::New("height"), get_height);

    return handle_scope.Close(tpl);
}

/* ************************************************************************ */

}

/* ************************************************************************ */

v8::Handle<v8::Value> create_barrier(const v8::Arguments& args)
{
    using namespace v8;

    HandleScope handle_scope;

    // Cell object template
    Local<ObjectTemplate> tpl = create_barrier_template();

    // Create new instance
    Local<Object> obj = tpl->NewInstance();

    // Get world object
    auto world = get_world();

    // Create new barrier
    simulator::Barrier* barrier = world->newBarrier<simulator::Barrier>();

    // Store pointer into JS object
    obj->SetInternalField(0, External::New(barrier));

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

                position.x = MicroMeters(val->NumberValue());
            }

            // Y
            if (options->Has(String::New("y")))
            {
                Local<Value> val = options->Get(String::New("y"));
                if (!val->IsNumber())
                    ThrowException(String::New("Yeast option 'y' must be a number"));

                position.y = MicroMeters(val->NumberValue());
            }

            // Y
            if (options->Has(String::New("z")))
            {
                Local<Value> val = options->Get(String::New("z"));
                if (!val->IsNumber())
                    ThrowException(String::New("Yeast option 'z' must be a number"));

                position.z = MicroMeters(val->NumberValue());
            }

            // Set new position
            barrier->setPosition(position);
        }

        // Width
        if (options->Has(String::New("width")))
        {
            Local<Value> val = options->Get(String::New("width"));
            if (!val->IsNumber())
                ThrowException(String::New("Yeast option 'width' must be a number"));

            barrier->setWidth(MicroMeters(val->NumberValue()));
        }

        // Height
        if (options->Has(String::New("height")))
        {
            Local<Value> val = options->Get(String::New("height"));
            if (!val->IsNumber())
                ThrowException(String::New("Yeast option 'height' must be a number"));

            barrier->setHeight(MicroMeters(val->NumberValue()));
        }

        // Depth
        if (options->Has(String::New("depth")))
        {
            Local<Value> val = options->Get(String::New("depth"));
            if (!val->IsNumber())
                v8::ThrowException(String::New("Yeast option 'depth' must be a number"));

            barrier->setDepth(MicroMeters(val->NumberValue()));
        }
    }

    // Create body
    barrier->updateBody();

    // Return cell object
    return handle_scope.Close(obj);
}

/* ************************************************************************ */

}

/* ************************************************************************ */
