/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/* Author: Jiří Fatka <fatkaj@ntis.zcu.cz>                                  */
/* Author: Václav Pelíšek <pelisekv@students.zcu.cz>                        */
/* ************************************************************************ */

#pragma once

#include "../stochastic-reactions/Reaction.hpp"

/* ************************************************************************ */

namespace plugin {
namespace stochasticreactions {

/* ************************************************************************ */

class ReactionIntercellular: public Reaction
{
protected:

    float computePropensity(const unsigned int index) override;

    void executeReaction(const unsigned int index) override;

    void changeMoleculesInEnvironment(const String id, const int change);

    void extendAbsorption(const DynamicArray<String>& ids_plus, const float rate);

    void extendExpression(const DynamicArray<String>& ids_minus, const float rate);

public:

    void extend(const DynamicArray<String>& ids_plus, const DynamicArray<String>& ids_minus, const float rate) override;

    int getIndexOf(const String& id) override;

    bool areEqualRules(const Reaction& rhs, unsigned int index1, unsigned int index2);
};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
