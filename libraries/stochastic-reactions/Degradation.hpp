#pragma once

#include "simulator/Object.hpp"
#include "core/Units.hpp"

class Degradation
{
    public:
    void operator()(simulator::Object& object, units::Duration);
};