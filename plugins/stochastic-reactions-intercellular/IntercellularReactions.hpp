/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/* Author: Václav Pelíšek <pelisekv@students.zcu.cz>                        */
/* Author: Jiří Fatka <fatkaj@ntis.zcu.cz>                                  */
/* ************************************************************************ */

#pragma once

// Plugins
#include "plugins/stochastic-reactions-intracellular/Reactions.hpp"
#include "plugins/diffusion/Module.hpp"

/* ************************************************************************ */

namespace plugin {
namespace stochastic_reactions_diffusive {

/* ************************************************************************ */

using RateType = plugin::stochastic_reactions::RateType;

/* ************************************************************************ */

struct ReqProd
{
    ReqProd() = default;
    unsigned int requirement = 0;
    unsigned int product = 0;
    unsigned int env_requirement = 0;
    unsigned int env_product = 0;
    bool mustnt_have = false;
    ReqProd(int req, int prod, int e_req = 0, int e_prod = 0, bool flag = false) noexcept
    : requirement(req), product(prod), env_requirement(e_req), env_product(e_prod), mustnt_have(flag)
    {
        // Nothing to do
    }
};

/* ************************************************************************ */

class IntercellularReactions: public stochastic_reactions::Reactions<ReqProd>
{

protected:

    PropensityType computePropensity(const unsigned int index, const plugin::cell::CellBase& cell, plugin::diffusion::Module* diffusion, const DynamicArray<plugin::diffusion::Module::Coordinate>& coords);

    void initializePropensities(const plugin::cell::CellBase& cell, plugin::diffusion::Module* diffusion, const DynamicArray<plugin::diffusion::Module::Coordinate>& coords);

    void refreshPropensities(const unsigned int index, const plugin::cell::CellBase& cell, plugin::diffusion::Module* diffusion, const DynamicArray<plugin::diffusion::Module::Coordinate>& coords);

    void executeReaction(const unsigned int index, plugin::cell::CellBase& cell, plugin::diffusion::Module* diffusion, const DynamicArray<plugin::diffusion::Module::Coordinate>& coords);

    void changeMoleculesInEnvironment(const String& id, const int change, plugin::diffusion::Module* diffusion, const DynamicArray<plugin::diffusion::Module::Coordinate>& coords);

    void extendAbsorption(const DynamicArray<String>& ids_plus, const RateType rate);

    void extendExpression(const DynamicArray<String>& ids_minus, const RateType rate);

public:

    void operator()(simulator::Object& object, simulator::Simulation& simulation, units::Duration step);

    void extend(const DynamicArray<String>& ids_plus, const DynamicArray<String>& ids_minus, const RateType rate);


    /**
     * @brief Add condition for required reaction.
     *
     * @param name         Molecule name that is used in condition.
     * @param requirements Amount of required molecule.
     * @param clone        Clone reaction.
     * @param noCond       Reaction.
     */
    void addCondition(const String& name, unsigned int requirement, bool clone, DynamicArray<ReqProd>& noCond);
};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
