#pragma once

#include <stdint.h>

class Variable
{
private:
    uint8_t* _wires;

public:
    Variable() {}
    Variable(int number_bytes) {
        _wires = new uint8_t[8*number_bytes];
    }

    ~Variable() { delete[] _wires; }
};
