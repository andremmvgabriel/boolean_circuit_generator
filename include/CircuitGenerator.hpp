#pragma once

#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <stdio.h>

#include <Variable.hpp>

namespace gabe
{
    namespace circuits
    {
        namespace generator
        {
            class CircuitGenerator
            {
            protected:
                // Files
                std::ofstream _circuit_file;
                std::fstream _temp_file;
                std::string _temp_file_name;

                // 
                std::unordered_map<std::string, std::string> _gates_map = {
                    {"xor", ""},
                    {"and", ""},
                    {"inv", ""},
                    {"or", ""},
                };

                // Zero and One wires
                Wire _zero_wire;
                Wire _one_wire;

                // Circuit info
                std::vector<uint64_t> _number_wires_input_parties;
                std::vector<uint64_t> _number_wires_output_parties;
                uint64_t _counter_gates = 0x00;
                uint64_t _counter_wires = 0x00;

                // Circuit control variables
                uint64_t _expected_input_wires = 0x00;
                uint64_t _expected_output_wires = 0x00;
                uint64_t _counter_output_wires = 0x00;

                // Circuit printable control variables
                uint64_t _counter_xor_gates = 0x00;
                uint64_t _counter_and_gates = 0x00;
                uint64_t _counter_inv_gates = 0x00;
                uint64_t _counter_or_gates = 0x00;

            protected:
                CircuitGenerator();
                CircuitGenerator(const std::string& circuit_file, const std::vector<uint64_t>& number_wires_input_parties, const std::vector<uint64_t>& number_wires_output_parties);

                void _assert_empty(const Variable& variable);
                void _assert_size(const Variable& variable, uint64_t size);
                void _assert_equal_size(const Variable& var1, const Variable& var2);

                void _make_same_size(Variable& var1, Variable& var2);

                virtual void _write_header();
                virtual void _write_circuit();

                void _write_1_1_gate(const uint64_t in, const uint64_t out, const std::string& gate);
                void _write_2_1_gate(const uint64_t in1, const uint64_t in2, const uint64_t out, const std::string& gate);

                void _xor_gate(const Wire& in1, const Wire& in2, Wire& out);
                void _and_gate(const Wire& in1, const Wire& in2, Wire& out);
                void _inv_gate(const Wire& in, Wire& out);
                void _or_gate(const Wire& in1, const Wire& in2, Wire& out);

            public:
                void start();
                void conclude();

                void add_input(Variable& input);
                void add_output(Variable& output);

                Variable create_constant(uint8_t n_bits, uint64_t value);

                // Basic operations
                //void XOR(const Variable& input1, const Variable& input2, Variable& output);
                //void AND(const Variable& input1, const Variable& input2, Variable& output);
                //void INV(const Variable& input, Variable& output);
                //void OR(const Variable& input1, const Variable& input2, Variable& output);
                void XOR(Variable input1, Variable input2, Variable& output);
                void AND(Variable input1, Variable input2, Variable& output);
                void INV(Variable input, Variable& output);
                void OR(Variable input1, Variable input2, Variable& output);

                // Arithmetic operations
                //void addition(const Variable& input1, const Variable& input2, Variable& output);
                void subtraction(const Variable& input1, const Variable& input2, Variable& output);
                void multiplication(const Variable& input1, const Variable& input2, Variable& output);
                void division(const Variable& input1, const Variable& input2, Variable& output_quotient, Variable& output_remainder);
                void division_quotient(const Variable& input1, const Variable& input2, Variable& output);
                void division_remainder(const Variable& input1, const Variable& input2, Variable& output);
                void addition(Variable input1, Variable input2, Variable& output);
                //void subtraction(const Variable& input1, const Variable& input2, Variable& output);
                //void multiplication(const Variable& input1, const Variable& input2, Variable& output);
                //void division(const Variable& input1, const Variable& input2, Variable& output_quotient, Variable& output_remainder);
                //void division_quotient(const Variable& input1, const Variable& input2, Variable& output);
                //void division_remainder(const Variable& input1, const Variable& input2, Variable& output);

                // Conditional operations
                void multiplexer(const Variable& control, const Variable& input1, const Variable& input2, Variable& output);
                void equal(const Variable& input1, const Variable& input2, Variable& output);
                void greater(const Variable& input1, const Variable& input2, Variable& output);
                void smaller(const Variable& input1, const Variable& input2, Variable& output);
                void greater_or_equal(const Variable& input1, const Variable& input2, Variable& output);
                void smaller_or_equal(const Variable& input1, const Variable& input2, Variable& output);
                void comparator(const Variable& input1, const Variable& input2, Variable& out_equal, Variable& out_greater, Variable &out_smaller);
            
            public:
                ~CircuitGenerator();
            };
        }
    }
}
