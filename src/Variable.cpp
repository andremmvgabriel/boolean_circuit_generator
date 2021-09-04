#include <Variable.hpp>

gabe::circuits::Variable::Variable() {}

gabe::circuits::Variable::Variable(uint8_t number_bits) : number_wires(number_bits) {
    wires.resize(number_bits);
}

gabe::circuits::Variable::~Variable() {}
