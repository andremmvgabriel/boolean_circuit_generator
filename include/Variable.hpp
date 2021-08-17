#pragma once

#include <stdint.h>
#include <vector>

class Variable
{
public:
    std::vector<uint64_t> wires;
    uint8_t number_wires;

public:
    Variable() : number_wires(0) {}
    Variable(uint8_t n_bits) : number_wires(n_bits) { wires.resize(n_bits); }

    virtual ~Variable() {}
};
