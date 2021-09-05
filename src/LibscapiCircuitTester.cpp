#include <LibscapiCircuitTester.hpp>

gabe::circuits::test::LibscapiTester::LibscapiTester() : TesterAbs() {}

gabe::circuits::test::LibscapiTester::LibscapiTester(const std::string& circuit) : TesterAbs(circuit) {
    read_header();
    
    // Assigns the correct gates
    _gates_map["xor"] = "0110";
    _gates_map["and"] = "0001";
    _gates_map["inv"] = "10";
    _gates_map["or"] = "0111";
}

gabe::circuits::test::LibscapiTester::~LibscapiTester() {}

void gabe::circuits::test::LibscapiTester::read_header() {
    // TODO - Change this to be more dynamic

    // String to read all the lines
    std::string line;

    // Reads first line
    std::getline(_circuit_file, line);
    std::stringstream first_line(line);
    first_line >> _number_gates >> _number_input_parties;

    // Ignores empty line // TODO
    std::getline(_circuit_file, line);

    for (int i = 0; i < _number_input_parties; i++) {
        std::getline(_circuit_file, line);

        std::stringstream party_info(line);

        int party_number, number_wires;
        party_info >> party_number >> number_wires;

        if (i + 1 != party_number) {
            throw std::runtime_error("The circuit file does not have data for all the parties.");
        }

        _number_wires_input_parties.push_back(number_wires);

        for (int j = 0; j < number_wires; j++) {
            std::getline(_circuit_file, line);
        }

        // Ignores empty line // TODO
        std::getline(_circuit_file, line);
    }

    uint32_t current_party = 0;

    while (true) {
        std::getline(_circuit_file, line);

        if (!line.size()) { continue; }
        if (line.at(0) == ' ') { continue; }
        
        std::stringstream party_info(line);

        party_info >> current_party;

        if (current_party != _number_output_parties + 1) {
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

        int number_wires;

        party_info >> number_wires;

        std::string bait1, bait2;

        party_info >> bait1 >> bait2;

        if (bait1.size() || bait2.size()) {
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

        _number_output_parties++;
        _number_wires_output_parties.push_back(number_wires);

        std::vector<uint64_t> wires;

        for (int j = 0; j < number_wires; j++) {
            std::getline(_circuit_file, line);
            wires.push_back( std::stol(line) );
        }

        _output_party_wires.push_back( wires );
    }

    _number_wires = _output_party_wires.back().back() + 1;

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
