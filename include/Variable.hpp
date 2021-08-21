#pragma once

#include <stdint.h>
#include <vector>

#include <Wire.hpp>

class Variable
{
public:
    std::vector<Wire> wires;
    uint8_t number_wires = 0;

public:
    Variable();
    Variable(uint8_t n_bits);

    virtual ~Variable();
};
