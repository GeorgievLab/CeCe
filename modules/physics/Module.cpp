
// Declaration
#include "Module.hpp"

// Simulator
#include "simulator/World.hpp"
#include "simulator/DynamicObject.hpp"
#include "simulator/Cell.hpp"

/* ************************************************************************ */

namespace module {
namespace physics {

/* ************************************************************************ */

Module::Module()
{
    // Nothing to do
}

/* ************************************************************************ */

Module::~Module()
{
    // Nothing to do
}

/* ************************************************************************ */

void Module::update(units::Duration dt, simulator::World& world)
{
    for (auto& obj1 : world.getObjects())
    {
        for (auto& obj2 : world.getObjects())
        {
            if (obj1 == obj2)
                continue;

            if (!obj1->hasFlag(simulator::OBJECT_CELL) ||
                !obj2->hasFlag(simulator::OBJECT_CELL))
                continue;

            // Cast
            auto ptr1 = obj1->cast<simulator::Cell>();
            auto ptr2 = obj2->cast<simulator::Cell>();

            // Position difference vector
            auto diff = ptr1->getPosition() - ptr2->getPosition();

            // Distance
            auto length = diff.getLength();

            auto r1 = simulator::Cell::calcSphereRadius(ptr1->getVolume());
            auto r2 = simulator::Cell::calcSphereRadius(ptr2->getVolume());

            if (length > r1 + r2)
                continue;

            // TODO: improve
            ptr1->setVelocity(ptr1->getVelocity() * -1);
            ptr2->setVelocity(ptr2->getVelocity() * -1);
        }
    }
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */
