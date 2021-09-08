#include <BristolCircuitGenerator.hpp>
#include <LibscapiCircuitGenerator.hpp>

void generate_bristol() {
    gabe::circuits::generator::BristolGenerator circuit_generator(
        "BristolCircuit_average.txt",
        { 8, 8 },
        { 8 }
    );

    gabe::circuits::Variable input1(8);
    gabe::circuits::Variable input2(8);
    gabe::circuits::Variable average(8);

    circuit_generator.add_input(input1);
    circuit_generator.add_input(input2);

    circuit_generator.start();

    gabe::circuits::Variable n_inputs = circuit_generator.create_constant(8, 0x02);

    circuit_generator.addition( input1, input2, average );
    circuit_generator.division_quotient( average, n_inputs, average );

    // DELETE THIS - ONLY HERE TO MAKE SURE THE OUTPUTS ARE THE VERY LAST WIRES
    circuit_generator.INV(average, average);
    circuit_generator.INV(average, average);

    circuit_generator.conclude();

    circuit_generator.add_output(average);
}

void generate_libscapi() {
    gabe::circuits::generator::LibscapiGenerator circuit_generator(
        "LibscapiCircuit_average.txt",
        { 8, 8 },
        { 8 }
    );

    gabe::circuits::Variable input1(8);
    gabe::circuits::Variable input2(8);
    gabe::circuits::Variable average(8);

    circuit_generator.add_input(input1);
    circuit_generator.add_input(input2);

    circuit_generator.start();

    gabe::circuits::Variable n_inputs = circuit_generator.create_constant(8, 0x02);

    circuit_generator.addition( input1, input2, average );
    circuit_generator.division_quotient( average, n_inputs, average );

    // DELETE THIS - ONLY HERE TO MAKE SURE THE OUTPUTS ARE THE VERY LAST WIRES
    circuit_generator.INV(average, average);
    circuit_generator.INV(average, average);

    circuit_generator.conclude();

    circuit_generator.add_output(average);
}

int main() {
    generate_bristol();
    generate_libscapi();
    return 0;
}