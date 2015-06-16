#pragma once

#include "simulator/Object.hpp"
#include "core/Units.hpp"

class Disassembly
{
    public:
    void operator()(simulator::Object& object, units::Duration);
};