/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/*                                                                          */
/* This file is part of CeCe.                                               */
/*                                                                          */
/* CeCe is free software: you can redistribute it and/or modify             */
/* it under the terms of the GNU General Public License as published by     */
/* the Free Software Foundation, either version 3 of the License, or        */
/* (at your option) any later version.                                      */
/*                                                                          */
/* CeCe is distributed in the hope that it will be useful,                  */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of           */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            */
/* GNU General Public License for more details.                             */
/*                                                                          */
/* You should have received a copy of the GNU General Public License        */
/* along with CeCe.  If not, see <http://www.gnu.org/licenses/>.            */
/*                                                                          */
/* ************************************************************************ */

// Declaration
#include "cece/plugins/agglutination/Module.hpp"

// C++
#include <random>
#include <algorithm>

// CeCe
#include "cece/core/Log.hpp"
#include "cece/core/constants.hpp"
#include "cece/core/TimeMeasurement.hpp"
#include "cece/simulator/TimeMeasurement.hpp"
#include "cece/simulator/Simulation.hpp"

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace agglutination {

/* ************************************************************************ */

namespace {

/* ************************************************************************ */

std::random_device g_rd;

/* ************************************************************************ */

std::default_random_engine g_gen(g_rd());

/* ************************************************************************ */

RealType getRelativeReceptorProportion(
    RealType radius,
    unsigned int numberOfRec)
{
    return 1 - std::exp(- numberOfRec / (4 * core::constants::PI * radius * radius));
}

/* ************************************************************************ */

RealType getAssociationPropensity(
    units::Duration step,
    RealType radius1,
    RealType radius2,
    unsigned int numberOfRec1,
    unsigned int numberOfRec2,
    RealType Ka)
{
    return  getRelativeReceptorProportion(radius1, numberOfRec1) *
            getRelativeReceptorProportion(radius2, numberOfRec2) *
            (1 - std::exp(-Ka * step.value()));
}

/* ************************************************************************ */

RealType getDisassociationPropensity(
    units::Duration step,
    RealType Kd)
{
    return 1 - std::exp(-Kd * step.value());
}

/* ************************************************************************ */

}

/* ************************************************************************ */

void Module::update()
{
    // Store time step
    m_step = getSimulation().getTimeStep();

    auto _ = measure_time("agglutination", simulator::TimeMeasurement(getSimulation()));

    // Get physics world
    auto& world = getSimulation().getWorld();

    // Foreach pending bodies
    for (const auto& p : m_toJoin)
    {
        b2WeldJointDef joint;
        joint.Initialize(p.bodyA, p.bodyB, p.bodyA->GetWorldCenter());
        JointUserData* jUserData = new JointUserData();
        jUserData->module = this;
        jUserData->Kd = p.dConst;
        joint.userData = jUserData;
        world.CreateJoint(&joint);
    }

    m_toJoin.clear();

    // Joints to remove
    DynamicArray<b2Joint*> toRemove;

    // Foreach active joints
    for (auto joint = world.GetJointList(); joint != nullptr; joint = joint->GetNext())
    {
        const JointUserData* jUserData = reinterpret_cast<const JointUserData*>(joint->GetUserData());
        // Not our joint
        if (jUserData == nullptr)
            continue;
        if (jUserData->guard != '@')
            continue;

        std::bernoulli_distribution dist(
            getDisassociationPropensity(
                m_step,
                jUserData->Kd
            )
        );

        if (dist(g_gen))
        {
            Log::debug("Released: ", joint->GetBodyA(), ", ", joint->GetBodyB());
            toRemove.push_back(joint);
            delete jUserData;
        }
    }

    // Destroy joints
    for (auto joint : toRemove)
        world.DestroyJoint(joint);
}

/* ************************************************************************ */

void Module::loadConfig(const config::Configuration& config)
{
    // Configure parent
    module::Module::loadConfig(config);

    getSimulation().getWorld().SetContactListener(this);

    for (auto&& c_bond : config.getConfigurations("bond"))
    {
        m_bonds.push_back(Bond{
            c_bond.get<RealType>("association-constant"),
            c_bond.get<RealType>("disassociation-constant"),
            c_bond.get("ligand"),
            c_bond.get("receptor")
        });
    }
}

/* ************************************************************************ */

void Module::storeConfig(config::Configuration& config) const
{
    module::Module::storeConfig(config);

    // Foreach bonds
    for (const auto& bond : m_bonds)
    {
        auto bondConfig = config.addConfiguration("bond");
        bondConfig.set("association-constant", bond.aConst);
        bondConfig.set("disassociation-constant", bond.dConst);
        bondConfig.set("ligand", bond.ligand);
        bondConfig.set("receptor", bond.receptor);
    }
}

/* ************************************************************************ */

void Module::BeginContact(b2Contact* contact)
{
    auto ba = contact->GetFixtureA()->GetBody();
    auto bb = contact->GetFixtureB()->GetBody();
    auto oa = static_cast<object::Object*>(ba->GetUserData());
    auto ob = static_cast<object::Object*>(bb->GetUserData());
    if (!oa->is<plugin::cell::CellBase>() || !ob->is<plugin::cell::CellBase>())
        return;
    auto& ca = static_cast<object::Object*>(ba->GetUserData())->castThrow<plugin::cell::CellBase>();
    auto& cb = static_cast<object::Object*>(bb->GetUserData())->castThrow<plugin::cell::CellBase>();
    auto radius1 = ca.getShapes()[0].getCircle().radius;
    auto radius2 = cb.getShapes()[0].getCircle().radius;

    for (unsigned int i = 0; i < m_bonds.size(); i++)
    {
        std::bernoulli_distribution dist1(
            getAssociationPropensity(m_step, radius1.value(), radius2.value(),
                ca.getMoleculeCount(m_bonds[i].receptor), cb.getMoleculeCount(m_bonds[i].ligand),
                m_bonds[i].aConst));
        if (dist1(g_gen))
        {
            Log::debug("Joined: ", ba, ", ", bb);
            m_toJoin.push_back(JointDef{ba, bb, m_bonds[i].dConst});
            continue;
        }
        std::bernoulli_distribution dist2(
            getAssociationPropensity(m_step, radius1.value(), radius2.value(),
                cb.getMoleculeCount(m_bonds[i].receptor), ca.getMoleculeCount(m_bonds[i].ligand),
                m_bonds[i].aConst));
        if (dist2(g_gen))
        {
            Log::debug("Joined: ", ba, ", ", bb);
            m_toJoin.push_back(JointDef{ba, bb, m_bonds[i].dConst});
            continue;
        }
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
}

/* ************************************************************************ */
