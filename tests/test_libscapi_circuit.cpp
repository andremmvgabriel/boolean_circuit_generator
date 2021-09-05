#include <LibscapiCircuitTester.hpp>

std::unordered_map<std::string, std::string> default_circuits = {
    { "xor", "LibscapiCircuit_XOR.txt" },  
    { "and", "LibscapiCircuit_AND.txt" },  
    { "inv", "LibscapiCircuit_INV.txt" },  
    { "or", "LibscapiCircuit_OR.txt" },  
    { "addition", "LibscapiCircuit_addition.txt" },  
    { "subtraction", "LibscapiCircuit_subtraction.txt" },  
    { "multiplication", "LibscapiCircuit_multiplication.txt" },
    { "division", "LibscapiCircuit_division.txt" },
    { "multiplexer", "LibscapiCircuit_multiplexer.txt" },
    { "equal", "LibscapiCircuit_equal.txt" },
    { "greater", "LibscapiCircuit_greater.txt" },
    { "smaller", "LibscapiCircuit_smaller.txt" },
    { "greater_or_equal", "LibscapiCircuit_greater_or_equal.txt" },
    { "smaller_or_equal", "LibscapiCircuit_smaller_or_equal.txt" },
    { "comparator", "LibscapiCircuit_comparator.txt" }
};

void test_circuit(const std::string& circuit, const std::string& inputs) {
    gabe::circuits::test::LibscapiTester circuit_tester( circuit );

    circuit_tester.run( inputs );
}

void test_circuit(const std::string& circuit, const std::vector<uint8_t>& inputs) {
    gabe::circuits::test::LibscapiTester circuit_tester( circuit );

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