#pragma once

#include <stdint.h>

enum class CircuitType
{
    BRISTOL
};

class Circuit
{
protected:
    CircuitType _circuit_type;

    uint64_t _zero_wire;
    uint64_t _one_wire;

public:
    virtual ~Circuit() {}
};
