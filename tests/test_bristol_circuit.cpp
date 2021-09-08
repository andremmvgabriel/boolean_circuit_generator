#include <BristolCircuitTester.hpp>

std::unordered_map<std::string, std::string> default_circuits = {
    { "xor", "BristolCircuit_XOR.txt" },  
    { "and", "BristolCircuit_AND.txt" },  
    { "inv", "BristolCircuit_INV.txt" },  
    { "or", "BristolCircuit_OR.txt" },  
    { "addition", "BristolCircuit_addition.txt" },  
    { "subtraction", "BristolCircuit_subtraction.txt" },  
    { "multiplication", "BristolCircuit_multiplication.txt" },
    { "division", "BristolCircuit_division.txt" },
    { "division_quotient", "BristolCircuit_division_quotient.txt" },
    { "division_remainder", "BristolCircuit_division_remainder.txt" },
    { "multiplexer", "BristolCircuit_multiplexer.txt" },
    { "equal", "BristolCircuit_equal.txt" },
    { "greater", "BristolCircuit_greater.txt" },
    { "smaller", "BristolCircuit_smaller.txt" },
    { "greater_or_equal", "BristolCircuit_greater_or_equal.txt" },
    { "smaller_or_equal", "BristolCircuit_smaller_or_equal.txt" },
    { "comparator", "BristolCircuit_comparator.txt" }
};

void test_circuit(const std::string& circuit, const std::string& inputs) {
    gabe::circuits::test::BristolTester circuit_tester( circuit );

    circuit_tester.run( inputs );
}

void test_circuit(const std::string& circuit, const std::vector<uint8_t>& inputs) {
    gabe::circuits::test::BristolTester circuit_tester( circuit );

    circuit_tester.run( inputs );
}

int main(int argc, char* argv[]) {
    if (argc <= 1) {
        printf("You didn't insert any argument...\n");
        return 0;
    }

    std::vector<uint8_t> manual_input = {
        0,0,0,0,0,0,0,0, // Party 1
        0,1,1,1,1,1,1,0 // Party 2
    };
 
    switch (argc) {
        case 2: 
            test_circuit(
                default_circuits[ argv[1] ],
                manual_input
            );
            break;
        case 3:
            test_circuit(
                default_circuits[ argv[1] ],
                argv[2]
            );
            break;
        case 4:
            test_circuit(
                argv[2],
                argv[3]
            );
            break;
    }

    return 0;
}