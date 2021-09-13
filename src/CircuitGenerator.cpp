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

    // Informs the user if the output wires are as expected
    if (_expected_output_wires != _counter_output_wires) {
        printf(">>> Warning: The number of expected output wires do not match with the amount given. Expected: %ld; Given: %ld", _expected_output_wires, _counter_output_wires);
    }
}

void gabe::circuits::generator::CircuitGenerator::_assert_empty(const Variable& variable) {
    if (!variable.number_wires) { throw std::invalid_argument("The inserted variable does not have any wires."); }
}

void gabe::circuits::generator::CircuitGenerator::_assert_size(const Variable& variable, uint64_t size) {}

void gabe::circuits::generator::CircuitGenerator::_assert_equal_size(const Variable& var1, const Variable& var2) {
    if (var1.number_wires != var2.number_wires) {
        throw std::invalid_argument("The inserted variables do not share the same size.");
    }
}

void gabe::circuits::generator::CircuitGenerator::_make_same_size(Variable& var1, Variable& var2) {
    if (var1.number_wires >= var2.number_wires) {
        for (int i = var2.number_wires; i < var1.number_wires; i++) {
            var2.number_wires++;
            var2.wires.push_back(_zero_wire);
        }
    }
    else {
        for (int i = var1.number_wires; i < var2.number_wires; i++) {
            var1.number_wires++;
            var1.wires.push_back(_zero_wire);
        }
    }
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

void gabe::circuits::generator::CircuitGenerator::add_output(Variable& output) {
    _counter_output_wires += output.number_wires;
}

gabe::circuits::Variable gabe::circuits::generator::CircuitGenerator::create_constant(uint8_t n_bits, uint64_t value) {
    // Creates a Variable with the wanted size
    gabe::circuits::Variable constant(n_bits);

    // Assigns the correct wires to the constant
    for (uint8_t i = 0; i < n_bits; i++) {
        // Gets the correct value
        uint8_t cur_val = (value >> i) & 0x01;

        // Assigns zero or one wire label to the current wire
        constant.wires[i].label = cur_val == 0 ? _zero_wire.label : _one_wire.label;

        // Registers the value
        // constant.wires[i].value = value;
    }

    return constant;
}

//void gabe::circuits::generator::CircuitGenerator::XOR(const Variable& input1, const Variable& input2, Variable& output) {
void gabe::circuits::generator::CircuitGenerator::XOR(Variable input1, Variable input2, Variable& output) {
    // Safety checks
    _assert_empty(input1);
    _assert_empty(input2);
    _assert_equal_size(input1, input2);

    // Gives the correct size of the output
    output = Variable(input1.number_wires);

    // XORs all the wires
    for (int i = 0; i < input1.number_wires; i++) {
        _xor_gate( input1.wires[i], input2.wires[i], output.wires[i] );
    }
}

//void gabe::circuits::generator::CircuitGenerator::AND(const Variable& input1, const Variable& input2, Variable& output) {
void gabe::circuits::generator::CircuitGenerator::AND(Variable input1, Variable input2, Variable& output) {
    // Safety checks
    _assert_empty(input1);
    _assert_empty(input2);
    _assert_equal_size(input1, input2);

    // Gives the correct size of the output
    output = Variable(input1.number_wires);

    // ANDs all the wires
    for (int i = 0; i < input1.number_wires; i++) {
        _and_gate( input1.wires[i], input2.wires[i], output.wires[i] );
    }
}

//void gabe::circuits::generator::CircuitGenerator::INV(const Variable& input, Variable& output) {
void gabe::circuits::generator::CircuitGenerator::INV(Variable input, Variable& output) {
    // Safety checks
    _assert_empty(input);

    // Gives the correct size of the output
    output = Variable(input.number_wires);

    // Negates all the wires
    for (int i = 0; i < input.number_wires; i++) {
        _inv_gate( input.wires[i], output.wires[i] );
    }
}

//void gabe::circuits::generator::CircuitGenerator::OR(const Variable& input1, const Variable& input2, Variable& output) {
void gabe::circuits::generator::CircuitGenerator::OR(Variable input1, Variable input2, Variable& output) {
    // Safety checks
    _assert_empty(input1);
    _assert_empty(input2);
    _assert_equal_size(input1, input2);

    // Gives the correct size of the output
    output = Variable(input1.number_wires);

    // ORs all the wires
    for (int i = 0; i < input1.number_wires; i++) {
        _or_gate( input1.wires[i], input2.wires[i], output.wires[i] );
    }
}

//void gabe::circuits::generator::CircuitGenerator::addition(const Variable& input1, const Variable& input2, Variable& output) {
void gabe::circuits::generator::CircuitGenerator::addition(Variable input1, Variable input2, Variable& output) {
    // Safety check
    _assert_empty(input1);
    _assert_empty(input2);

    _make_same_size(input1, input2);

    // Gives the correct size of the output
    output = Variable(input1.number_wires);

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

void gabe::circuits::generator::CircuitGenerator::division(const Variable& input1, const Variable& input2, Variable& output_quotient, Variable& output_remainder) {
    //
    _assert_equal_size(input1, input2);

    output_remainder = create_constant(output_remainder.number_wires, 0x00);

    //
    Variable zero = create_constant( input1.number_wires, 0x00 );

    Variable control(1);

    Variable subtractor(input1.number_wires);

    for (int i = 0; i < input1.number_wires; i++) {
        // Shifting
        for (int j = output_remainder.number_wires-1; j > 0; j--) {
            output_remainder.wires[j] = output_remainder.wires[j-1];
            output_remainder.wires[j-1] = _zero_wire;
        }

        output_remainder.wires[0] = input1.wires[input1.number_wires - 1 - i];

        //
        greater_or_equal(output_remainder, input2, control);

        //
        multiplexer(control, zero, input2, subtractor);

        //
        subtraction(output_remainder, subtractor, output_remainder);

        //
        output_quotient.wires[input1.number_wires - 1 - i] = control.wires[0];
    }
}

void gabe::circuits::generator::CircuitGenerator::division_quotient(const Variable& input1, const Variable& input2, Variable& output) {
    //
    _assert_equal_size(input1, input2);

    Variable output_remainder = create_constant(input1.number_wires, 0x00);

    //
    Variable zero = create_constant( input1.number_wires, 0x00 );

    Variable control(1);

    Variable subtractor(input1.number_wires);

    for (int i = 0; i < input1.number_wires; i++) {
        // Shifting
        for (int j = output_remainder.number_wires-1; j > 0; j--) {
            output_remainder.wires[j] = output_remainder.wires[j-1];
            output_remainder.wires[j-1] = _zero_wire;
        }

        output_remainder.wires[0] = input1.wires[input1.number_wires - 1 - i];

        //
        greater_or_equal(output_remainder, input2, control);

        if (i != input1.number_wires - 1) {
            //
            multiplexer(control, zero, input2, subtractor);

            //
            subtraction(output_remainder, subtractor, output_remainder);
        }

        //
        output.wires[input1.number_wires - 1 - i] = control.wires[0];
    }
}

void gabe::circuits::generator::CircuitGenerator::division_remainder(const Variable& input1, const Variable& input2, Variable& output) {
    Variable quotient(output.number_wires);

    division(input1, input2, quotient, output);
}

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

void gabe::circuits::generator::CircuitGenerator::equal(const Variable& input1, const Variable& input2, Variable& output) {
    // Safety checks
    _assert_equal_size(input1, input2);

    Variable xors(input1.number_wires);

    XOR(input1, input2, xors);

    for (int i = 1; i < xors.number_wires; i++) {
        if (i == 1) {
            _or_gate( xors.wires[i-1], xors.wires[i], output.wires[0] );
        } else {
            _or_gate( xors.wires[i], output.wires[0], output.wires[0] );
        }
        printf("%d ",i);
    } printf("\n");

    INV(output, output);
}

void gabe::circuits::generator::CircuitGenerator::greater(const Variable& input1, const Variable& input2, Variable& output) {
    // Safety checks
    _assert_equal_size( input1, input2 );

    // Performs initial steps
    Variable not_input2(input2.number_wires);
    INV(input2, not_input2);

    Variable xnors(input1.number_wires);
    for (int i = 1; i < input1.number_wires; i++) {
        _xor_gate( input1.wires[i], input2.wires[i], xnors.wires[i] );
        _inv_gate( xnors.wires[i], xnors.wires[i] );
    }

    // Performs middle steps
    Variable middle(input1.number_wires);
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

void gabe::circuits::generator::CircuitGenerator::smaller(const Variable& input1, const Variable& input2, Variable& output) {
    // Safety checks
    _assert_equal_size( input1, input2 );

    // Performs initial steps
    Variable not_input1(input1.number_wires);
    INV(input1, not_input1);

    Variable xnors(input1.number_wires);
    for (int i = 1; i < input1.number_wires; i++) {
        _xor_gate( input1.wires[i], input2.wires[i], xnors.wires[i] );
        _inv_gate( xnors.wires[i], xnors.wires[i] );
    }

    // Performs middle steps
    Variable middle(input1.number_wires);
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

void gabe::circuits::generator::CircuitGenerator::greater_or_equal(const Variable& input1, const Variable& input2, Variable& output) {
    smaller(input1, input2, output);
    INV(output, output);
}

void gabe::circuits::generator::CircuitGenerator::smaller_or_equal(const Variable& input1, const Variable& input2, Variable& output) {
    greater(input1, input2, output);
    INV(output, output);
}

void gabe::circuits::generator::CircuitGenerator::comparator(const Variable& input1, const Variable& input2, Variable& out_equal, Variable& out_greater, Variable &out_smaller) {
    // Safety checks
    _assert_equal_size( input1, input2 );

    // Performs initial steps
    Variable not_input1(input1.number_wires);
    INV(input1, not_input1);

    Variable not_input2(input2.number_wires);
    INV(input2, not_input2);

    Variable xnors(input1.number_wires);
    for (int i = 1; i < input1.number_wires; i++) {
        _xor_gate( input1.wires[i], input2.wires[i], xnors.wires[i] );
        _inv_gate( xnors.wires[i], xnors.wires[i] );
    }

    // Performs middle steps
    Variable middle_greater(input1.number_wires);
    Variable middle_smaller(input1.number_wires);
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
