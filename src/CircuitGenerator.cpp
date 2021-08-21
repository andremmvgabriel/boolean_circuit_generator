#include <CircuitGenerator.hpp>

CircuitGenerator::CircuitGenerator() {}

CircuitGenerator::CircuitGenerator(CircuitType circuit_type, const std::vector<int> &input_parties_wires, const std::vector<int> &output_parties_wires) : _circuit_type(circuit_type), _inputs_number_wires(input_parties_wires), _outputs_number_wires(output_parties_wires) {
    printf("> Circuit Generator object created.\n");
    printf("   - Target type: %s.\n\n",
        circuit_type == CircuitType::BRISTOL ? "Bristol" : "None"
    );

    _circuit_file = std::ofstream(
        "BristolCircuit.txt",
        std::ios::out
    );

    _temp_circuit_file = std::fstream(
        "TEMP_BristolCircuit.txt",
        std::ios::in | std::ios::out | std::ios::trunc
    );

    for (auto & amount : input_parties_wires) {
        _number_wires += amount;
    }
}

Variable CircuitGenerator::create_constant(int n_bits, uint64_t value) {
    //
    Variable variable (n_bits);

    for (int i = 0; i < n_bits; i++) {
        variable.wires[i].label = ((value >> i) & 0x01) == 0 ? zero_wire : one_wire;
        // variable.wires[i].value = 0;
    }

    return variable;
}
