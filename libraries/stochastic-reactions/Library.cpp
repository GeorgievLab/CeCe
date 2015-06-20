/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/* Author:                                                                  */  
/* Václav Pelíšek <pelisekv@students.zcu.cz>                                */
/* Jiří Fatka <fatkaj@ntis.zcu.cz>                                          */
/* ************************************************************************ */

// Declaration
#include "Library.hpp"

// C++
#include <cassert>
#include <cstring>

// Simulator
#include "parser/Parser.hpp"
#include "simulator/Simulation.hpp"
#include "simulator/Library.hpp"
#include "simulator/LibraryApi.hpp"
#include "core/Log.hpp"

// Reactions
#include "Reaction.hpp"

/************************************************************************** */

using namespace simulator;

/* ************************************************************************ */

/**
 * @brief Parse inner XML code of reaction.
 *
 * @param String
 *
 * @return instance of Reaction
 */
 
Reaction parseReactionCode(const String code)
{
    // initial row of rule matrix
    DynamicArray<int> local (1);
    // flag that indicates if the pointer is before, after or in rate area
    unsigned int position = 0;
    // flag that indicates if reaction has rate or not
    bool has_rate = false;
    // "greater-than-sign" counter
    int sign_count = 0;
    // container for current molecule name
    String id;
    // flag that indicates if the current reaction is valid or not
    bool validator = true;
    
    // iterate through the string
    Reaction reaction;
    const char* current = code.c_str();
    while (current != current + code.length())
    {
        // end of reaction sign
        if (*current == ';')
        {
            if (has_rate)
            {
                // save reaction rule
                reaction.m_rules.push_back(local);
                // reset variables
                std::fill(local.begin(), local.end(), 0);
                validator = true;
                position = 0;
                sign_count = 0;
            }
            else
            {
                validator = false;
                Log::warning("Reaction has no rate.");
            }
            //
            ++current;
        }
        // whitespace or current reaction is not valid
        else if(*current == ' ' || *current == '\n' || *current == '\r' || *current == '\t' || *current == '\v' || !validator)
        {
            ++current;
        }
        // reaction rate sign
        else if (*current == '>')
        {
            if (sign_count == 1)
            {
                ++sign_count;
                position = 2;
            }
            else if (sign_count == 2)
            {
                validator = false;
                Log::warning("Invalid reaction. Too many '>' signs");
            }
            else
            {
                // add last molecule type and reset container
                if (id.size() == 0)
                {
                    validator = false;
                    Log::warning("Couldnt parse reaction");
                }
                else
                {
                    unsigned int index = reaction.get_index_of(id);
                    if (index > local.size())
                        local.push_back(-1);
                    else
                        local[index] -= 1;
                    id.clear();
                    // get reaction rate using strtof
                    char* end;
                    float rate = strtof(current, &end);
                    if (current == end)
                    {
                        validator = false;
                        Log::warning("Reaction has invalid rate.");
                    }
                    current = end;
                    // add reaction rate
                    reaction.m_rates.push_back(rate);
                    has_rate = true;
                    ++sign_count;
                }
            }
            //
            ++current;
        }
        // and sign
        else if (*current == '+')
        {
            if (id.size() == 0)
            {
                validator = false;
                Log::warning("Couldnt parse reaction");
            }
            else
            {
                // add last molecule type and reset container
                unsigned int index = reaction.get_index_of(id);
                if (index > local.size())
                {
                    // extend array
                    if (position == 0)
                        local.push_back(-1);
                    else
                        local.push_back(1);
                }
                else
                {
                    // edit array
                    if (position == 0)
                        local[index] -= 1;
                    else
                        local[index] += 1;
                }
                id.clear();
            }
            //
            ++current;
        }
        // other characters = names of molecules
        else
        {
            // other characters are located in reaction rate area
            if (position == 1)
            {
                validator = false;
                Log::warning("Reaction has invalid reaction rate.");
            }
            else
            {
                id += *current;
            }
            //
            ++current;
        }
    }
    return reaction;
}

/* ************************************************************************ */

class StochasticReactionsApi : public simulator::LibraryApi
{
    Program createProgram(Simulation& simulation, const String& name, String code = {}) override
    {
        return parseReactionCode(code);
    }
};

/* ************************************************************************ */

DEFINE_LIBRARY(stochastic_reactions, StochasticReactionsApi)

/* ************************************************************************ */