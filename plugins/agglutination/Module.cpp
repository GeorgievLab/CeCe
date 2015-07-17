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

// Simulator
#include "core/TimeMeasurement.hpp"
#include "simulator/Simulation.hpp"

/* ************************************************************************ */

namespace plugin {
namespace agglutination {

/* ************************************************************************ */

void Module::update(units::Duration dt, simulator::Simulation& simulation)
{
    auto _ = measure_time("agglutination", simulator::TimeMeasurementIterationOutput(simulation));

    // Foreach pending bodies
    for (auto p : m_toJoin)
    {
        //b2RevoluteJointDef joint;
        b2WeldJointDef joint;
        joint.Initialize(p.first, p.second, p.first->GetWorldCenter());
        simulation.getWorld().CreateJoint(&joint);
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

    m_toJoin.emplace_back(ba, bb);
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
