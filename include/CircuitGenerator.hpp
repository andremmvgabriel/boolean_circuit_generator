#pragma once

#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <stdio.h>

#include <UnsignedVariable.hpp>
#include <SignedVariable.hpp>

#define ALLOW_OR_GATES 0

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

                // Circuit printable control variables
                uint64_t _counter_xor_gates = 0x00;
                uint64_t _counter_and_gates = 0x00;
                uint64_t _counter_inv_gates = 0x00;
                uint64_t _counter_or_gates = 0x00;

            protected:
                CircuitGenerator();
                CircuitGenerator(const std::string& circuit_file, const std::vector<uint64_t>& number_wires_input_parties, const std::vector<uint64_t>& number_wires_output_parties);

                void _assert_equal_size(const UnsignedVariable& var1, const UnsignedVariable& var2);
                void _assert_equal_size(const SignedVariable& var1, const SignedVariable& var2);

                UnsignedVariable _signed_to_unsigned(const SignedVariable& input);
                SignedVariable _unsigned_to_signed(const UnsignedVariable& input);

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

                void add_input(UnsignedVariable& input);
                void add_input(SignedVariable& input);
                void add_output(UnsignedVariable& output);
                void add_output(SignedVariable& output);

                UnsignedVariable create_constant(uint8_t n_bits, uint64_t value);
                //SignedVariable create_constant(uint8_t n_bits, int64_t value);

                // Basic unsigned operations
                void XOR(const UnsignedVariable& input1, const UnsignedVariable& input2, UnsignedVariable& output);
                void AND(const UnsignedVariable& input1, const UnsignedVariable& input2, UnsignedVariable& output);
                void INV(const UnsignedVariable& input, UnsignedVariable& output);
                void OR(const UnsignedVariable& input1, const UnsignedVariable& input2, UnsignedVariable& output);

                // Basic signed operations
                void XOR(const SignedVariable& input1, const SignedVariable& input2, SignedVariable& output);
                void AND(const SignedVariable& input1, const SignedVariable& input2, SignedVariable& output);
                void INV(const SignedVariable& input, SignedVariable& output);
                void OR(const SignedVariable& input1, const SignedVariable& input2, SignedVariable& output);

                // Arithmetic unsigned operations
                void addition(const UnsignedVariable& input1, const UnsignedVariable& input2, UnsignedVariable& output);
                void subtraction(const UnsignedVariable& input1, const UnsignedVariable& input2, UnsignedVariable& output);
                void multiplication(const UnsignedVariable& input1, const UnsignedVariable& input2, UnsignedVariable& output);
                void division(const UnsignedVariable& input1, const UnsignedVariable& input2, UnsignedVariable& output_quotient, UnsignedVariable& output_remainder);
                void division_quotient(const UnsignedVariable& input1, const UnsignedVariable& input2, UnsignedVariable& output);
                void division_remainder(const UnsignedVariable& input1, const UnsignedVariable& input2, UnsignedVariable& output);

                // Arithmetic signed operations
                void multiplication(const SignedVariable& input1, const SignedVariable& input2, SignedVariable& output);
                void division(const SignedVariable& input1, const SignedVariable& input2, SignedVariable& output_quotient, SignedVariable& output_remainder);
                void division_quotient(const SignedVariable& input1, const SignedVariable& input2, SignedVariable& output);
                void division_remainder(const SignedVariable& input1, const SignedVariable& input2, SignedVariable& output);

                // Conditional operations
                void multiplexer(const UnsignedVariable& control, const UnsignedVariable& input1, const UnsignedVariable& input2, UnsignedVariable& output);
                void equal(const UnsignedVariable& input1, const UnsignedVariable& input2, UnsignedVariable& output);
                void greater(const UnsignedVariable& input1, const UnsignedVariable& input2, UnsignedVariable& output);
                void smaller(const UnsignedVariable& input1, const UnsignedVariable& input2, UnsignedVariable& output);
                void greater_or_equal(const UnsignedVariable& input1, const UnsignedVariable& input2, UnsignedVariable& output);
                void smaller_or_equal(const UnsignedVariable& input1, const UnsignedVariable& input2, UnsignedVariable& output);
                void comparator(const UnsignedVariable& input1, const UnsignedVariable& input2, UnsignedVariable& out_equal, UnsignedVariable& out_greater, UnsignedVariable &out_smaller);

                // Complement operations
                void complement_2s(const UnsignedVariable& input, UnsignedVariable& output);
                void complement_2s(const SignedVariable& input, SignedVariable& output);
            
            public:
                ~CircuitGenerator();
            };
        }
    }
}
