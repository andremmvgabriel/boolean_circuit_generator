#include <CircuitGenerator.hpp>

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

void gabe::circuits::generator::CircuitGenerator::_assert_equal_size(const UnsignedVariable& var1, const UnsignedVariable& var2) {
    if (var1.number_wires != var2.number_wires) {
        throw std::invalid_argument("The inserted variables do not share the same size.");
    }
}

void gabe::circuits::generator::CircuitGenerator::_assert_equal_size(const SignedVariable& var1, const SignedVariable& var2) {
    if (var1.number_wires != var2.number_wires) {
        throw std::invalid_argument("The inserted variables do not share the same size.");
    }
}

gabe::circuits::UnsignedVariable gabe::circuits::generator::CircuitGenerator::_stou(const SignedVariable& input) {
    UnsignedVariable output(input.number_wires);
    
    for (int i = 0 ; i < input.number_wires; i++) {
        output.wires[i] = input.wires[i];
    }

    return output;
}

gabe::circuits::SignedVariable gabe::circuits::generator::CircuitGenerator::_utos(const UnsignedVariable& input) {
    SignedVariable output(input.number_wires);

    for (int i = 0 ; i < input.number_wires; i++) {
        output.wires[i] = input.wires[i];
    }

    return output;
}

void gabe::circuits::generator::CircuitGenerator::_write_header() {}

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

    _write_2_1_gate( in1.label, in2.label, _counter_wires, _gates_map["xor"] );

    out.label = _counter_wires++;
}

void gabe::circuits::generator::CircuitGenerator::_and_gate(const Wire& in1, const Wire& in2, Wire& out) {
    // Increments the counters
    _counter_gates++;
    _counter_and_gates++;

    _write_2_1_gate( in1.label, in2.label, _counter_wires, _gates_map["and"] );

    out.label = _counter_wires++;
}

void gabe::circuits::generator::CircuitGenerator::_inv_gate(const Wire& in, Wire& out) {
    // Increments the counters
    _counter_gates++;
    _counter_inv_gates++;

    _write_1_1_gate( in.label, _counter_wires, _gates_map["inv"] );

    out.label = _counter_wires++;
}

void gabe::circuits::generator::CircuitGenerator::_or_gate(const Wire& in1, const Wire& in2, Wire& out) {
    // Increments the counters
    _counter_gates++;
    _counter_or_gates++;

    _write_2_1_gate( in1.label, in2.label, _counter_wires, _gates_map["or"] );

    out.label = _counter_wires++;
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

void gabe::circuits::generator::CircuitGenerator::add_input(UnsignedVariable& input) {
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

void gabe::circuits::generator::CircuitGenerator::add_input(SignedVariable& input) {
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

void gabe::circuits::generator::CircuitGenerator::add_output(UnsignedVariable& output) {}

void gabe::circuits::generator::CircuitGenerator::add_output(SignedVariable& output) {}

gabe::circuits::UnsignedVariable gabe::circuits::generator::CircuitGenerator::create_constant(uint8_t n_bits, uint64_t value) {
    // Creates a UnsignedVariable with the wanted size
    gabe::circuits::UnsignedVariable constant(n_bits);

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

/*
gabe::circuits::SignedVariable gabe::circuits::generator::CircuitGenerator::create_constant(uint8_t n_bits, int64_t value) {
    // Creates a UnsignedVariable with the wanted size
    gabe::circuits::SignedVariable constant(n_bits);

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
} */

void gabe::circuits::generator::CircuitGenerator::XOR(const UnsignedVariable& input1, const UnsignedVariable& input2, UnsignedVariable& output) {
    // Safety checks
    _assert_equal_size(input1, input2);
    _assert_equal_size(input1, output);

    // XORs all the wires
    for (int i = 0; i < input1.number_wires; i++) {
        _xor_gate( input1.wires[i], input2.wires[i], output.wires[i] );
    }
}

void gabe::circuits::generator::CircuitGenerator::XOR(const SignedVariable& input1, const SignedVariable& input2, SignedVariable& output) {
    // Safety checks
    _assert_equal_size(input1, input2);
    _assert_equal_size(input1, output);

    // XORs all the wires
    for (int i = 0; i < input1.number_wires; i++) {
        _xor_gate( input1.wires[i], input2.wires[i], output.wires[i] );
    }
}

void gabe::circuits::generator::CircuitGenerator::AND(const UnsignedVariable& input1, const UnsignedVariable& input2, UnsignedVariable& output) {
    // Safety checks
    _assert_equal_size(input1, input2);
    _assert_equal_size(input1, output);

    // XORs all the wires
    for (int i = 0; i < input1.number_wires; i++) {
        _and_gate( input1.wires[i], input2.wires[i], output.wires[i] );
    }
}

void gabe::circuits::generator::CircuitGenerator::AND(const SignedVariable& input1, const SignedVariable& input2, SignedVariable& output) {
    // Safety checks
    _assert_equal_size(input1, input2);
    _assert_equal_size(input1, output);

    // XORs all the wires
    for (int i = 0; i < input1.number_wires; i++) {
        _and_gate( input1.wires[i], input2.wires[i], output.wires[i] );
    }
}

void gabe::circuits::generator::CircuitGenerator::INV(const UnsignedVariable& input, UnsignedVariable& output) {
    // Safety checks
    _assert_equal_size(input, output);

    // XORs all the wires
    for (int i = 0; i < input.number_wires; i++) {
        _inv_gate( input.wires[i], output.wires[i] );
    }
}

void gabe::circuits::generator::CircuitGenerator::INV(const SignedVariable& input, SignedVariable& output) {
    // Safety checks
    _assert_equal_size(input, output);

    // XORs all the wires
    for (int i = 0; i < input.number_wires; i++) {
        _inv_gate( input.wires[i], output.wires[i] );
    }
}

void gabe::circuits::generator::CircuitGenerator::OR(const UnsignedVariable& input1, const UnsignedVariable& input2, UnsignedVariable& output) {
    // Safety checks
    _assert_equal_size(input1, input2);
    _assert_equal_size(input1, output);

    // XORs all the wires
    for (int i = 0; i < input1.number_wires; i++) {
        _or_gate( input1.wires[i], input2.wires[i], output.wires[i] );
    }
}

void gabe::circuits::generator::CircuitGenerator::OR(const SignedVariable& input1, const SignedVariable& input2, SignedVariable& output) {
    // Safety checks
    _assert_equal_size(input1, input2);
    _assert_equal_size(input1, output);

    // XORs all the wires
    for (int i = 0; i < input1.number_wires; i++) {
        _or_gate( input1.wires[i], input2.wires[i], output.wires[i] );
    }
}

void gabe::circuits::generator::CircuitGenerator::addition(const UnsignedVariable& input1, const UnsignedVariable& input2, UnsignedVariable& output) {
    // Safety check
    _assert_equal_size(input1, input2);
    _assert_equal_size(input1, output);

    UnsignedVariable carry = create_constant(output.number_wires, 0);

    UnsignedVariable a_xor_b(input1.number_wires);
    UnsignedVariable a_and_b(input1.number_wires);
    UnsignedVariable a_xor_b_and_c(input1.number_wires);

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

void gabe::circuits::generator::CircuitGenerator::subtraction(const UnsignedVariable& input1, const UnsignedVariable& input2, UnsignedVariable& output) {
    // Safety check
    _assert_equal_size(input1, input2);
    _assert_equal_size(input1, output);

    UnsignedVariable carry = create_constant(output.number_wires, 0);

    UnsignedVariable a_xor_b(output.number_wires);
    UnsignedVariable inv_xor(output.number_wires);
    UnsignedVariable inv_a(output.number_wires);
    UnsignedVariable and_xor(output.number_wires);
    UnsignedVariable and_a_c(output.number_wires);

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

void gabe::circuits::generator::CircuitGenerator::multiplication(const UnsignedVariable& input1, const UnsignedVariable& input2, UnsignedVariable& output) {
    // Safety check
    // MISSING

    std::vector<UnsignedVariable> variables(input2.number_wires);

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

void gabe::circuits::generator::CircuitGenerator::multiplication(const SignedVariable& input1, const SignedVariable& input2, SignedVariable& output) {
    // Initial convertions
    UnsignedVariable input1_u = _stou(input1);
    UnsignedVariable input2_u = _stou(input2);
    UnsignedVariable output_u(input1.number_wires + input2.number_wires);

    // 2s complement Input 1
    UnsignedVariable input1_2s_comp(input1_u.number_wires);
    complement_2s(input1_u, input1_2s_comp);

    // 2s complement Input 2
    UnsignedVariable input2_2s_comp(input2_u.number_wires);
    complement_2s(input2_u, input2_2s_comp);

    // Signs - Control variables
    UnsignedVariable input1_sign(1); input1_sign.wires[0] = input1_u.wires.back();
    UnsignedVariable input2_sign(1); input2_sign.wires[0] = input2_u.wires.back();
    UnsignedVariable xored_signs(1); XOR(input1_sign, input2_sign, xored_signs);

    // Multiplexer outputs
    UnsignedVariable mux1_output(input1.number_wires);
    UnsignedVariable mux2_output(input2.number_wires);

    multiplexer(input1_sign, input1_u, input1_2s_comp, mux1_output);
    multiplexer(input2_sign, input2_u, input2_2s_comp, mux2_output);

    UnsignedVariable mult_output(input1.number_wires + input2.number_wires);

    multiplication(mux1_output, mux2_output, mult_output);

    UnsignedVariable mult_2s_comp(mult_output.number_wires);
    complement_2s(mult_output, mult_2s_comp);

    multiplexer(xored_signs, mult_output, mult_2s_comp, output_u);

    output = _utos(output_u);
}

void gabe::circuits::generator::CircuitGenerator::division(const UnsignedVariable& input1, const UnsignedVariable& input2, UnsignedVariable& output) {}

void gabe::circuits::generator::CircuitGenerator::multiplexer(const UnsignedVariable& control, const UnsignedVariable& input1, const UnsignedVariable& input2, UnsignedVariable& output) {
    // Creates the not control
    UnsignedVariable not_control(control.number_wires);
    INV(control, not_control);

    UnsignedVariable and_in1(input1.number_wires);
    UnsignedVariable and_in2(input2.number_wires);

    for (int i = 0; i < input1.number_wires; i++) {
        _and_gate( not_control.wires[0], input1.wires[i], and_in1.wires[i] );
        _and_gate( control.wires[0], input2.wires[i], and_in2.wires[i] );
    }

    for (int i = 0; i < output.number_wires; i++) {
        _or_gate( and_in1.wires[i], and_in2.wires[i], output.wires[i] );
    }
}

void gabe::circuits::generator::CircuitGenerator::equal(const UnsignedVariable& input1, const UnsignedVariable& input2, UnsignedVariable& output) {
    // Safety checks
    _assert_equal_size(input1, input2);

    UnsignedVariable xors(input1.number_wires);

    XOR(input1, input2, xors);

    for (int i = 1; i < xors.number_wires; i++) {
        if (i == 1) {
            _or_gate( xors.wires[i-1], xors.wires[i], output.wires[0] );
        } else {
            _or_gate( xors.wires[i], output.wires[0], output.wires[0] );
        }
    }

    INV(output, output);
}

void gabe::circuits::generator::CircuitGenerator::greater(const UnsignedVariable& input1, const UnsignedVariable& input2, UnsignedVariable& output) {
    // Safety checks
    _assert_equal_size( input1, input2 );

    // Performs initial steps
    UnsignedVariable not_input2(input2.number_wires);
    INV(input2, not_input2);

    UnsignedVariable xnors(input1.number_wires);
    for (int i = 1; i < input1.number_wires; i++) {
        _xor_gate( input1.wires[i], input2.wires[i], xnors.wires[i] );
        _inv_gate( xnors.wires[i], xnors.wires[i] );
    }

    // Performs middle steps
    UnsignedVariable middle(input1.number_wires);
    for (int i = 0; i < middle.number_wires; i++) {
        for (int j = i; j < middle.number_wires; j++) {
            if (i == j) {
                _and_gate( input1.wires[j], not_input2.wires[j], middle.wires[i] );
            } else {
                _and_gate( middle.wires[i], xnors.wires[j], middle.wires[i] );
            }
        }
    }

    // Performs final steps
    for (int i = 1; i < middle.number_wires; i++) {
        if (i == 1) {
            _or_gate( middle.wires[i - 1], middle.wires[i], output.wires[0] );
        } else {
            _or_gate( middle.wires[i], output.wires[0], output.wires[0] );
        }
    }
}

void gabe::circuits::generator::CircuitGenerator::smaller(const UnsignedVariable& input1, const UnsignedVariable& input2, UnsignedVariable& output) {
    // Safety checks
    _assert_equal_size( input1, input2 );

    // Performs initial steps
    UnsignedVariable not_input1(input1.number_wires);
    INV(input1, not_input1);

    UnsignedVariable xnors(input1.number_wires);
    for (int i = 1; i < input1.number_wires; i++) {
        _xor_gate( input1.wires[i], input2.wires[i], xnors.wires[i] );
        _inv_gate( xnors.wires[i], xnors.wires[i] );
    }

    // Performs middle steps
    UnsignedVariable middle(input1.number_wires);
    for (int i = 0; i < middle.number_wires; i++) {
        for (int j = i; j < middle.number_wires; j++) {
            if (i == j) {
                _and_gate( not_input1.wires[j], input2.wires[j], middle.wires[i] );
            } else {
                _and_gate( middle.wires[i], xnors.wires[j], middle.wires[i] );
            }
        }
    }

    // Performs final steps
    for (int i = 1; i < middle.number_wires; i++) {
        if (i == 1) {
            _or_gate( middle.wires[i - 1], middle.wires[i], output.wires[0] );
        } else {
            _or_gate( middle.wires[i], output.wires[0], output.wires[0] );
        }
    }
}

void gabe::circuits::generator::CircuitGenerator::greater_or_equal(const UnsignedVariable& input1, const UnsignedVariable& input2, UnsignedVariable& output) {
    smaller(input1, input2, output);
    INV(output, output);
}

void gabe::circuits::generator::CircuitGenerator::smaller_or_equal(const UnsignedVariable& input1, const UnsignedVariable& input2, UnsignedVariable& output) {
    greater(input1, input2, output);
    INV(output, output);
}

void gabe::circuits::generator::CircuitGenerator::comparator(const UnsignedVariable& input1, const UnsignedVariable& input2, UnsignedVariable& out_equal, UnsignedVariable& out_greater, UnsignedVariable &out_smaller) {
    // Safety checks
    _assert_equal_size( input1, input2 );

    // Performs initial steps
    UnsignedVariable not_input1(input1.number_wires);
    INV(input1, not_input1);

    UnsignedVariable not_input2(input2.number_wires);
    INV(input2, not_input2);

    UnsignedVariable xnors(input1.number_wires);
    for (int i = 1; i < input1.number_wires; i++) {
        _xor_gate( input1.wires[i], input2.wires[i], xnors.wires[i] );
        _inv_gate( xnors.wires[i], xnors.wires[i] );
    }

    // Performs middle steps
    UnsignedVariable middle_greater(input1.number_wires);
    UnsignedVariable middle_smaller(input1.number_wires);
    for (int i = 0; i < middle_greater.number_wires; i++) {
        for (int j = i; j < middle_greater.number_wires; j++) {
            if (i == j) {
                _and_gate(input1.wires[j], not_input2.wires[j], middle_greater.wires[i]);
                _and_gate(not_input1.wires[j], input2.wires[j], middle_smaller.wires[i]);
            } else {
                _and_gate(middle_greater.wires[i], xnors.wires[j], middle_greater.wires[i]);
                _and_gate(middle_smaller.wires[i], xnors.wires[j], middle_smaller.wires[i]);
            }
        }
    }

    // Performs final steps
    for (int i = 1; i < middle_greater.number_wires; i++) {
        if (i == 1) {
            _or_gate( middle_greater.wires[i - 1], middle_greater.wires[i], out_greater.wires[0] );
            _or_gate( middle_smaller.wires[i - 1], middle_smaller.wires[i], out_smaller.wires[0] );
        } else {
            _or_gate( middle_greater.wires[i], out_greater.wires[0], out_greater.wires[0] );
            _or_gate( middle_smaller.wires[i], out_smaller.wires[0], out_smaller.wires[0] );
        }
    }

    OR(out_greater, out_smaller, out_equal);
    INV(out_equal, out_equal);
}

void gabe::circuits::generator::CircuitGenerator::complement_2s(const UnsignedVariable& input, UnsignedVariable& output) {
    // Inverts all the wires
    INV(input, output);
    
    // Creates a constant with value 1
    UnsignedVariable constant_one = create_constant(output.number_wires, 0x01);

    // Adds the value to the output
    addition(output, constant_one, output);
}

void gabe::circuits::generator::CircuitGenerator::complement_2s(const SignedVariable& input, SignedVariable& output) {
    UnsignedVariable input_u = _stou(input);
    UnsignedVariable output_u(input.number_wires);

    complement_2s(input_u, output_u);

    output = _utos(output_u);
}
