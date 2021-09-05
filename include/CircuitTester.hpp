#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <unordered_map>

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

                //
                std::unordered_map<std::string, std::string> _gates_map = {
                    {"xor", ""},
                    {"and", ""},
                    {"inv", ""},
                    {"or", ""},
                };

                // Circuit info
                std::vector<uint64_t> _number_wires_input_parties;
                std::vector<uint64_t> _number_wires_output_parties;
                uint64_t _number_gates = 0;
                uint64_t _number_wires = 0;
                uint32_t _number_input_parties = 0;
                uint32_t _number_output_parties = 0;

                // Control variables
                uint64_t _circuit_start = 0;
                std::unordered_map<uint64_t, uint8_t> _wire_values;

            protected:
                TesterAbs();
                TesterAbs(const std::string& circuit);

                virtual ~TesterAbs();

                virtual void read_header();
                void read_inputs(const std::string& inputs);
                void read_inputs(const std::vector<uint8_t>& inputs);
                void execute_circuit();
            
            public:
                // void load_circuit(const std::string& circuit);
                void run(const std::string& inputs);
                void run(const std::vector<uint8_t>& inputs);
            };
        }
    }
}