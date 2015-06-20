
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

// Reactions
#include "Reaction.hpp"

/************************************************************************** */

using namespace simulator;

/* ************************************************************************ */

Reaction parseReactionCode(String code)
{
    Reaction reaction;
    char* current = &*code.begin();
    DynamicArray<int> local (1);
    bool flag_minus = true;
    bool flag_rate = false;
    String id = "";
    while (current != &*code.end())
    {
        if(*current == ' ' || *current == '\n' || *current == '\r' || *current == '\t' || flag_rate)
        {
            ++current;
        }
        else if (*current == ';')
        {
            reaction.m_rules.push_back(local);
            std::fill(local.begin(), local.end(), 0);
            flag_minus = true;
            ++current;
        }
        else if (*current == '>')
        {
            if (flag_rate)
            {
                flag_rate = false;
                ++current;
                continue;
            }
            local[reaction.get_index_of(id)] -= 1;
            id = "";
            char* end;
            float rate = strtof(current, &end);
            if (current == end)
            {
                throw std::runtime_error("Reaction has no rate");
            }
            current = end;
            reaction.m_rates.push_back(rate);
            flag_minus = false;
            flag_rate = true;
            ++current;
        }
        else if (*current == '+')
        {
            if (flag_minus)
                local[reaction.get_index_of(id)] -= 1;
            else
                local[reaction.get_index_of(id)] += 1;
            id = "";
            ++current;
        }
        else
        {
            id += *current;
        }
    }
    return Reaction();
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