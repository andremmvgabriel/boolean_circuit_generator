#include <BristolCircuitTester.hpp>

gabe::circuits::test::BristolTester::BristolTester() : TesterAbs() {}

gabe::circuits::test::BristolTester::BristolTester(const std::string& circuit) : TesterAbs(circuit) {
    read_header();
}

gabe::circuits::test::BristolTester::~BristolTester() {}

void gabe::circuits::test::BristolTester::read_header() {
    // String to read all the lines
    std::string line;

    // Reads first line
    std::getline(_circuit_file, line);
    std::stringstream first_line(line);
    first_line >> _number_gates >> _number_wires;

    // Reads second line
    std::getline(_circuit_file, line);
    std::stringstream second_line(line);
    second_line >> _number_input_parties;
    for (int i = 0; i < _number_input_parties; i++) {
        uint32_t amount = 0;
        second_line >> amount;
        _number_wires_input_parties.push_back(amount);
    }

    // Reads third line
    std::getline(_circuit_file, line);
    std::stringstream third_line(line);
    third_line >> _number_output_parties;
    for (int i = 0; i < _number_output_parties; i++) {
        uint32_t amount = 0;
        third_line >> amount;
        _number_wires_output_parties.push_back(amount);
    }

    // Finds the beginning of the circuit
    while(std::getline(_circuit_file, line)) {
        if (line.size()) {
            // Gets the current pointer position
            uint64_t current_pos = _circuit_file.tellg();

            // Returns the pointer back the whole string size
            _circuit_file.seekg( current_pos - line.size() - 1 );

            // Registers the beginning position of the circuit
            _circuit_start = _circuit_file.tellg();

            break;
        }
    }

    // Prints circuit information
    printf("> Circuit Info:\n");
    printf("   - Number gates: %ld\n", _number_gates);
    printf("   - Number wires: %ld\n", _number_wires);
    printf("   - Number input parties: %d\n", _number_input_parties);
    for (int i = 0; i < _number_input_parties; i++) {
        printf("      - Input party %d: %d wires\n", i, _number_wires_input_parties.at(i));
    }
    printf("   - Number output parties: %d\n", _number_output_parties);
    for (int i = 0; i < _number_output_parties; i++) {
        printf("      - Output party %d: %d wires\n", i, _number_wires_output_parties.at(i));
    }
}

void gabe::circuits::test::BristolTester::read_inputs(const std::string &inputs) {
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

void gabe::circuits::test::BristolTester::execute_circuit() {
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

        if (gate == "AND") {
            _wire_values[output_wires.at(0)] = _wire_values.at(input_wires.at(0)) & _wire_values.at(input_wires.at(1));
        }
        else if (gate == "OR") {
            _wire_values[output_wires.at(0)] = _wire_values.at(input_wires.at(0)) | _wire_values.at(input_wires.at(1));
        }
        else if (gate == "XOR") {
            _wire_values[output_wires.at(0)] = _wire_values.at(input_wires.at(0)) ^ _wire_values.at(input_wires.at(1));
        }
        else if (gate == "INV") {
            _wire_values[output_wires.at(0)] = !_wire_values.at(input_wires.at(0));
        }
        else {
            throw std::runtime_error("The gate is not recognized.");
        }
    }

    printf("> Output wires: ");
    for (int j = _number_wires_output_parties.at(0); j > 0 ; j--) {
        printf("%d ", _number_wires - j );
    } printf("\n");

    printf("> Output wires values: ");
    for (int j = _number_wires_output_parties.at(0); j > 0 ; j--) {
        printf("%d ", _wire_values.at( _number_wires - j ) );
    } printf("\n");
}
