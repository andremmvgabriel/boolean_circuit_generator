#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <Variable.hpp>

enum class CircuitType
{
    BRISTOL
};

class CircuitGenerator
{
private:
    CircuitType _circuit_type;
    std::fstream _circuit_file;

    int _number_inputs;
    int _number_outputs;

    int _number_gates = 0;
    int _number_parties = 0;
    int _number_wires = 0;

    std::vector<int> _inputs_number_wires;
    std::vector<int> _outputs_number_wires;

    uint64_t zero_wire;
    uint64_t one_wire;

    // Control
    int assigned_wires = 0;

private:
    CircuitGenerator() {}

    void _write_gate(uint64_t in_wire, uint64_t out_wire) {
    }

    void _write_gate(uint64_t in_wire1, uint64_t in_wire2, uint64_t out_wire) {
    }

public:
    CircuitGenerator(CircuitType circuit_type, const std::vector<int> &input_parties_wires, const std::vector<int> &output_parties_wires) : _circuit_type(circuit_type), _inputs_number_wires(input_parties_wires), _outputs_number_wires(output_parties_wires) {
        printf("> Circuit Generator object created.\n");
        printf("   - Target type: %s.\n\n",
            circuit_type == CircuitType::BRISTOL ? "Bristol" : "None"
        );

        _circuit_file = std::fstream(
            "BristolCircuit.txt",
            std::ios::in | std::ios::out
        );

        for (auto & amount : input_parties_wires) {
            _number_wires += amount;
        }
    }

    ~CircuitGenerator() { printf("Closed.\n"); _circuit_file.close(); }

    template<typename DataType>
    Variable create_constant(DataType value) {
        Variable output(8 * sizeof(DataType));

        printf("Creating variable with value: %d\n Wires:\n", value);
        for(int i = 0; i < 8 * sizeof(DataType); i++) {
            output.wires[i] = ((value >> i) & 0x01) == 0x00 ? zero_wire : one_wire;
            printf("   %ld\n", output.wires[i]);
        }

        return output;
    }

    void start() {
        printf("%d\n", _circuit_file.is_open());
        printf("> Writting...\n");

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

        _xor_gate( 0, 0, zero_wire );

        std::string always_1;
        if (_inputs_number_wires.size() > 0) {
            always_1 += "1 1 " + std::to_string(_number_wires);
            always_1 += " " + std::to_string(++_number_wires) + " INV\n";
        }
        _circuit_file.write(always_1.c_str(), always_1.size());
        one_wire = _number_wires;

        printf("Zero wire: %lld\n", zero_wire);
        printf("One wire: %lld\n", one_wire);
    }

    void conclude() {}

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
    
    A|----- (...)       --A--|               --1----|
                             |XOR|--1--             |XOR|--|INV|--O--
                        --B--|               --Cin--|
    B|----- (...)
                        --A--|               --2----|
                             |AND|--2--             |OR|------Cout---
    Cin|--- (...)       --B--|               --Cin--|

    */
    void addition(Variable& input1, Variable& input2, Variable& output) {
        printf("> Addition...\n");

        Variable Cin = create_constant((uint8_t)0x00);

        Variable XOR_output(input1.number_wires);
        Variable AND_output(input1.number_wires);
        Variable XOR_INV_output(input1.number_wires);

        for (int i = 0; i < output.number_wires; i++) {
        }
    }

    void _xor_gate(const uint64_t& wire_in1, const uint64_t& wire_in2, uint64_t& wire_out) {
        std::string gate = "2 1 ";

        gate += wire_in1 < wire_in2 ? std::to_string(wire_in1) + " " + std::to_string(wire_in2) : std::to_string(wire_in2) + " " + std::to_string(wire_in1);

        wire_out = ++_number_wires;

        gate += " " + std::to_string(wire_out) + " XOR\n";

        _circuit_file.write(gate.c_str(), gate.size());
    }

    void _inv_gate(const uint64_t& wire_in, uint64_t& wire_out) {
        std::string gate = "1 1 ";

        gate += std::to_string(wire_in);

        wire_out = ++_number_wires;

        gate += " " + std::to_string(wire_out) + " INV\n";

        _circuit_file.write(gate.c_str(), gate.size());
    }

    void _and_gate(const uint64_t& wire_in1, const uint64_t& wire_in2, uint64_t& wire_out) {
        std::string gate = "2 1 ";

        gate += wire_in1 < wire_in2 ? std::to_string(wire_in1) + " " + std::to_string(wire_in2) : std::to_string(wire_in2) + " " + std::to_string(wire_in1);

        wire_out = ++_number_wires;

        gate += " " + std::to_string(wire_out) + " AND\n";

        _circuit_file.write(gate.c_str(), gate.size());
    }

    void _or_gate(const uint64_t& wire_in1, const uint64_t& wire_in2, uint64_t& wire_out) {
        std::string gate = "2 1 ";

        gate += wire_in1 < wire_in2 ? std::to_string(wire_in1) + " " + std::to_string(wire_in2) : std::to_string(wire_in2) + " " + std::to_string(wire_in1);

        wire_out = ++_number_wires;

        gate += " " + std::to_string(wire_out) + " OR\n";

        _circuit_file.write(gate.c_str(), gate.size());
    }

    void XOR(Variable& input1, Variable& input2, Variable& output) {
        _number_gates++;

        if (input1.number_wires != input2.number_wires && input1.number_wires != output.number_wires) {
            printf("The variables inputted do not share the same size.\n");
        }

        for (int i = 0; i < output.number_wires; i++) {
            _xor_gate(input1.wires[i], input2.wires[i], output.wires[i]);
        }
    }

    void INV(Variable& input, Variable& output) {
        _number_gates++;

        if (input.number_wires != output.number_wires) {
            printf("The variables inputted do not share the same size.\n");
        }

        for (int i = 0; i < output.number_wires; i++) {
            _inv_gate(input.wires[i], output.wires[i]);
        }
    }

    void AND(Variable& input1, Variable& input2, Variable& output) {
        _number_gates++;

        if (input1.number_wires != input2.number_wires && input1.number_wires != output.number_wires) {
            printf("The variables inputted do not share the same size.\n");
        }

        for (int i = 0; i < output.number_wires; i++) {
            _and_gate(input1.wires[i], input2.wires[i], output.wires[i]);
        }
    }

    void OR(Variable& input1, Variable& input2, Variable& output) {
        _number_gates++;

        if (input1.number_wires != input2.number_wires && input1.number_wires != output.number_wires) {
            printf("The variables inputted do not share the same size.\n");
        }

        for (int i = 0; i < output.number_wires; i++) {
            _or_gate(input1.wires[i], input2.wires[i], output.wires[i]);
        }
    }
};
