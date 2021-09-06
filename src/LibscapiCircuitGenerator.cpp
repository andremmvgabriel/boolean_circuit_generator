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

    std::string header = std::to_string(_counter_gates) + " " + std::to_string(_number_wires_input_parties.size()) + sbreak + sbreak;

    uint64_t assigned_inputs = 0x00;
    std::string inputs;
    for (int i = 0; i < _number_wires_input_parties.size(); i++) {
        inputs += std::to_string(i + 1) + " " + std::to_string(_number_wires_input_parties.at(i)) + sbreak;
        for (uint64_t j = 0; j < _number_wires_input_parties.at(i); j++) {
            inputs += std::to_string(j + assigned_inputs) + sbreak;
        }
        inputs += sbreak;
        assigned_inputs += _number_wires_input_parties.at(i);
    }

    uint64_t wires_to_assign = 0x00;
    for (auto & amount : _number_wires_output_parties) { wires_to_assign += amount; }
    std::string outputs;
    int counter = 1;
    for (auto & amount : _number_wires_output_parties) {
        outputs += std::to_string(counter) + " " + std::to_string(amount) + sbreak;

        // Assigns the wires values
        for (uint64_t i = amount; i > 0; i--) {
            outputs += std::to_string( _counter_wires - wires_to_assign + amount - i ) + sbreak;
        }

        outputs += sbreak;

        wires_to_assign -= amount;

        counter++;
    } 

    // Writes the header
    _circuit_file.write( header.c_str(), header.size() );
    _circuit_file.write( inputs.c_str(), inputs.size() );
    _circuit_file.write( outputs.c_str(), outputs.size() );
}
