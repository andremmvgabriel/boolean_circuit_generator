#include <iostream>

#include <CircuitGenerator.hpp>
#include <Variable.hpp>

void test_addition() {
    CircuitGenerator circuit_generator(
        CircuitType::BRISTOL,
        {8, 8},
        {8}
    );

    gabe::circuits::Variable input1(8);
    gabe::circuits::Variable input2(8);
    gabe::circuits::Variable output(8);

    circuit_generator.add_input(input1);
    circuit_generator.add_input(input2);

    circuit_generator.start();

    circuit_generator.addition( input1, input2, output );

    circuit_generator.conclude();
}

void test_subtraction() {
    CircuitGenerator circuit_generator(
        CircuitType::BRISTOL,
        {8, 8},
        {8}
    );

    gabe::circuits::Variable input1(8);
    gabe::circuits::Variable input2(8);
    gabe::circuits::Variable output(8);

    circuit_generator.add_input(input1);
    circuit_generator.add_input(input2);

    circuit_generator.start();

    circuit_generator.subtraction( input1, input2, output );

    circuit_generator.conclude();
}

void test_multiplication() {
    CircuitGenerator circuit_generator(
        CircuitType::BRISTOL,
        {4, 4},
        {8}
    );

    gabe::circuits::Variable input1(4);
    gabe::circuits::Variable input2(4);
    gabe::circuits::Variable output(8);

    circuit_generator.add_input(input1);
    circuit_generator.add_input(input2);

    circuit_generator.start();

    circuit_generator.multiplication( input1, input2, output );

    circuit_generator.conclude();
}

void test_equal() {
    CircuitGenerator circuit_generator(
        CircuitType::BRISTOL,
        {8, 8},
        {1}
    );

    gabe::circuits::Variable input1(8);
    gabe::circuits::Variable input2(8);
    gabe::circuits::Variable output(1);

    circuit_generator.add_input(input1);
    circuit_generator.add_input(input2);

    circuit_generator.start();

    circuit_generator.equal( input1, input2, output );

    circuit_generator.conclude();
}

void test_multiplexer() {
    CircuitGenerator circuit_generator(
        CircuitType::BRISTOL,
        {1, 8, 8},
        {8}
    );

    gabe::circuits::Variable control(1);
    gabe::circuits::Variable input1(8);
    gabe::circuits::Variable input2(8);
    gabe::circuits::Variable output(8);

    circuit_generator.add_input(control);
    circuit_generator.add_input(input1);
    circuit_generator.add_input(input2);

    circuit_generator.start();

    circuit_generator.multiplexer( control, input1, input2, output );

    circuit_generator.conclude();
}

void test_greater() {
    CircuitGenerator circuit_generator(
        CircuitType::BRISTOL,
        {4, 4},
        {1}
    );

    gabe::circuits::Variable input1(4);
    gabe::circuits::Variable input2(4);
    gabe::circuits::Variable output(1);

    circuit_generator.add_input(input1);
    circuit_generator.add_input(input2);

    circuit_generator.start();

    circuit_generator.greater( input1, input2, output );

    circuit_generator.conclude();
}

void test_smaller() {
    CircuitGenerator circuit_generator(
        CircuitType::BRISTOL,
        {4, 4},
        {1}
    );

    gabe::circuits::Variable input1(4);
    gabe::circuits::Variable input2(4);
    gabe::circuits::Variable output(1);

    circuit_generator.add_input(input1);
    circuit_generator.add_input(input2);

    circuit_generator.start();

    circuit_generator.smaller( input1, input2, output );

    circuit_generator.conclude();
}

void test_greater_or_equal() {
    CircuitGenerator circuit_generator(
        CircuitType::BRISTOL,
        {4, 4},
        {1}
    );

    gabe::circuits::Variable input1(4);
    gabe::circuits::Variable input2(4);
    gabe::circuits::Variable output(1);

    circuit_generator.add_input(input1);
    circuit_generator.add_input(input2);

    circuit_generator.start();

    circuit_generator.greater_or_equal( input1, input2, output );

    circuit_generator.conclude();
}

void test_smaller_or_equal() {
    CircuitGenerator circuit_generator(
        CircuitType::BRISTOL,
        {4, 4},
        {1}
    );

    gabe::circuits::Variable input1(4);
    gabe::circuits::Variable input2(4);
    gabe::circuits::Variable output(1);

    circuit_generator.add_input(input1);
    circuit_generator.add_input(input2);

    circuit_generator.start();

    circuit_generator.smaller_or_equal( input1, input2, output );

    circuit_generator.conclude();
}

void test_libscapi() {
    CircuitGenerator circuit_generator(
        CircuitType::BRISTOL,
        {4, 4},
        {4}
    );

    gabe::circuits::Variable input1(4);
    gabe::circuits::Variable input2(4);
    gabe::circuits::Variable output(4);

    circuit_generator.add_input(input1);
    circuit_generator.add_input(input2);

    circuit_generator.start();

    circuit_generator.addition( input1, input2, output );

    circuit_generator.conclude();

    circuit_generator.add_output(output);
}

#include <BristolCircuitGenerator.hpp>

int main() {
    //test_addition();
    //test_subtraction();
    //test_multiplication();
    //test_equal();
    //test_multiplexer();
    //test_greater();
    //test_smaller();
    //test_greater_or_equal();
    //test_smaller_or_equal();
    test_libscapi();

    gabe::circuits::generator::BristolGenerator test(
        "BristolCircuit",
        { 4, 4 }, // Number wires in each input party
        { 4 } // Number wires in each output party
    );
    gabe::circuits::Wire wire1; wire1.label=1;
    gabe::circuits::Wire wire2; wire2.label=2;
    gabe::circuits::Wire wire3;
    test._xor_gate(wire1, wire2, wire3);
    test._inv_gate(wire1, wire3);

    return 0;
}