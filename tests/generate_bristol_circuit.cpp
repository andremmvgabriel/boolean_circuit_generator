#include <UnsignedVariable.hpp>
#include <BristolCircuitGenerator.hpp>

void test_xor() {
    gabe::circuits::generator::BristolGenerator circuit_generator(
        "BristolCircuit_XOR.txt",
        { 8, 8 },
        { 8 }
    );

    gabe::circuits::UnsignedVariable input1(8);
    gabe::circuits::UnsignedVariable input2(8);
    gabe::circuits::UnsignedVariable output(8);

    circuit_generator.add_input(input1);
    circuit_generator.add_input(input2);

    circuit_generator.start();

    circuit_generator.XOR(input1, input2, output);

    circuit_generator.conclude();

    circuit_generator.add_output(output);
}

void test_and() {
    gabe::circuits::generator::BristolGenerator circuit_generator(
        "BristolCircuit_AND.txt",
        { 8, 8 },
        { 8 }
    );

    gabe::circuits::UnsignedVariable input1(8);
    gabe::circuits::UnsignedVariable input2(8);
    gabe::circuits::UnsignedVariable output(8);

    circuit_generator.add_input(input1);
    circuit_generator.add_input(input2);

    circuit_generator.start();

    circuit_generator.AND(input1, input2, output);

    circuit_generator.conclude();

    circuit_generator.add_output(output);
}

void test_inv() {
    gabe::circuits::generator::BristolGenerator circuit_generator(
        "BristolCircuit_INV.txt",
        { 8 },
        { 8 }
    );

    gabe::circuits::UnsignedVariable input(8);
    gabe::circuits::UnsignedVariable output(8);

    circuit_generator.add_input(input);

    circuit_generator.start();

    circuit_generator.INV(input, output);

    circuit_generator.conclude();

    circuit_generator.add_output(output);
}

void test_or() {
    gabe::circuits::generator::BristolGenerator circuit_generator(
        "BristolCircuit_OR.txt",
        { 8, 8 },
        { 8 }
    );

    gabe::circuits::UnsignedVariable input1(8);
    gabe::circuits::UnsignedVariable input2(8);
    gabe::circuits::UnsignedVariable output(8);

    circuit_generator.add_input(input1);
    circuit_generator.add_input(input2);

    circuit_generator.start();

    circuit_generator.OR(input1, input2, output);

    circuit_generator.conclude();

    circuit_generator.add_output(output);
}

void test_addition() {
    gabe::circuits::generator::BristolGenerator circuit_generator(
        "BristolCircuit_addition.txt",
        { 4, 4 },
        { 4 }
    );

    gabe::circuits::UnsignedVariable input1(4);
    gabe::circuits::UnsignedVariable input2(4);
    gabe::circuits::UnsignedVariable output(4);

    circuit_generator.add_input(input1);
    circuit_generator.add_input(input2);

    circuit_generator.start();

    circuit_generator.addition(input1, input2, output);

    circuit_generator.conclude();

    circuit_generator.add_output(output);
}

void test_subtraction() {
    gabe::circuits::generator::BristolGenerator circuit_generator(
        "BristolCircuit_subtraction.txt",
        { 8, 8 },
        { 8 }
    );

    gabe::circuits::UnsignedVariable input1(8);
    gabe::circuits::UnsignedVariable input2(8);
    gabe::circuits::UnsignedVariable output(8);

    circuit_generator.add_input(input1);
    circuit_generator.add_input(input2);

    circuit_generator.start();

    circuit_generator.subtraction(input1, input2, output);

    circuit_generator.conclude();

    circuit_generator.add_output(output);
}

void test_multiplication() {
    gabe::circuits::generator::BristolGenerator circuit_generator(
        "BristolCircuit_multiplication.txt",
        { 4, 4 },
        { 8 }
    );

    gabe::circuits::UnsignedVariable input1(4);
    gabe::circuits::UnsignedVariable input2(4);
    gabe::circuits::UnsignedVariable output(8);

    circuit_generator.add_input(input1);
    circuit_generator.add_input(input2);

    circuit_generator.start();

    circuit_generator.multiplication(input1, input2, output);

    circuit_generator.conclude();

    circuit_generator.add_output(output);
}

void test_multiplication_signed() {
    gabe::circuits::generator::BristolGenerator circuit_generator(
        "BristolCircuit_multiplication_signed.txt",
        { 6, 4 },
        { 9 }
    );

    gabe::circuits::SignedVariable input1(6);
    gabe::circuits::SignedVariable input2(4);
    gabe::circuits::SignedVariable output(9);

    circuit_generator.add_input(input1);
    circuit_generator.add_input(input2);

    circuit_generator.start();

    circuit_generator.multiplication(input1, input2, output);

    circuit_generator.INV(output, output);
    circuit_generator.INV(output, output);

    circuit_generator.conclude();

    circuit_generator.add_output(output);
}

void test_division() {
    gabe::circuits::generator::BristolGenerator circuit_generator(
        "BristolCircuit_division.txt",
        { 8, 8 },
        { 8, 8 }
    );

    gabe::circuits::UnsignedVariable input1(8);
    gabe::circuits::UnsignedVariable input2(8);
    gabe::circuits::UnsignedVariable output_q(8);
    gabe::circuits::UnsignedVariable output_r(8);

    circuit_generator.add_input(input1);
    circuit_generator.add_input(input2);

    circuit_generator.start();

    circuit_generator.division(input1, input2, output_q, output_r);

    // DELETE THIS - ONLY HERE TO MAKE SURE THE OUTPUTS ARE THE VERY LAST WIRES
    circuit_generator.INV(output_q, output_q);
    circuit_generator.INV(output_r, output_r);
    circuit_generator.INV(output_q, output_q);
    circuit_generator.INV(output_r, output_r);

    circuit_generator.conclude();

    circuit_generator.add_output(output_q);
    circuit_generator.add_output(output_r);
}

void test_division_quotient() {
    gabe::circuits::generator::BristolGenerator circuit_generator(
        "BristolCircuit_division_quotient.txt",
        { 8, 8 },
        { 8 }
    );

    gabe::circuits::UnsignedVariable input1(8);
    gabe::circuits::UnsignedVariable input2(8);
    gabe::circuits::UnsignedVariable output_q(8);

    circuit_generator.add_input(input1);
    circuit_generator.add_input(input2);

    circuit_generator.start();

    circuit_generator.division_quotient(input1, input2, output_q);

    // DELETE THIS - ONLY HERE TO MAKE SURE THE OUTPUTS ARE THE VERY LAST WIRES
    circuit_generator.INV(output_q, output_q);
    circuit_generator.INV(output_q, output_q);

    circuit_generator.conclude();

    circuit_generator.add_output(output_q);
}

void test_division_remainder() {
    gabe::circuits::generator::BristolGenerator circuit_generator(
        "BristolCircuit_division_remainder.txt",
        { 8, 8 },
        { 8 }
    );

    gabe::circuits::UnsignedVariable input1(8);
    gabe::circuits::UnsignedVariable input2(8);
    gabe::circuits::UnsignedVariable output_r(8);

    circuit_generator.add_input(input1);
    circuit_generator.add_input(input2);

    circuit_generator.start();

    circuit_generator.division_remainder(input1, input2, output_r);

    // DELETE THIS - ONLY HERE TO MAKE SURE THE OUTPUTS ARE THE VERY LAST WIRES
    circuit_generator.INV(output_r, output_r);
    circuit_generator.INV(output_r, output_r);

    circuit_generator.conclude();

    circuit_generator.add_output(output_r);
}

void test_multiplexer() {
    gabe::circuits::generator::BristolGenerator circuit_generator(
        "BristolCircuit_multiplexer.txt",
        { 1, 8, 8 },
        { 8 }
    );

    gabe::circuits::UnsignedVariable control(1);
    gabe::circuits::UnsignedVariable input1(8);
    gabe::circuits::UnsignedVariable input2(8);
    gabe::circuits::UnsignedVariable output(8);

    circuit_generator.add_input(control);
    circuit_generator.add_input(input1);
    circuit_generator.add_input(input2);

    circuit_generator.start();

    circuit_generator.multiplexer( control, input1, input2, output );

    circuit_generator.conclude();

    circuit_generator.add_output(output);
}

void test_equal() {
    gabe::circuits::generator::BristolGenerator circuit_generator(
        "BristolCircuit_equal.txt",
        { 8, 8 },
        { 1 }
    );

    gabe::circuits::UnsignedVariable input1(8);
    gabe::circuits::UnsignedVariable input2(8);
    gabe::circuits::UnsignedVariable output(1);

    circuit_generator.add_input(input1);
    circuit_generator.add_input(input2);

    circuit_generator.start();

    circuit_generator.equal( input1, input2, output );

    circuit_generator.conclude();

    circuit_generator.add_output(output);
}

void test_greater() {
    gabe::circuits::generator::BristolGenerator circuit_generator(
        "BristolCircuit_greater.txt",
        { 8, 8 },
        { 1 }
    );

    gabe::circuits::UnsignedVariable input1(8);
    gabe::circuits::UnsignedVariable input2(8);
    gabe::circuits::UnsignedVariable output(1);

    circuit_generator.add_input(input1);
    circuit_generator.add_input(input2);

    circuit_generator.start();

    circuit_generator.greater( input1, input2, output );

    circuit_generator.conclude();

    circuit_generator.add_output(output);
}

void test_smaller() {
    gabe::circuits::generator::BristolGenerator circuit_generator(
        "BristolCircuit_smaller.txt",
        { 8, 8 },
        { 1 }
    );

    gabe::circuits::UnsignedVariable input1(8);
    gabe::circuits::UnsignedVariable input2(8);
    gabe::circuits::UnsignedVariable output(1);

    circuit_generator.add_input(input1);
    circuit_generator.add_input(input2);

    circuit_generator.start();

    circuit_generator.smaller( input1, input2, output );

    circuit_generator.conclude();

    circuit_generator.add_output(output);
}

void test_greater_or_equal() {
    gabe::circuits::generator::BristolGenerator circuit_generator(
        "BristolCircuit_greater_or_equal.txt",
        { 8, 8 },
        { 1 }
    );

    gabe::circuits::UnsignedVariable input1(8);
    gabe::circuits::UnsignedVariable input2(8);
    gabe::circuits::UnsignedVariable output(1);

    circuit_generator.add_input(input1);
    circuit_generator.add_input(input2);

    circuit_generator.start();

    circuit_generator.greater_or_equal( input1, input2, output );

    circuit_generator.conclude();

    circuit_generator.add_output(output);
}

void test_smaller_or_equal() {
    gabe::circuits::generator::BristolGenerator circuit_generator(
        "BristolCircuit_smaller_or_equal.txt",
        { 8, 8 },
        { 1 }
    );

    gabe::circuits::UnsignedVariable input1(8);
    gabe::circuits::UnsignedVariable input2(8);
    gabe::circuits::UnsignedVariable output(1);

    circuit_generator.add_input(input1);
    circuit_generator.add_input(input2);

    circuit_generator.start();

    circuit_generator.smaller_or_equal( input1, input2, output );

    circuit_generator.conclude();

    circuit_generator.add_output(output);
}

void test_comparator() {
    gabe::circuits::generator::BristolGenerator circuit_generator(
        "BristolCircuit_comparator.txt",
        { 8, 8 },
        { 1, 1, 1 }
    );

    gabe::circuits::UnsignedVariable input1(8);
    gabe::circuits::UnsignedVariable input2(8);
    gabe::circuits::UnsignedVariable output_equal(1);
    gabe::circuits::UnsignedVariable output_greater(1);
    gabe::circuits::UnsignedVariable output_smaller(1);

    circuit_generator.add_input(input1);
    circuit_generator.add_input(input2);

    circuit_generator.start();

    circuit_generator.comparator( input1, input2, output_equal, output_greater, output_smaller );

    circuit_generator.conclude();

    circuit_generator.add_output(output_equal);
    circuit_generator.add_output(output_greater);
    circuit_generator.add_output(output_smaller);
}

int main() {
    //test_xor();
    //test_and();
    //test_inv();
    //test_or();
    //test_addition();
    //test_subtraction();
    //test_multiplication();
    test_multiplication_signed();
    test_division();
    test_division_quotient();
    test_division_remainder();
    //test_multiplexer();
    //test_equal();
    //test_greater();
    //test_smaller();
    //test_greater_or_equal();
    //test_smaller_or_equal();
    //test_comparator();

    return 0;
}