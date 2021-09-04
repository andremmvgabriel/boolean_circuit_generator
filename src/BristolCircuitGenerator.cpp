#include <BristolCircuitGenerator.hpp>

gabe::circuits::generator::BristolGenerator::BristolGenerator() : CircuitGenerator() {}

gabe::circuits::generator::BristolGenerator::BristolGenerator(const std::string& circuit_file, const std::vector<uint64_t>& number_wires_input_parties, const std::vector<uint64_t>& number_wires_output_parties) : CircuitGenerator(circuit_file, number_wires_input_parties, number_wires_output_parties) {}
