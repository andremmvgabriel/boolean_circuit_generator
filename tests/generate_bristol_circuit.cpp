#include <iostream>

#include <CircuitGenerator.hpp>
#include <Variable.hpp>

int main() {
    CircuitGenerator circuit_generator(
        CircuitType::BRISTOL,
        /* {8, 8},
        {8} */
        {4, 4},
        {8}
    );

    /* Variable input1(8);
    Variable input2(8);
    Variable output(8); */

    Variable input1(4);
    Variable input2(4);
    Variable output(8);

    circuit_generator.add_input(input1);
    circuit_generator.add_input(input2);

    circuit_generator.start();

    /* circuit_generator.XOR(output, test, test);
    circuit_generator.INV(test, test);
    circuit_generator.AND(output, test, output);
    circuit_generator.OR(output, test, output); */

    //circuit_generator.addition( input1, input2, output );
    //circuit_generator.subtraction( input1, input2, output );
    circuit_generator.multiplication( input1, input2, output );

    circuit_generator.conclude();

    //circuit_generator.add_output(output);

    return 0;
}