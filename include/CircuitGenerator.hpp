#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <Variable.hpp>
//#include <Circuit.hpp>

enum class CircuitType
{
    BRISTOL
};

class CircuitGenerator// : virtual public Circuit
{
private:
    CircuitType _circuit_type;
    std::ofstream _circuit_file;
    std::fstream _temp_circuit_file;

    int _number_inputs;
    int _number_outputs;

    int _number_gates = 0;
    int _number_parties = 0;
    uint64_t _number_wires = 0;

    std::vector<int> _inputs_number_wires;
    std::vector<int> _outputs_number_wires;

    uint64_t zero_wire;
    uint64_t one_wire;

    // Control
    int assigned_wires = 0;

    // Debug
    uint64_t _counter_xor_gates = 0;
    uint64_t _counter_inv_gates = 0;
    uint64_t _counter_and_gates = 0;
    uint64_t _counter_or_gates = 0;

private:
    CircuitGenerator();

    void _write_gate(uint64_t in_wire, uint64_t out_wire) {
    }

    void _write_gate(uint64_t in_wire1, uint64_t in_wire2, uint64_t out_wire) {
    }

public:
    CircuitGenerator(CircuitType circuit_type, const std::vector<int> &input_parties_wires, const std::vector<int> &output_parties_wires);

    ~CircuitGenerator() { printf("Closed.\n"); _circuit_file.close(); _temp_circuit_file.close(); }

    template<typename DataType>
    Variable create_constant(DataType value) {
        Variable output(8 * sizeof(DataType));

        for(int i = 0; i < 8 * sizeof(DataType); i++) {
            output.wires[i] = ((value >> i) & 0x01) == 0x00 ? zero_wire : one_wire;
        }

        return output;
    }

    Variable create_constant(int n_bits, uint64_t value);

    void start() {
        printf("> Writting...\n");

        _xor_gate( 0, 0, zero_wire );
        _inv_gate( zero_wire, one_wire );

        printf("Zero wire: %lld\n", zero_wire);
        printf("One wire: %lld\n", one_wire);
    }

    void conclude() {
        printf("> Final circuit details:\n");
        printf("   - TOTAL gates: %lld\n", _number_gates);
        printf("   - TOTAL wires: %lld\n", _number_wires);
        printf("   - XOR gates: %lld\n", _counter_xor_gates);
        printf("   - AND gates: %lld\n", _counter_and_gates);
        printf("   - INV gates: %lld\n", _counter_inv_gates);
        printf("   - OR gates: %lld\n", _counter_or_gates);





        std::string header = std::to_string(_number_gates) + " " + std::to_string(_number_wires) + "\n";
        _circuit_file.write(header.c_str(), header.size());

        std::string header_l2 = std::to_string(_inputs_number_wires.size());
        for (auto & party_wires : _inputs_number_wires) {
            header_l2 += " " + std::to_string(party_wires);
        } header_l2 += "\n";
        _circuit_file.write(header_l2.c_str(), header_l2.size());

        std::string header_l3 = std::to_string(_outputs_number_wires.size());
        for (auto & party_wires : _outputs_number_wires) {
            header_l3 += " " + std::to_string(party_wires);
        } header_l3 += "\n\n";
        _circuit_file.write(header_l3.c_str(), header_l3.size());

        _temp_circuit_file.seekg(0);

        std::string line;
        while (std::getline(_temp_circuit_file, line)) {
            line += "\n";
            _circuit_file.write(line.c_str(), line.size());
        }
    }

    void add_input(Variable &input) {
        for (int i = 0; i < input.number_wires; i++) {
            if (assigned_wires > _number_wires) {
                printf("There are no more input wires left to be assigned.\n");
            }

            input.wires[i] = assigned_wires++;
        }
    }

    void add_output(Variable &output) {
    }

    /*
    Truth table:

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
    */
    void addition(Variable& input1, Variable& input2, Variable& output) {
        printf("> Addition...\n");

        Variable c_var = create_constant(input1.number_wires, 0x00);

        Variable a_xor_b(input1.number_wires);
        Variable a_and_b(input1.number_wires);
        Variable a_xor_b_and_c(input1.number_wires);

        for (int i = 0; i < output.number_wires; i++) {
            _xor_gate( input1.wires[i], input2.wires[i], a_xor_b.wires[i] );
            _xor_gate( a_xor_b.wires[i], c_var.wires[i], output.wires[i] );
            if (i != output.number_wires - 1) {
                _and_gate( input1.wires[i], input2.wires[i], a_and_b.wires[i] );
                _and_gate( a_xor_b.wires[i], c_var.wires[i], a_xor_b_and_c.wires[i] );
                _or_gate( a_and_b.wires[i], a_xor_b_and_c.wires[i], c_var.wires[i+1] );
            }
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
    void subtraction(Variable& input1, Variable& input2, Variable& output) {
        printf("> Addition...\n");

        Variable c_var = create_constant<uint64_t>(0x00);

        Variable a_xor_b(input1.number_wires);
        Variable a_and_b(input1.number_wires);
        Variable a_xor_b_and_c(input1.number_wires);

        for (int i = 0; i < output.number_wires; i++) {
            _xor_gate( input1.wires[i], input2.wires[i], a_xor_b.wires[i] );
            _xor_gate( a_xor_b.wires[i], c_var.wires[i], output.wires[i] );
            if (i != output.number_wires - 1) {
                _and_gate( input1.wires[i], input2.wires[i], a_and_b.wires[i] );
                _and_gate( a_xor_b.wires[i], c_var.wires[i], a_xor_b_and_c.wires[i] );
                _or_gate( a_and_b.wires[i], a_xor_b_and_c.wires[i], c_var.wires[i+1] );
            }
        }
    }

    void _xor_gate(const uint64_t& wire_in1, const uint64_t& wire_in2, uint64_t& wire_out) {
        _number_gates++;
        _counter_xor_gates++;

        std::string gate = "2 1 ";

        gate += wire_in1 < wire_in2 ? std::to_string(wire_in1) + " " + std::to_string(wire_in2) : std::to_string(wire_in2) + " " + std::to_string(wire_in1);

        wire_out = _number_wires++;

        gate += " " + std::to_string(wire_out) + " XOR\n";

        _temp_circuit_file.write(gate.c_str(), gate.size());
    }

    void _inv_gate(const uint64_t& wire_in, uint64_t& wire_out) {
        _number_gates++;
        _counter_inv_gates++;

        std::string gate = "1 1 ";

        gate += std::to_string(wire_in);

        wire_out = _number_wires++;

        gate += " " + std::to_string(wire_out) + " INV\n";

        _temp_circuit_file.write(gate.c_str(), gate.size());
    }

    void _and_gate(const uint64_t& wire_in1, const uint64_t& wire_in2, uint64_t& wire_out) {
        _number_gates++;
        _counter_and_gates++;

        std::string gate = "2 1 ";

        gate += wire_in1 < wire_in2 ? std::to_string(wire_in1) + " " + std::to_string(wire_in2) : std::to_string(wire_in2) + " " + std::to_string(wire_in1);

        wire_out = _number_wires++;

        gate += " " + std::to_string(wire_out) + " AND\n";

        _temp_circuit_file.write(gate.c_str(), gate.size());
    }

    void _or_gate(const uint64_t& wire_in1, const uint64_t& wire_in2, uint64_t& wire_out) {
        _number_gates++;
        _counter_or_gates++;

        std::string gate = "2 1 ";

        gate += wire_in1 < wire_in2 ? std::to_string(wire_in1) + " " + std::to_string(wire_in2) : std::to_string(wire_in2) + " " + std::to_string(wire_in1);

        wire_out = _number_wires++;

        gate += " " + std::to_string(wire_out) + " OR\n";

        _temp_circuit_file.write(gate.c_str(), gate.size());
    }

    void XOR(Variable& input1, Variable& input2, Variable& output) {
        if (input1.number_wires != input2.number_wires && input1.number_wires != output.number_wires) {
            printf("The variables inputted do not share the same size.\n");
        }

        for (int i = 0; i < output.number_wires; i++) {
            _xor_gate(input1.wires[i], input2.wires[i], output.wires[i]);
        }
    }

    void INV(Variable& input, Variable& output) {
        if (input.number_wires != output.number_wires) {
            printf("The variables inputted do not share the same size.\n");
        }

        for (int i = 0; i < output.number_wires; i++) {
            _inv_gate(input.wires[i], output.wires[i]);
        }
    }

    void AND(Variable& input1, Variable& input2, Variable& output) {
        if (input1.number_wires != input2.number_wires && input1.number_wires != output.number_wires) {
            printf("The variables inputted do not share the same size.\n");
        }

        for (int i = 0; i < output.number_wires; i++) {
            _and_gate(input1.wires[i], input2.wires[i], output.wires[i]);
        }
    }

    void OR(Variable& input1, Variable& input2, Variable& output) {
        if (input1.number_wires != input2.number_wires && input1.number_wires != output.number_wires) {
            printf("The variables inputted do not share the same size.\n");
        }

        for (int i = 0; i < output.number_wires; i++) {
            _or_gate(input1.wires[i], input2.wires[i], output.wires[i]);
        }
    }
};
