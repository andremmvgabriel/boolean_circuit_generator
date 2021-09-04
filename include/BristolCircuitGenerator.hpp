#pragma once

#include <CircuitGenerator.hpp>

namespace gabe
{
    namespace circuits
    {
        namespace generator
        {
            class BristolGenerator : public CircuitGenerator {
            protected:
                BristolGenerator();
            
            public:
                BristolGenerator(const std::string& circuit_file, const std::vector<uint64_t>& number_wires_input_parties, const std::vector<uint64_t>& number_wires_output_parties);
            };
        }
    }
}
