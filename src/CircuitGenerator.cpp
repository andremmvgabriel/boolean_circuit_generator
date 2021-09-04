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

gabe::circuits::generator::CircuitGenerator::CircuitGenerator(const std::string& circuit_file, const std::vector<uint64_t>& number_wires_input_parties, const std::vector<uint64_t>& number_wires_output_parties) : _number_wires_input_parties(number_wires_input_parties), _number_wires_output_parties(number_wires_output_parties) {
    // Saves the temp file name
    _temp_file_name = "temp_" + circuit_file;

    // Opens the files
    _circuit_file = std::ofstream( circuit_file, std::ios::out );
    _temp_file = std::fstream( _temp_file_name, std::ios::in | std::ios::out | std::ios::trunc );

    // Registers the expected wires for inputs and outputs
    for (auto & amount : number_wires_input_parties) { _expected_input_wires += amount; }
    for (auto & amount : number_wires_output_parties) { _expected_output_wires += amount; }
}

gabe::circuits::generator::CircuitGenerator::~CircuitGenerator() {
    // Closes the files
    if (_circuit_file.is_open()) { _circuit_file.close(); }
    if (_temp_file.is_open()) { _temp_file.close(); }

    // Removes the temporary file from the system
    remove( _temp_file_name.c_str() );
}

void gabe::circuits::generator::CircuitGenerator::_assert_equal_size(const Variable& var1, const Variable& var2) {
    if (var1.number_wires != var2.number_wires) {
        throw std::invalid_argument("The inserted variables do not share the same size.");
    }
}

void gabe::circuits::generator::CircuitGenerator::_write_header() {
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

void gabe::circuits::generator::CircuitGenerator::_write_circuit() {
    // Places the reading pointer in the beginning of the file
    _temp_file.seekg(0);

    std::string line;
    while (std::getline(_temp_file, line)) {
        line += "\n";
        _circuit_file.write(line.c_str(), line.size());
    }
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

void gabe::circuits::generator::CircuitGenerator::_xor_gate(const Wire& in1, const Wire& in2, Wire& out) {
    // Increments the counters
    _counter_gates++;
    _counter_xor_gates++;

    _write_2_1_gate( in1.label, in2.label, out.label = _counter_wires++, "XOR" );
}

void gabe::circuits::generator::CircuitGenerator::_and_gate(const Wire& in1, const Wire& in2, Wire& out) {
    // Increments the counters
    _counter_gates++;
    _counter_and_gates++;

    _write_2_1_gate( in1.label, in2.label, out.label = _counter_wires++, "AND" );
}

void gabe::circuits::generator::CircuitGenerator::_inv_gate(const Wire& in, Wire& out) {
    // Increments the counters
    _counter_gates++;
    _counter_inv_gates++;

    _write_1_1_gate( in.label, out.label = _counter_wires++, "INV" );
}

void gabe::circuits::generator::CircuitGenerator::_or_gate(const Wire& in1, const Wire& in2, Wire& out) {
    // Increments the counters
    _counter_gates++;
    _counter_or_gates++;

    _write_2_1_gate( in1.label, in2.label, out.label = _counter_wires++, "OR" );
}

void gabe::circuits::generator::CircuitGenerator::start() {
    // Checks if all the inputs wires were assigned
    if (_counter_wires < _expected_input_wires) {
        printf("\n>>> WARNING: There are less input wires assigned than expected. [%ld, %ld] wires are dead in the circuit.\n\n", _counter_wires, _expected_input_wires - 1);
        _counter_wires = _expected_input_wires;
    }
    
    // Creates the zero and one wires
    _xor_gate( Wire(), Wire(), _zero_wire );
    _inv_gate( _zero_wire, _one_wire );

    printf("Zero wire: %ld\n", _zero_wire.label);
    printf("One wire: %ld\n", _one_wire.label);
}

void gabe::circuits::generator::CircuitGenerator::conclude() {
    printf("> Final circuit details:\n");
    printf("   - TOTAL gates: %ld\n", _counter_gates);
    printf("   - TOTAL wires: %ld\n", _counter_wires);
    printf("   - XOR gates: %ld\n", _counter_xor_gates);
    printf("   - AND gates: %ld\n", _counter_and_gates);
    printf("   - INV gates: %ld\n", _counter_inv_gates);
    printf("   - OR gates: %ld\n", _counter_or_gates);

    _write_header();
    _write_circuit();
}

void gabe::circuits::generator::CircuitGenerator::add_input(Variable& input) {
    // Goes through all the input wires
    for (auto & wire : input.wires) {
        // Safety check
        if (_counter_wires >= _expected_input_wires) {
            throw std::runtime_error("There aren't enough input wires available. Make sure the specified input wires for each party are correct.");
        }

        // Assigns a label to the current wire
        wire.label = _counter_wires++;
    }
}

void gabe::circuits::generator::CircuitGenerator::add_output(Variable& output) {}

gabe::circuits::Variable gabe::circuits::generator::CircuitGenerator::create_constant(uint8_t n_bits, uint64_t value) {
    // Creates a Variable with the wanted size
    gabe::circuits::Variable constant(n_bits);

    // Assigns the correct wires to the constant
    for (int i = 0; i < n_bits; i++) {
        // Gets the correct value
        uint8_t cur_val = (value >> i) & 0x01;

        // Assigns zero or one wire label to the current wire
        constant.wires[i].label = cur_val == 0 ? _zero_wire.label : _one_wire.label;

        // Registers the value
        // constant.wires[i].value = value;
    }

    return constant;
}

void gabe::circuits::generator::CircuitGenerator::XOR(const Variable& input1, const Variable& input2, Variable& output) {
    // Safety checks
    _assert_equal_size(input1, input2);
    _assert_equal_size(input1, output);

    // XORs all the wires
    for (int i = 0; i < input1.number_wires; i++) {
        _xor_gate( input1.wires[i], input2.wires[i], output.wires[i] );
    }
}

void gabe::circuits::generator::CircuitGenerator::AND(const Variable& input1, const Variable& input2, Variable& output) {
    // Safety checks
    _assert_equal_size(input1, input2);
    _assert_equal_size(input1, output);

    // XORs all the wires
    for (int i = 0; i < input1.number_wires; i++) {
        _and_gate( input1.wires[i], input2.wires[i], output.wires[i] );
    }
}

void gabe::circuits::generator::CircuitGenerator::INV(const Variable& input, Variable& output) {
    // Safety checks
    _assert_equal_size(input, output);

    // XORs all the wires
    for (int i = 0; i < input.number_wires; i++) {
        _inv_gate( input.wires[i], output.wires[i] );
    }
}

void gabe::circuits::generator::CircuitGenerator::OR(const Variable& input1, const Variable& input2, Variable& output) {
    // Safety checks
    _assert_equal_size(input1, input2);
    _assert_equal_size(input1, output);

    // XORs all the wires
    for (int i = 0; i < input1.number_wires; i++) {
        _or_gate( input1.wires[i], input2.wires[i], output.wires[i] );
    }
}

void gabe::circuits::generator::CircuitGenerator::addition(const Variable& input1, const Variable& input2, Variable& output) {
    // Safety check
    _assert_equal_size(input1, input2);
    _assert_equal_size(input1, output);

    Variable carry = create_constant(output.number_wires, 0);

    Variable a_xor_b(input1.number_wires);
    Variable a_and_b(input1.number_wires);
    Variable a_xor_b_and_c(input1.number_wires);

    for (int i = 0; i < output.number_wires; i++) {
        _xor_gate( input1.wires[i], input2.wires[i], a_xor_b.wires[i] );

        if (i != output.number_wires - 1) {
            _and_gate( input1.wires[i], input2.wires[i], a_and_b.wires[i] );
            _and_gate( a_xor_b.wires[i], carry.wires[i], a_xor_b_and_c.wires[i] );
            _or_gate( a_and_b.wires[i], a_xor_b_and_c.wires[i], carry.wires[i + 1] );
        }
    }

    for (int i = 0; i < output.number_wires; i++) {
        _xor_gate( a_xor_b.wires[i], carry.wires[i], output.wires[i] );
    }
}

void gabe::circuits::generator::CircuitGenerator::subtraction(const Variable& input1, const Variable& input2, Variable& output) {
    // Safety check
    _assert_equal_size(input1, input2);
    _assert_equal_size(input1, output);

    Variable carry = create_constant(output.number_wires, 0);

    Variable a_xor_b(output.number_wires);
    Variable inv_xor(output.number_wires);
    Variable inv_a(output.number_wires);
    Variable and_xor(output.number_wires);
    Variable and_a_c(output.number_wires);

    for (int i = 0; i < output.number_wires; i++) {
        _xor_gate( input1.wires[i], input2.wires[i], a_xor_b.wires[i] );
        if (i != output.number_wires-1) {
            _inv_gate( a_xor_b.wires[i], inv_xor.wires[i] );
            _inv_gate( input1.wires[i], inv_a.wires[i] );
            _and_gate( inv_xor.wires[i], carry.wires[i], and_xor.wires[i] );
            _and_gate( inv_a.wires[i], input2.wires[i], and_a_c.wires[i] );
            _or_gate( and_xor.wires[i], and_a_c.wires[i], carry.wires[i+1] );
        }
    }

    for (int i = 0; i < output.number_wires; i++) {
        _xor_gate( a_xor_b.wires[i], carry.wires[i], output.wires[i] );
    }
}

void gabe::circuits::generator::CircuitGenerator::multiplication(const Variable& input1, const Variable& input2, Variable& output) {
    // Safety check
    // MISSING

    std::vector<Variable> variables(input2.number_wires);

    // Creation of the multiple variables
    for (int i = 0; i < input2.number_wires; i++) {
        variables.at(i) = create_constant(output.number_wires, 0x00);

        for (int j = 0; j < input1.number_wires; j++) {
            _and_gate( input1.wires[j], input2.wires[i], variables.at(i).wires[i+j] );
        }
    }

    for (int i = 1; i < variables.size() ; i++) {
        if (i == 1) {
            addition( variables.at(i-1), variables.at(i), output );
        }
        else {
            addition( output, variables.at(i), output );
        }
    }
}

void gabe::circuits::generator::CircuitGenerator::division(const Variable& input1, const Variable& input2, Variable& output) {}

void gabe::circuits::generator::CircuitGenerator::multiplexer(const Variable& control, const Variable& input1, const Variable& input2, Variable& output) {
    // Creates the not control
    Variable not_control(control.number_wires);
    INV(control, not_control);

    Variable and_in1(input1.number_wires);
    Variable and_in2(input2.number_wires);

    for (int i = 0; i < input1.number_wires; i++) {
        _and_gate( not_control.wires[0], input1.wires[i], and_in1.wires[i] );
        _and_gate( control.wires[0], input2.wires[i], and_in2.wires[i] );
    }

    for (int i = 0; i < output.number_wires; i++) {
        _or_gate( and_in1.wires[i], and_in2.wires[i], output.wires[i] );
    }
}
