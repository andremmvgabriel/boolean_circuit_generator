#include <iostream>

#include <CircuitGenerator.hpp>
#include <Variable.hpp>

int main() {
    CircuitGenerator circuit_generator(CircuitType::BRISTOL);

    Variable input1;
    Variable input2;
    Variable output;

    circuit_generator.add_input(input1);
    circuit_generator.add_input(input2);

    circuit_generator.start();

    circuit_generator.add( input1, input2, output );

    circuit_generator.conclude();

    circuit_generator.add_output(output);

    return 0;
}