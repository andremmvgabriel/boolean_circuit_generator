#include <CircuitTester.hpp>

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
