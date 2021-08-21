#include <Variable.hpp>

Variable::Variable() {}

Variable::Variable(uint8_t number_bits) : number_wires(number_bits) {
    wires.resize(number_bits);
    wires2.resize(number_bits);
}

Variable::~Variable() {}
