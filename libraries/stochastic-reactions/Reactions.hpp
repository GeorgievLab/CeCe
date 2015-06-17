#pragma once

#include "simulator/Object.hpp"
#include "core/Units.hpp"

class Reactions
{
    public:
    void operator()(simulator::Object& object, units::Duration);
};