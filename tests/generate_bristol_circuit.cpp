#include <iostream>

#include <CircuitGenerator.hpp>
#include <Variable.hpp>

void test_addition() {
    CircuitGenerator circuit_generator(
        CircuitType::BRISTOL,
        {8, 8},
        {8}
    );

    Variable input1(8);
    Variable input2(8);
    Variable output(8);

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

    Variable input1(8);
    Variable input2(8);
    Variable output(8);

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

    Variable input1(4);
    Variable input2(4);
    Variable output(8);

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

    Variable input1(8);
    Variable input2(8);
    Variable output(1);

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

    Variable control(1);
    Variable input1(8);
    Variable input2(8);
    Variable output(8);

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

    Variable input1(4);
    Variable input2(4);
    Variable output(1);

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

    Variable input1(4);
    Variable input2(4);
    Variable output(1);

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

    Variable input1(4);
    Variable input2(4);
    Variable output(1);

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

    Variable input1(4);
    Variable input2(4);
    Variable output(1);

    circuit_generator.add_input(input1);
    circuit_generator.add_input(input2);

    circuit_generator.start();

    circuit_generator.smaller_or_equal( input1, input2, output );

    circuit_generator.conclude();
}

int main() {
    //test_addition();
    //test_subtraction();
    //test_multiplication();
    //test_equal();
    //test_multiplexer();
    //test_greater();
    //test_smaller();
    //test_greater_or_equal();
    test_smaller_or_equal();

    return 0;
}