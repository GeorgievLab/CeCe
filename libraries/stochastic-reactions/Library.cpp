
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

Reaction par(String code)
{
    Reaction reaction;
    char* current = code.begin();
    DynamicArray<int> local (m_ids.size());
    bool flag_minus = true;
    String id = "";
    while (current != code.end())
    {
        if(*current == " " || *current == "\n" || *current == "\r")
        {
            ++current;
        }
        else if (*current == ";")
        {
            reaction.m_rules.push_back(local);
            std::fill(local.begin(), local.end(), 0);
            flag_minus = true;
            ++current;
        }
        else if (*current == ">")
        {
            local[get_index_of(id)] -= 1;
            id = "";
            char* end;
            float rate = strtof(current, &end);
            if (current == end)
            {
                throw std::runtime_error("Reaction has no rate");
            }
            reaction.m_rates.push_back(rate);
            flag_minus = false;
            ++current;
        }
        else if (*current == "+")
        {
            if (minus)
                local[get_index_of(id)] -= 1;
            else
                local[get_index_of(id)] += 1;
            id = "";
            ++current;
        }
        else
        {
            id += *current;
        }
    }
    return reaction();
}

/* ************************************************************************ */

int get_index_of(String id)
{
    auto pointer = std::find(m_ids.begin(), m_ids.end(), id);
    if (pointer == m_ids.end())
    {
        for(auto& rule : m_rules)
        {
            rule.push_back(0);
        }
        m_ids.push_back(id);
        return m_ids.size() - 1;
    }
    return std::distance(m_ids.begin(), pointer);
}

/* ************************************************************************ */

class StochasticReactionsApi : public simulator::LibraryApi
{
    Program createProgram(Simulation& simulation, const String& name, String code = {}) override
    {
        return par(code);
    }
};

/* ************************************************************************ */

DEFINE_LIBRARY(stochastic_reactions, StochasticReactionsApi)

/* ************************************************************************ */