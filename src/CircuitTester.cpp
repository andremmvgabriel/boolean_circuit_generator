#include <CircuitTester.hpp>

/*
CircuitTester::CircuitTester() {}

CircuitTester::CircuitTester(CircuitType circuit_type, const std::string &circuit) : _circuit_type(circuit_type), _circuit_file(circuit, std::ios::in) {
    _read_header();
} 

void CircuitTester::_read_header() {
    switch (_circuit_type) {
        case CircuitType::BRISTOL: break;
        case CircuitType::BRISTOL_FASHION: break;
        case CircuitType::LIBSCAPI:
            //while (true) {
                std::string line;
                std::getline(_circuit_file, line);
                std::stringstream ss_line(line);

                ss_line >> _number_gates >> _number_inputs;

                printf("%ld %ld\n", _number_gates, _number_inputs);
            //}
            break;
    }
}

void CircuitTester::run(const std::string &inputs) {}
*/



gabe::circuits::test::TesterAbs::TesterAbs() {}

gabe::circuits::test::TesterAbs::TesterAbs(const std::string& circuit) : _circuit_file(circuit, std::ios::in) {}

gabe::circuits::test::TesterAbs::~TesterAbs() {
    _circuit_file.close();
}

void gabe::circuits::test::TesterAbs::read_header() {}

void gabe::circuits::test::TesterAbs::read_inputs(const std::string& inputs) {}

void gabe::circuits::test::TesterAbs::execute_circuit() {}

void gabe::circuits::test::TesterAbs::run(const std::string& inputs) {
    // Reads the inputs
    read_inputs(inputs);

    // Executes the circuit
    execute_circuit();

    // Returns the pointer to the start of the circuit
    _circuit_file.seekg( _circuit_start );
}
