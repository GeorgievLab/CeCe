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

/* ************************************************************************ */

// Simulator
#include "core/Real.hpp"

// Plugins
#include "plugins/stochastic-reactions-intracellular/Reactions.hpp"
#include "plugins/diffusion/Module.hpp"

/* ************************************************************************ */

namespace plugin {
namespace stochastic_reactions_diffusive {

/* ************************************************************************ */

struct ReqProd
{
    unsigned int requirement;
    unsigned int product;
    RealType env_requirement;
    unsigned int env_product;
    bool less;
    bool env_less;

    ReqProd(int req = 0, int prod = 0, RealType e_req = 0, int e_prod = 0, bool flag = false, bool flagEnv = false) noexcept
    : requirement(req), product(prod), env_requirement(e_req), env_product(e_prod), less(flag), env_less(flagEnv)
    {
        // Nothing to do
    }
};

/* ************************************************************************ */

class IntercellularReactions: public stochastic_reactions::Reactions<ReqProd>
{

protected:


    /**
     * @brief Computes propensity of given reaction.
     *
     * @param index of row, cell, diffusion
     * @return propensity
     */
    PropensityType computePropensity(const unsigned int index, const plugin::cell::CellBase& cell, plugin::diffusion::Module* diffusion, const DynamicArray<plugin::diffusion::Module::Coordinate>& coords);


    /**
     * @brief Computes propensities of all reactions.
     *
     * @param cell, diffusion
     * @return
     */
    void initializePropensities(const plugin::cell::CellBase& cell, plugin::diffusion::Module* diffusion, const DynamicArray<plugin::diffusion::Module::Coordinate>& coords);


    /**
     * @brief Refreshes propensities of ractions which have requirements of specific molecule.
     *
     * @param index of column, cell, diffusion
     */
    void refreshPropensities(const unsigned int index, const plugin::cell::CellBase& cell, plugin::diffusion::Module* diffusion, const DynamicArray<plugin::diffusion::Module::Coordinate>& coords);


    /**
     * @brief Executes reaction which ocuured.
     *
     * @param index of rection, cell, diffusion
     */
    void executeReaction(const unsigned int index, plugin::cell::CellBase& cell, plugin::diffusion::Module* diffusion, const DynamicArray<plugin::diffusion::Module::Coordinate>& coords);


    /**
     * @brief Function that releases or absorbs the molecules outside the cell.
     *
     * @param id of molecule, number of molecules, diffusion
     */
    void changeMoleculesInEnvironment(const simulator::Simulation& simulation, const String& id, const int change, plugin::diffusion::Module* diffusion, const DynamicArray<plugin::diffusion::Module::Coordinate>& coords);


    /**
     * @brief Function that extends reaction rule matrix with reaction that absorbs signal from the outside.
     *
     * @param array of IDs, reation rate
     */
    void extendAbsorption(const DynamicArray<String>& ids_plus, const RateType rate);


    /**
     * @brief Function that extends reaction rule matrix with reaction that releases signal outside the cell.
     *
     * @param array of IDs, reation rate
     */
    void extendExpression(const DynamicArray<String>& ids_minus, const RateType rate);


public:


    /**
     * @brief Main function of this plugin.
     *
     */
    void operator()(simulator::Object& object, simulator::Simulation& simulation, units::Duration step);


    /**
     * @brief Function that extends reaction rule matrix.
     *
     * @param array of molecule IDs reation rate
     */
    void extend(const DynamicArray<String>& ids_plus, const DynamicArray<String>& ids_minus, const RateType rate);


    /**
     * @brief Extend matrix using rules for intracellular rections.
     *
     * @param arrays of molucule's IDs, reaction rate
     *
     * @return
     */
    void extendIntracellular(const DynamicArray<String>& ids_plus, const DynamicArray<String>& ids_minus, const RateType rate);


    /**
     * @brief Add condition for required reaction.
     *
     * @param name         Molecule name that is used in condition.
     * @param requirements Amount of required molecule.
     * @param clone        Clone reaction.
     * @param noCond       Reaction.
     */
    void addCondition(const Condition& condition, DynamicArray<ReqProd>& noCond);
};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
