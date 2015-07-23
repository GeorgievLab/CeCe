/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/* Author: Jiří Fatka <fatkaj@ntis.zcu.cz>                                  */
/* ************************************************************************ */

// Declaration
#include "Module.hpp"

// C++
#include <random>

// Simulator
#include "core/Log.hpp"
#include "core/TimeMeasurement.hpp"
#include "simulator/Simulation.hpp"

/* ************************************************************************ */

namespace plugin {
namespace agglutination {

/* ************************************************************************ */

void Module::update(units::Duration dt, simulator::Simulation& simulation)
{
    auto _ = measure_time("agglutination", simulator::TimeMeasurementIterationOutput(simulation));

    std::random_device rd;
    std::default_random_engine e1(rd());
    std::bernoulli_distribution dist(0.01);

    // Get physics world
    auto& world = simulation.getWorld();

    // Joints to remove
    DynamicArray<b2Joint*> toRemove;

    // Foreach active joints
    for (auto joint = world.GetJointList(); joint != nullptr; joint = joint->GetNext())
    {
        // Not our joint
        if (joint->GetUserData() != this)
            continue;

        // TODO: disconnect
        if (dist(e1))
        {
            Log::debug("Released: ", joint->GetBodyA(), ", ", joint->GetBodyB());
            toRemove.push_back(joint);
        }
    }

    // Destroy joints
    for (auto joint : toRemove)
        world.DestroyJoint(joint);

    // Foreach pending bodies
    for (auto p : m_toJoin)
    {
        //b2RevoluteJointDef joint;
        b2WeldJointDef joint;
        joint.Initialize(p.first, p.second, p.first->GetWorldCenter());
        joint.userData = this;
        world.CreateJoint(&joint);
    }

    m_toJoin.clear();
}

/* ************************************************************************ */

void Module::configure(const simulator::Configuration& config, simulator::Simulation& simulation)
{
    simulation.getWorld().SetContactListener(this);
}

/* ************************************************************************ */

void Module::BeginContact(b2Contact* contact)
{
    auto fa = contact->GetFixtureA();
    auto fb = contact->GetFixtureB();
    auto ba = fa->GetBody();
    auto bb = fb->GetBody();

    // TODO: connect only in some cases
    std::random_device rd;
    std::default_random_engine e1(rd());
    std::bernoulli_distribution dist(0.5);

    if (dist(e1))
    {
        Log::debug("Joined: ", ba, ", ", bb);
        m_toJoin.emplace_back(ba, bb);
    }
}

/* ************************************************************************ */

void Module::EndContact(b2Contact* contact)
{
    // Nothing to do?
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */
