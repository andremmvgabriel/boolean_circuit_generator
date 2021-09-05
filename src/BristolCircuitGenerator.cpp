#include <BristolCircuitGenerator.hpp>

gabe::circuits::generator::BristolGenerator::BristolGenerator() : CircuitGenerator() {}

gabe::circuits::generator::BristolGenerator::BristolGenerator(const std::string& circuit_file, const std::vector<uint64_t>& number_wires_input_parties, const std::vector<uint64_t>& number_wires_output_parties) : CircuitGenerator(circuit_file, number_wires_input_parties, number_wires_output_parties) {
    // Assigns the correct gates
    _gates_map["xor"] = "XOR";
    _gates_map["and"] = "AND";
    _gates_map["inv"] = "INV";
    _gates_map["or"] = "OR";
}

void gabe::circuits::generator::BristolGenerator::_write_header() {
    std::string sbreak = "\n";

    std::string header = std::to_string(_counter_gates) + " " + std::to_string(_counter_wires) + sbreak;

    std::string inputs = std::to_string(_number_wires_input_parties.size());
    for (auto & amount : _number_wires_input_parties) {
        inputs += " " + std::to_string(amount);
    }
    inputs += sbreak;

    std::string outputs = std::to_string(_number_wires_output_parties.size());
    for (auto & amount : _number_wires_output_parties) {
        outputs += " " + std::to_string(amount);
    }
    outputs += sbreak;

    // Writes the header
    _circuit_file.write( header.c_str(), header.size() );
    _circuit_file.write( inputs.c_str(), inputs.size() );
    _circuit_file.write( outputs.c_str(), outputs.size() );
    _circuit_file.write( sbreak.c_str(), sbreak.size() );
}
