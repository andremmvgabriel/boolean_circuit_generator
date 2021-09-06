#include <BristolCircuitTester.hpp>

gabe::circuits::test::BristolTester::BristolTester() : TesterAbs() {}

gabe::circuits::test::BristolTester::BristolTester(const std::string& circuit) : TesterAbs(circuit) {
    _read_header();

    // Assigns the correct gates
    _gates_map["xor"] = "XOR";
    _gates_map["and"] = "AND";
    _gates_map["inv"] = "INV";
    _gates_map["or"] = "OR";
}

gabe::circuits::test::BristolTester::~BristolTester() {}

void gabe::circuits::test::BristolTester::_read_header() {
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

#ifdef _WIN32
            // Returns the pointer back the whole string size
            _circuit_file.seekg( current_pos - line.size() - 2 );
#else
            // Returns the pointer back the whole string size
            _circuit_file.seekg( current_pos - line.size() - 1 );
#endif
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
