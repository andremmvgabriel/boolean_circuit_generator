#pragma once

#include <CircuitGenerator.hpp>

namespace gabe
{
    namespace circuits
    {
        namespace generator
        {
            class LibscapiGenerator : public CircuitGenerator
            {
            protected:
                LibscapiGenerator();

                void _write_header() override;
            
            public:
                LibscapiGenerator(const std::string& circuit_file, const std::vector<uint64_t>& number_wires_input_parties, const std::vector<uint64_t>& number_wires_output_parties);
            };
        }
    }
}
