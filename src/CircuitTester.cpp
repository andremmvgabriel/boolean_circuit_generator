#include <CircuitTester.hpp>

gabe::circuits::test::TesterAbs::TesterAbs() {}

gabe::circuits::test::TesterAbs::TesterAbs(const std::string& circuit) : _circuit_file(circuit, std::ios::in) {}

gabe::circuits::test::TesterAbs::~TesterAbs() {
    _circuit_file.close();
}

void gabe::circuits::test::TesterAbs::_read_header() {}

void gabe::circuits::test::TesterAbs::_read_inputs(const std::string& inputs) {
    // Opens the input file
    std::ifstream inputs_file( inputs, std::ios::in );

    // Makes sure the file is opened
    if (inputs_file.fail()) {
        throw std::runtime_error("The inputs file could not be opened. Make sure you are giving a valid file.");
    }

    // Reads and registers the values of the input wires
    std::string line;
    uint64_t read_wires_counter = 0;
    for (auto & amount : _number_wires_input_parties) {
        for (uint64_t i = 0; i < amount; i++) {
            // Reads the value
            std::getline(inputs_file, line);

            // Assigns the value
            _wire_values[read_wires_counter + i] = (uint8_t)(std::stoi(line));

            printf("> Wire %ld: %d\n", read_wires_counter + i, _wire_values[read_wires_counter + i]);
        }
        printf("\n");

        // Increases the read wires counter
        read_wires_counter += amount;
    }

    // Closes the file
    inputs_file.close();
}

void gabe::circuits::test::TesterAbs::_read_inputs(const std::vector<uint8_t>& inputs) {
    // Reads and registers the values of the input wires
    uint64_t read_wires_counter = 0;
    for (auto & amount : _number_wires_input_parties) {
        for (uint64_t i = 0; i < amount; i++) {
            // Assigns the value
            _wire_values[read_wires_counter + i] = inputs.at(read_wires_counter + i);

            printf("> Wire %ld: %d\n", read_wires_counter + i, _wire_values[read_wires_counter + i]);
        }
        printf("\n");

        // Increases the read wires counter
        read_wires_counter += amount;
    }
}

void gabe::circuits::test::TesterAbs::_execute_circuit() {
    //
    std::string line;
    while (std::getline(_circuit_file, line)) {
        std::stringstream gate_info(line);

        int n_input_wires = 0;
        int n_output_wires = 0;

        gate_info >> n_input_wires >> n_output_wires;

        std::vector<uint64_t> input_wires(n_input_wires);
        std::vector<uint64_t> output_wires(n_output_wires);

        for (int i = 0; i < n_input_wires; i++) {
            gate_info >> input_wires.at(i);
        }

        for (int i = 0; i < n_output_wires; i++) {
            gate_info >> output_wires.at(i);
        }

        std::string gate;
        gate_info >> gate;

        if (gate == _gates_map["and"]) {
            _wire_values[output_wires.at(0)] = _wire_values.at(input_wires.at(0)) & _wire_values.at(input_wires.at(1));
        }
        else if (gate == _gates_map["or"]) {
            _wire_values[output_wires.at(0)] = _wire_values.at(input_wires.at(0)) | _wire_values.at(input_wires.at(1));
        }
        else if (gate == _gates_map["xor"]) {
            _wire_values[output_wires.at(0)] = _wire_values.at(input_wires.at(0)) ^ _wire_values.at(input_wires.at(1));
        }
        else if (gate == _gates_map["inv"]) {
            _wire_values[output_wires.at(0)] = !_wire_values.at(input_wires.at(0));
        }
        else {
            throw std::runtime_error("The gate is not recognized.");
        }
    }

    _print_results();
}

void gabe::circuits::test::TesterAbs::_print_results() {
    uint64_t assigned = 0x00;
    for (auto & amount : _number_wires_output_parties) { assigned += amount; }

    for (uint64_t i = 0; i < _number_wires_output_parties.size(); i++) {
        printf("> Output party %ld:\n", i);
        printf("   - Wires: ");
        for (uint64_t j = _number_wires_output_parties.at(i); j > 0; j--) {
            printf("%ld ", _number_wires - assigned + _number_wires_output_parties.at(i) - j);
        }
        printf("\n");

        printf("   - Values bin: ");
        for (uint64_t j = _number_wires_output_parties.at(i); j > 0; j--) {
            printf("%ld", _wire_values[_number_wires - assigned + _number_wires_output_parties.at(i) - j] );
        }
        printf("\n");

        printf("   - Values byte: ");
        uint8_t val = 0x00;
        int counter = 0;
        for (uint64_t j = _number_wires_output_parties.at(i); j > 0; j--) {
            val |= ( _wire_values[_number_wires - assigned + _number_wires_output_parties.at(i) - j] << counter );
            counter++;
            if (counter == 8 || j == 1) {
                printf("%ld ", val);
                val = 0x00;
                counter = 0;
            }
        }
        printf("\n");

        printf("   - Values hex: ");
        for (uint64_t j = _number_wires_output_parties.at(i); j > 0; j--) {
            val |= ( _wire_values[_number_wires - assigned + _number_wires_output_parties.at(i) - j] << counter );
            counter++;
            if (counter == 8 || j == 1) {
                printf("%04x ", val);
                val = 0x00;
                counter = 0;
            }
        }
        printf("\n");

        assigned -= _number_wires_output_parties.at(i);
    }
}

void gabe::circuits::test::TesterAbs::run(const std::string& inputs) {
    // Reads the inputs
    _read_inputs(inputs);

    // Executes the circuit
    _execute_circuit();

    // Returns the pointer to the start of the circuit
    _circuit_file.seekg( _circuit_start );
}

void gabe::circuits::test::TesterAbs::run(const std::vector<uint8_t>& inputs) {
    // Reads the inputs
    _read_inputs(inputs);

    // Executes the circuit
    _execute_circuit();

    // Returns the pointer to the start of the circuit
    _circuit_file.seekg( _circuit_start );
}
