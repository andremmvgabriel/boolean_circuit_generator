#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <Variable.hpp>
//#include <Circuit.hpp>

enum class CircuitType
{
    BRISTOL,
    BRISTOL_FASHION,
    LIBSCAPI
};

class CircuitGenerator// : virtual public Circuit
{
private:
    int _number_inputs;
    int _number_outputs;

    int _number_gates = 0;
    int _number_parties = 0;
    uint64_t _number_wires = 0;

    std::vector<int> _inputs_number_wires;
    std::vector<int> _outputs_number_wires;

    uint64_t zero_wire;
    uint64_t one_wire;

    // Debug
    uint64_t _counter_xor_gates = 0;
    uint64_t _counter_inv_gates = 0;
    uint64_t _counter_and_gates = 0;
    uint64_t _counter_or_gates = 0;




    CircuitType _circuit_type;
    std::ofstream _circuit_file;
    std::fstream _temp_circuit_file;

    // Control variables
    uint64_t _expected_input_wires = 0x00;
    uint64_t _expected_output_wires = 0x00;

    // Zero and One wires
    gabe::circuits::Wire _zero;
    gabe::circuits::Wire _one;


private:
    CircuitGenerator();

    void _write_2_1_gate();
    void _write_1_1_gate();

    void _xor_gate(const gabe::circuits::Wire& in1, const gabe::circuits::Wire& in2, gabe::circuits::Wire& out);
    void _inv_gate(const gabe::circuits::Wire& in, gabe::circuits::Wire& out);
    void _and_gate(const gabe::circuits::Wire& in1, const gabe::circuits::Wire& in2, gabe::circuits::Wire& out);
    void _or_gate(const gabe::circuits::Wire& in1, const gabe::circuits::Wire& in2, gabe::circuits::Wire& out);

public:
    CircuitGenerator(CircuitType circuit_type, const std::vector<int> &input_parties_wires, const std::vector<int> &output_parties_wires);

    ~CircuitGenerator() { printf("Closed.\n"); _circuit_file.close(); _temp_circuit_file.close(); }

    // template<typename DataType>
    // Variable create_constant(DataType value) {
    //     Variable output(8 * sizeof(DataType));

    //     for(int i = 0; i < 8 * sizeof(DataType); i++) {
    //         output.wires[i] = ((value >> i) & 0x01) == 0x00 ? zero_wire : one_wire;
    //     }

    //     return output;
    // }

    gabe::circuits::Variable create_constant(int n_bits, uint64_t value);

    void start() {
        printf("> Writting...\n");

        _xor_gate( gabe::circuits::Wire(), gabe::circuits::Wire(), _zero );
        _inv_gate( _zero, _one );

        printf("Zero wire: %lld\n", _zero.label);
        printf("One wire: %lld\n", _one.label);
    }

    void conclude();

    void add_input(gabe::circuits::Variable &input);

    void add_output(gabe::circuits::Variable &output) {
    }

    /*
    > Truth table:

    A   B   Cin |   O   Cout
    ------------------------
    0   0   0   |   0   0
    0   0   1   |   1   0
    0   1   0   |   1   0
    0   1   1   |   0   1
    1   0   0   |   1   0
    1   0   1   |   0   1
    1   1   0   |   0   1
    1   1   1   |   1   1


    > Karnaugh maps:

    O (A, B, Cin)    
    */
    void addition(gabe::circuits::Variable& input1, gabe::circuits::Variable& input2, gabe::circuits::Variable& output) {
        printf("> Addition...\n");

        gabe::circuits::Variable c_var = create_constant(input1.number_wires, 0x00);

        gabe::circuits::Variable a_xor_b(input1.number_wires);
        gabe::circuits::Variable a_and_b(input1.number_wires);
        gabe::circuits::Variable a_xor_b_and_c(input1.number_wires);

        for (int i = 0; i < output.number_wires; i++) {
            _xor_gate( input1.wires[i], input2.wires[i], a_xor_b.wires[i] );
            
            if (i != output.number_wires - 1) {
                _and_gate( input1.wires[i], input2.wires[i], a_and_b.wires[i] );
                _and_gate( a_xor_b.wires[i], c_var.wires[i], a_xor_b_and_c.wires[i] );
                _or_gate( a_and_b.wires[i], a_xor_b_and_c.wires[i], c_var.wires[i+1] );
            }
        }

        // Output
        for (int i = 0; i < output.number_wires; i++) {
            _xor_gate( a_xor_b.wires[i], c_var.wires[i], output.wires[i] );
        }
    }

    /*
    Truth table:

    A   B   Cin |   O   Cout
    ------------------------
    0   0   0   |   0   0
    0   0   1   |   1   1
    0   1   0   |   1   1
    0   1   1   |   0   1
    1   0   0   |   1   0
    1   0   1   |   0   0
    1   1   0   |   0   0
    1   1   1   |   1   1
    */
    void subtraction(gabe::circuits::Variable& input1, gabe::circuits::Variable& input2, gabe::circuits::Variable& output) {
        printf("> Subtraction...\n");

        gabe::circuits::Variable c_io = create_constant(output.number_wires, 0x00);

        //Wire wire1, wire2, wire3, wire4, wire5;
        gabe::circuits::Variable a_xor_b(output.number_wires);
        gabe::circuits::Variable inv_xor(output.number_wires);
        gabe::circuits::Variable inv_a(output.number_wires);
        gabe::circuits::Variable and_xor(output.number_wires);
        gabe::circuits::Variable and_a_c(output.number_wires);

        for (int i = 0; i < output.number_wires; i++) {
            _xor_gate( input1.wires[i], input2.wires[i], a_xor_b.wires[i] );
            if (i != output.number_wires-1) {
                _inv_gate( a_xor_b.wires[i], inv_xor.wires[i] );
                _inv_gate( input1.wires[i], inv_a.wires[i] );
                _and_gate( inv_xor.wires[i], c_io.wires[i], and_xor.wires[i] );
                _and_gate( inv_a.wires[i], input2.wires[i], and_a_c.wires[i] );
                _or_gate( and_xor.wires[i], and_a_c.wires[i], c_io.wires[i+1] );
            }
        }

        // Output
        for (int i = 0; i < output.number_wires; i++) {
            _xor_gate( a_xor_b.wires[i], c_io.wires[i], output.wires[i] );
        }
    }

    void multiplication(gabe::circuits::Variable& input1, gabe::circuits::Variable& input2, gabe::circuits::Variable& output) {
        output = gabe::circuits::Variable(input1.number_wires + input2.number_wires);

        std::vector<gabe::circuits::Variable> vars (input2.number_wires);

        // Creation of the multiple variables
        for (int i = 0; i < vars.size(); i++) {
            vars.at(i) = create_constant(input1.number_wires + input2.number_wires, 0x00);

            for (int j = 0; j < input1.number_wires; j++) {
                _and_gate( input1.wires[j], input2.wires[i], vars.at(i).wires[i+j] );
            }
        }

        // 
        for (int i = 0; i < vars.size() - 1 ; i++) {
            if (i == 0) {
                addition( vars.at(i), vars.at(i+1), output );
            }
            else {
                addition( output, vars.at(i+1), output );
            }
        }
    }

    void division() {}

    void equal(gabe::circuits::Variable& input1, gabe::circuits::Variable& input2, gabe::circuits::Variable& output) {
        // Safety check

        gabe::circuits::Variable xor_outputs(input1.number_wires);

        for (int i = 0; i < input1.number_wires; i++) {
            _xor_gate(input1.wires[i], input2.wires[i], xor_outputs.wires[i]);
        }

        for (int i = 0; i < xor_outputs.number_wires - 1; i++) {
            if (i == 0) {
                _or_gate(xor_outputs.wires[i], xor_outputs.wires[i+1], output.wires[0]);
            } else {
                _or_gate(xor_outputs.wires[i+1], output.wires[0], output.wires[0]);
            }
        }

        _inv_gate(output.wires[0], output.wires[0]);
    }

    void multiplexer(gabe::circuits::Variable& control, gabe::circuits::Variable& input1, gabe::circuits::Variable& input2, gabe::circuits::Variable& output) {
        gabe::circuits::Variable not_control(control.number_wires);

        _inv_gate(control.wires[0], not_control.wires[0]);

        gabe::circuits::Variable and_in1(input1.number_wires);
        gabe::circuits::Variable and_in2(input2.number_wires);

        for (int i = 0; i < input1.number_wires; i++) {
            _and_gate( not_control.wires[0], input1.wires[i], and_in1.wires[i] );
            _and_gate( control.wires[0], input2.wires[i], and_in2.wires[i] );
        }

        for (int i = 0; i < output.number_wires; i++) {
            _or_gate( and_in1.wires[i], and_in2.wires[i], output.wires[i] );
        }
    }

    void comparator(gabe::circuits::Variable& input1, gabe::circuits::Variable& input2, gabe::circuits::Variable& output_smaller, gabe::circuits::Variable& output_equal, gabe::circuits::Variable& output_greater) {
        gabe::circuits::Variable not_input1(input1.number_wires);
        gabe::circuits::Variable not_input2(input2.number_wires);

        gabe::circuits::Variable xnors(input1.number_wires);

        // Prepares the ground base
        for (int i = 0; i < input1.number_wires; i++) {
            _inv_gate(input1.wires[i], not_input1.wires[i]);
            _inv_gate(input2.wires[i], not_input2.wires[i]);

            if (i != 0) {
                _xor_gate(input1.wires[i], input2.wires[i], xnors.wires[i]);
                _inv_gate(xnors.wires[i], xnors.wires[i]);
            }
        }

        gabe::circuits::Variable middle_greater(input1.number_wires);
        gabe::circuits::Variable middle_smaller(input2.number_wires);

        for (int i = 0; i < middle_greater.number_wires; i++) {
            for (int j = i; j < middle_greater.number_wires; j++) {
                if (i == j) {
                    _and_gate(input1.wires[j], not_input2.wires[j], middle_greater.wires[i]);
                    _and_gate(not_input1.wires[j], input2.wires[j], middle_smaller.wires[i]);
                }
                else {
                    _and_gate(middle_greater.wires[i], xnors.wires[j], middle_greater.wires[i]);
                    _and_gate(middle_smaller.wires[i], xnors.wires[j], middle_smaller.wires[i]);
                }
            }
        }

        for (int i = 1; i < middle_greater.number_wires; i++) {
            if (i == 1) {
                _or_gate( middle_greater.wires[i - 1], middle_greater.wires[i], output_greater.wires[0] );
                _or_gate( middle_smaller.wires[i - 1], middle_smaller.wires[i], output_smaller.wires[0] );
            }
            else {
                _or_gate( middle_greater.wires[i], output_greater.wires[0], output_greater.wires[0] );
                _or_gate( middle_smaller.wires[i], output_smaller.wires[0], output_smaller.wires[0] );
            }
        }

        _or_gate(output_greater.wires[0], output_greater.wires[0], output_equal.wires[0]);
    }

    void greater(gabe::circuits::Variable& input1, gabe::circuits::Variable& input2, gabe::circuits::Variable& output) {
        // Safety check


        gabe::circuits::Variable not_input2(input2.number_wires);
        gabe::circuits::Variable xnors(input1.number_wires);

        // Prepares the ground base
        for (int i = 0; i < input1.number_wires; i++) {
            _inv_gate(input2.wires[i], not_input2.wires[i]);

            if (i != 0) {
                _xor_gate(input1.wires[i], input2.wires[i], xnors.wires[i]);
                _inv_gate(xnors.wires[i], xnors.wires[i]);
            }
        }

        gabe::circuits::Variable middle(input1.number_wires);
        for (int i = 0; i < middle.number_wires; i++) {
            for (int j = i; j < middle.number_wires; j++) {
                if (i == j) {
                    _and_gate(input1.wires[j], not_input2.wires[j], middle.wires[i]);
                }
                else {
                    _and_gate(middle.wires[i], xnors.wires[j], middle.wires[i]);
                }
            }
        }

        for (int i = 1; i < middle.number_wires; i++) {
            if (i == 1) {
                _or_gate( middle.wires[i - 1], middle.wires[i], output.wires[0] );
            }
            else {
                _or_gate( middle.wires[i], output.wires[0], output.wires[0] );
            }
        }
    }

    void smaller(gabe::circuits::Variable& input1, gabe::circuits::Variable& input2, gabe::circuits::Variable& output) {
        // Safety check


        gabe::circuits::Variable not_input1(input1.number_wires);
        gabe::circuits::Variable xnors(input1.number_wires);

        // Prepares the ground base
        for (int i = 0; i < input1.number_wires; i++) {
            _inv_gate(input1.wires[i], not_input1.wires[i]);

            if (i != 0) {
                _xor_gate(input1.wires[i], input2.wires[i], xnors.wires[i]);
                _inv_gate(xnors.wires[i], xnors.wires[i]);
            }
        }

        gabe::circuits::Variable middle(input1.number_wires);
        for (int i = 0; i < middle.number_wires; i++) {
            for (int j = i; j < middle.number_wires; j++) {
                if (i == j) {
                    _and_gate(not_input1.wires[j], input2.wires[j], middle.wires[i]);
                }
                else {
                    _and_gate(middle.wires[i], xnors.wires[j], middle.wires[i]);
                }
            }
        }

        for (int i = 1; i < middle.number_wires; i++) {
            if (i == 1) {
                _or_gate( middle.wires[i - 1], middle.wires[i], output.wires[0] );
            }
            else {
                _or_gate( middle.wires[i], output.wires[0], output.wires[0] );
            }
        }
    }

    void greater_or_equal(gabe::circuits::Variable& input1, gabe::circuits::Variable& input2, gabe::circuits::Variable& output) {
        smaller(input1, input2, output);
        _inv_gate(output.wires[0], output.wires[0]);
    }

    void smaller_or_equal(gabe::circuits::Variable& input1, gabe::circuits::Variable& input2, gabe::circuits::Variable& output) {
        greater(input1, input2, output);
        _inv_gate(output.wires[0], output.wires[0]);
    }

    void XOR(gabe::circuits::Variable& input1, gabe::circuits::Variable& input2, gabe::circuits::Variable& output) {
        if (input1.number_wires != input2.number_wires && input1.number_wires != output.number_wires) {
            printf("The variables inputted do not share the same size.\n");
        }

        for (int i = 0; i < output.number_wires; i++) {
            _xor_gate(input1.wires[i], input2.wires[i], output.wires[i]);
        }
    }

    void INV(gabe::circuits::Variable& input, gabe::circuits::Variable& output) {
        if (input.number_wires != output.number_wires) {
            printf("The variables inputted do not share the same size.\n");
        }

        for (int i = 0; i < output.number_wires; i++) {
            _inv_gate(input.wires[i], output.wires[i]);
        }
    }

    void AND(gabe::circuits::Variable& input1, gabe::circuits::Variable& input2, gabe::circuits::Variable& output) {
        if (input1.number_wires != input2.number_wires && input1.number_wires != output.number_wires) {
            printf("The variables inputted do not share the same size.\n");
        }

        for (int i = 0; i < output.number_wires; i++) {
            _and_gate(input1.wires[i], input2.wires[i], output.wires[i]);
        }
    }

    void OR(gabe::circuits::Variable& input1, gabe::circuits::Variable& input2, gabe::circuits::Variable& output) {
        if (input1.number_wires != input2.number_wires && input1.number_wires != output.number_wires) {
            printf("The variables inputted do not share the same size.\n");
        }

        for (int i = 0; i < output.number_wires; i++) {
            _or_gate(input1.wires[i], input2.wires[i], output.wires[i]);
        }
    }
};

#include <unordered_map>
#include <stdio.h>

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
                std::unordered_map<std::string, std::string> _gates_map;

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

                void _assert_equal_size(const Variable& var1, const Variable& var2);

                void _write_header();
                void _write_circuit();

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
                void XOR(const Variable& input1, const Variable& input2, Variable& output);
                void AND(const Variable& input1, const Variable& input2, Variable& output);
                void INV(const Variable& input, Variable& output);
                void OR(const Variable& input1, const Variable& input2, Variable& output);

                // Arithmetic operations
                void addition(const Variable& input1, const Variable& input2, Variable& output);
                void subtraction(const Variable& input1, const Variable& input2, Variable& output);
                void multiplication(const Variable& input1, const Variable& input2, Variable& output);
                void division(const Variable& input1, const Variable& input2, Variable& output);

                // Conditional operations
                void multiplexer(const Variable& control, const Variable& input1, const Variable& input2, Variable& output);
            
            public:
                ~CircuitGenerator();
            };
        }
    }
}
