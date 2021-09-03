#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>

/* enum class CircuitType
{
    BRISTOL,
    BRISTOL_FASHION,
    LIBSCAPI
};

class CircuitTester
{
private:
    CircuitType _circuit_type;
    std::ifstream _circuit_file;

    // Circuit Specs
    uint64_t _number_gates = 0;
    uint64_t _number_wires = 0;
    uint64_t _number_inputs = 0;
    uint64_t _number_outputs = 0;

    // Control variables
    int _begin_of_circuit = 0;

private:
    CircuitTester();

    void _read_header();

public:
    CircuitTester(CircuitType circuit_type, const std::string &circuit);

    void run(const std::string &inputs);
}; */


namespace gabe
{
    namespace circuits
    {
        namespace test
        {
            class TesterAbs
            {
            protected:
                // Core variables
                std::ifstream _circuit_file;

                // Circuit specs
                uint64_t _number_gates = 0;
                std::unordered_map<uint64_t, uint8_t> _wire_values;

                // Control Variables
                uint64_t _circuit_start = 0;

            protected:
                TesterAbs();
                TesterAbs(const std::string& circuit);

                virtual ~TesterAbs();

                virtual void read_header();
                virtual void read_inputs(const std::string& inputs);
                virtual void execute_circuit();
            
            public:
                void run(const std::string& inputs);
            };
        }
    }
}