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
    second_line >> _number_inputs;
    for (int i = 0; i < _number_inputs; i++) {}

    // Prints circuit information
    printf("> Circuit Info:\n");
    printf("   - Number gates: %ld\n", _number_gates);
    printf("   - Number wires: %ld\n", _number_wires);
    /* printf("   - Number input parties: %d\n", input_wires.size());
    for (int i = 0; i < input_wires.size(); i++) {
        printf("      - Party %d number wires: %d\n", i, input_wires.at(i));
    }
    printf("   - Number output parties: %d\n", outputs_wires.size());
    for (int i = 0; i < outputs_wires.size(); i++) {
        printf("      - Party %d number wires: %d\n", i, outputs_wires.at(i));
    } */
}

void gabe::circuits::test::BristolTester::run(const std::string& inputs) {}
