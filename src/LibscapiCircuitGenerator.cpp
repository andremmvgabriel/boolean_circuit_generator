#include <LibscapiCircuitGenerator.hpp>

gabe::circuits::generator::LibscapiGenerator::LibscapiGenerator() : CircuitGenerator() {}

gabe::circuits::generator::LibscapiGenerator::LibscapiGenerator(const std::string& circuit_file, const std::vector<uint64_t>& number_wires_input_parties, const std::vector<uint64_t>& number_wires_output_parties) : CircuitGenerator(circuit_file, number_wires_input_parties, number_wires_output_parties) {
    // Assigns the correct gates
    _gates_map["xor"] = "0110";
    _gates_map["and"] = "0001";
    _gates_map["inv"] = "10";
    _gates_map["or"] = "0111";
}

void gabe::circuits::generator::LibscapiGenerator::_write_header() {
    std::string sbreak = "\n";

    std::string header = std::to_string(_counter_gates) + " " + std::to_string(_counter_wires);

    std::string inputs = std::to_string(_number_wires_input_parties.size());
    for (auto & amount : _number_wires_input_parties) {
        inputs += " " + std::to_string(amount);
    }

    std::string outputs = std::to_string(_number_wires_output_parties.size());
    for (auto & amount : _number_wires_output_parties) {
        outputs += " " + std::to_string(amount);
    }

    // Writes the header
    _circuit_file.write( (header + sbreak).c_str(), (header + sbreak).size() );
    _circuit_file.write( (inputs + sbreak).c_str(), (inputs + sbreak).size() );
    _circuit_file.write( (outputs + sbreak).c_str(), (outputs + sbreak).size() );
    _circuit_file.write( sbreak.c_str(), sbreak.size() );
}
