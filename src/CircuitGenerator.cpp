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

    for (auto & amount : input_parties_wires) { _expected_input_wires += amount; }
    for (auto & amount : output_parties_wires) { _expected_output_wires += amount; }
}

gabe::circuits::Variable CircuitGenerator::create_constant(int n_bits, uint64_t value) {
    //
    gabe::circuits::Variable variable (n_bits);

    for (int i = 0; i < n_bits; i++) {
        variable.wires[i].label = ((value >> i) & 0x01) == 0 ? _zero.label : _one.label;
        // variable.wires[i].value = 0;
    }

    return variable;
}

void CircuitGenerator::add_input(gabe::circuits::Variable &input) {
    // Goes through all the input wires
    for (int i = 0; i < input.number_wires; i++) {
        // Safety check in case the user got mistaken with the number of input wires
        if ( _number_wires > _expected_input_wires) {
            throw std::runtime_error("There aren't enough input wires available to accept more input variables.");
        }

        // Assigns a label to the i-th input wire
        input.wires[i].label = _number_wires++;
    }
}

void CircuitGenerator::_xor_gate(const gabe::circuits::Wire& in1, const gabe::circuits::Wire& in2, gabe::circuits::Wire& out) {
    // Increase the counters
    _number_gates++;
    _counter_xor_gates++;

    // 
    std::string gate = "2 1 ";

    gate += in1.label < in2.label ? std::to_string(in1.label) + " " + std::to_string(in2.label) : std::to_string(in2.label) + " " + std::to_string(in1.label);

    out.label = _number_wires++;

    gate += " " + std::to_string(out.label);

    switch (_circuit_type) {
        case CircuitType::BRISTOL: gate += " XOR\n"; break;
        case CircuitType::BRISTOL_FASHION: gate += " XOR\n"; break;
        case CircuitType::LIBSCAPI: gate += " 0110\n"; break;
        default: break;
    }

    _temp_circuit_file.write(gate.c_str(), gate.size());
}

void CircuitGenerator::_inv_gate(const gabe::circuits::Wire& in, gabe::circuits::Wire& out) {
    // Increase the counters
    _number_gates++;
    _counter_inv_gates++;

    // 
    std::string gate = "1 1 ";

    gate += std::to_string(in.label);

    out.label = _number_wires++;

    gate += " " + std::to_string(out.label);

    switch (_circuit_type) {
        case CircuitType::BRISTOL: gate += " INV\n"; break;
        case CircuitType::BRISTOL_FASHION: gate += " INV\n"; break;
        case CircuitType::LIBSCAPI: gate += " 10\n"; break;
        default: break;
    }

    _temp_circuit_file.write(gate.c_str(), gate.size());
}

void CircuitGenerator::_and_gate(const gabe::circuits::Wire& in1, const gabe::circuits::Wire& in2, gabe::circuits::Wire& out) {
    // Increase the counters
    _number_gates++;
    _counter_and_gates++;

    std::string gate = "2 1 ";

    gate += in1.label < in2.label ? std::to_string(in1.label) + " " + std::to_string(in2.label) : std::to_string(in2.label) + " " + std::to_string(in1.label);

    out.label = _number_wires++;

    gate += " " + std::to_string(out.label);

    switch (_circuit_type) {
        case CircuitType::BRISTOL: gate += " AND\n"; break;
        case CircuitType::BRISTOL_FASHION: gate += " AND\n"; break;
        case CircuitType::LIBSCAPI: gate += " 0001\n"; break;
        default: break;
    }

    _temp_circuit_file.write(gate.c_str(), gate.size());
}

void CircuitGenerator::_or_gate(const gabe::circuits::Wire& in1, const gabe::circuits::Wire& in2, gabe::circuits::Wire& out) {
    // Increase the counters
    _number_gates++;
    _counter_or_gates++;

    std::string gate = "2 1 ";

    gate += in1.label < in2.label ? std::to_string(in1.label) + " " + std::to_string(in2.label) : std::to_string(in2.label) + " " + std::to_string(in1.label);

    out.label = _number_wires++;

    gate += " " + std::to_string(out.label);

    switch (_circuit_type) {
        case CircuitType::BRISTOL: gate += " OR\n"; break;
        case CircuitType::BRISTOL_FASHION: gate += " OR\n"; break;
        case CircuitType::LIBSCAPI: gate += " 0111\n"; break;
        default: break;
    }

    _temp_circuit_file.write(gate.c_str(), gate.size());
}

void CircuitGenerator::conclude() {
    printf("> Final circuit details:\n");
    printf("   - TOTAL gates: %lld\n", _number_gates);
    printf("   - TOTAL wires: %lld\n", _number_wires);
    printf("   - XOR gates: %lld\n", _counter_xor_gates);
    printf("   - AND gates: %lld\n", _counter_and_gates);
    printf("   - INV gates: %lld\n", _counter_inv_gates);
    printf("   - OR gates: %lld\n", _counter_or_gates);

    std::string header;
    switch (_circuit_type) {
        case CircuitType::BRISTOL: header = std::to_string(_number_gates) + " " + std::to_string(_number_wires) + "\n"; break;
        case CircuitType::BRISTOL_FASHION: header = std::to_string(_number_gates) + " " + std::to_string(_number_wires) + "\n"; break;
        case CircuitType::LIBSCAPI: header = std::to_string(_number_gates) + " " + std::to_string(_inputs_number_wires.size()) + "\n\n"; break;
    }
    _circuit_file.write(header.c_str(), header.size());

    std::string inputs_section;
    switch (_circuit_type) {
        case CircuitType::BRISTOL:
            inputs_section = std::to_string(_inputs_number_wires.size());
            for (auto & party_wires : _inputs_number_wires) {
                inputs_section += " " + std::to_string(party_wires);
            } inputs_section += "\n";
            _circuit_file.write(inputs_section.c_str(), inputs_section.size());
            break;

        case CircuitType::BRISTOL_FASHION:
            inputs_section = std::to_string(_inputs_number_wires.size());
            for (auto & party_wires : _inputs_number_wires) {
                inputs_section += " " + std::to_string(party_wires);
            } inputs_section += "\n";
            _circuit_file.write(inputs_section.c_str(), inputs_section.size());
            break;

        case CircuitType::LIBSCAPI:
            int assigned = 0;
            for (int i = 0; i < _inputs_number_wires.size(); i++) {
                inputs_section = std::to_string(i+1) + " " + std::to_string(_inputs_number_wires.at(i)) + "\n";
                _circuit_file.write(inputs_section.c_str(), inputs_section.size());
                for (int j = 0; j < _inputs_number_wires.at(i); j++) {
                    inputs_section = std::to_string(assigned++) + "\n";
                    _circuit_file.write(inputs_section.c_str(), inputs_section.size());
                }
                inputs_section = "\n";
                _circuit_file.write(inputs_section.c_str(), inputs_section.size());
            }
            break;
    }

    std::string outputs_section;
    switch (_circuit_type) {
        case CircuitType::BRISTOL:
            outputs_section = std::to_string(_outputs_number_wires.size());
            for (auto & party_wires : _outputs_number_wires) {
                outputs_section += " " + std::to_string(party_wires);
            } outputs_section += "\n\n";
            _circuit_file.write(outputs_section.c_str(), outputs_section.size());
            break;

        case CircuitType::BRISTOL_FASHION:
            outputs_section = std::to_string(_outputs_number_wires.size());
            for (auto & party_wires : _outputs_number_wires) {
                outputs_section += " " + std::to_string(party_wires);
            } outputs_section += "\n\n";
            _circuit_file.write(outputs_section.c_str(), outputs_section.size());
            break;

        case CircuitType::LIBSCAPI:
            int assigned = 0;
            for (int i = 0; i < _outputs_number_wires.size(); i++) {
                outputs_section = std::to_string(i+1) + " " + std::to_string(_outputs_number_wires.at(i)) + "\n";
                _circuit_file.write(outputs_section.c_str(), outputs_section.size());
                assigned += _outputs_number_wires.at(i);
                for (int j = 0; j < _outputs_number_wires.at(i); j++) {
                    outputs_section = std::to_string(_number_wires - assigned + j) + "\n";
                    _circuit_file.write(outputs_section.c_str(), outputs_section.size());
                }
                outputs_section = "\n";
                _circuit_file.write(outputs_section.c_str(), outputs_section.size());
            }
            break;
    }

    _temp_circuit_file.seekg(0);

    std::string line;
    while (std::getline(_temp_circuit_file, line)) {
        line += "\n";
        _circuit_file.write(line.c_str(), line.size());
    }
}


///////

gabe::circuits::generator::CircuitGenerator::CircuitGenerator() {}

gabe::circuits::generator::CircuitGenerator::CircuitGenerator(const std::string& circuit_file, const std::vector<uint64_t>& number_wires_input_parties, const std::vector<uint64_t>& number_wires_output_parties) {
    // Saves the temp file name
    _temp_file_name = "temp_" + circuit_file + ".txt";

    // Opens the files
    _circuit_file = std::ofstream( circuit_file + ".txt", std::ios::out );
    _temp_file = std::fstream( _temp_file_name, std::ios::in | std::ios::out | std::ios::trunc );

    // Registers the input wires
}

gabe::circuits::generator::CircuitGenerator::~CircuitGenerator() {
    // Closes the files
    if (_circuit_file.is_open()) { _circuit_file.close(); }
    if (_temp_file.is_open()) { _temp_file.close(); }

    // Removes the temporary file from the system
    remove( _temp_file_name.c_str() );
}

void gabe::circuits::generator::CircuitGenerator::_xor_gate(const Wire& in1, const Wire& in2, Wire& out) {
    // Increments the counters
    _counter_gates++;
    _counter_xor_gates++;

    _write_2_1_gate( in1.label, in2.label, out.label = ++_counter_wires, "XOR" );
}

void gabe::circuits::generator::CircuitGenerator::_and_gate(const Wire& in1, const Wire& in2, Wire& out) {
    // Increments the counters
    _counter_gates++;
    _counter_and_gates++;

    _write_2_1_gate( in1.label, in2.label, out.label = ++_counter_wires, "AND" );
}

void gabe::circuits::generator::CircuitGenerator::_inv_gate(const Wire& in, Wire& out) {
    // Increments the counters
    _counter_gates++;
    _counter_inv_gates++;

    _write_1_1_gate( in.label, out.label = ++_counter_wires, "INV" );
}

void gabe::circuits::generator::CircuitGenerator::_or_gate(const Wire& in1, const Wire& in2, Wire& out) {
    // Increments the counters
    _counter_gates++;
    _counter_or_gates++;

    _write_2_1_gate( in1.label, in2.label, out.label = ++_counter_wires, "OR" );
}

void gabe::circuits::generator::CircuitGenerator::_write_1_1_gate(const uint64_t in, const uint64_t out, const std::string& gate) {
    std::string line = "1 1 " + std::to_string(in) + " " + std::to_string(out) + " " + gate + "\n";

    _temp_file.write( line.c_str(), line.size() );
}

void gabe::circuits::generator::CircuitGenerator::_write_2_1_gate(const uint64_t in1, const uint64_t in2, const uint64_t out, const std::string& gate) {
    std::string input1 = in1 > in2 ? std::to_string(in2) : std::to_string(in1);

    std::string input2 = in1 > in2 ? std::to_string(in1) : std::to_string(in2);

    std::string line = "2 1 " + input1 + " " + input2 + " " + std::to_string(out) + " " + gate + "\n";

    _temp_file.write( line.c_str(), line.size() );
}