#pragma once

#include <stdint.h>

class Variable
{
private:
    uint8_t* _wires;

public:
    const int number_wires;

public:
    Variable() : number_wires(0) {}
    Variable(int number_bytes) : number_wires(8 * number_bytes) {
        _wires = new uint8_t[8*number_bytes];
    }

    ~Variable() { delete[] _wires; }
};
