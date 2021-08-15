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
    std::ofstream _circuit_file;

    int _number_inputs;
    int _number_outputs;

    int _number_gates = 0;
    int _number_parties = 0;
    int _number_wires = 0;

    std::vector<int> _inputs_number_wires;
    std::vector<int> _outputs_number_wires;

private:
    CircuitGenerator() {}

public:
    CircuitGenerator(CircuitType circuit_type) : _circuit_type(circuit_type) {
        printf("> Circuit Generator object created.\n");
        printf("   - Target type: %s.\n\n",
            circuit_type == CircuitType::BRISTOL ? "Bristol" : "None"
        );

        _circuit_file = std::ofstream(
            "BristolCircuit.txt",
            std::ios::out
        );

        _circuit_file << "1234567\n";
    }

    ~CircuitGenerator() { printf("Closed.\n"); _circuit_file.close(); }

    void start() {
        //_circuit_file.write("2 1 0 0 ");

        printf("%d\n", _circuit_file.is_open());
        printf("> Writting...\n");

        std::string header = std::to_string(_number_gates) + " " + std::to_string(_number_wires);
        _circuit_file.write(&header[0], header.size());

        std::string header_l2 = std::to_string(_inputs_number_wires.size());
        for (auto & party_wires : _inputs_number_wires) {
            header_l2 += " " + std::to_string(party_wires);
        } header_l2 += "\n";
        _circuit_file.write(header_l2.c_str(), header_l2.size());

        std::string header_l3 = std::to_string(_outputs_number_wires.size());
        for (auto & party_wires : _outputs_number_wires) {
            header_l3 += " " + std::to_string(party_wires);
        } header_l3 += "\n";
        _circuit_file.write(header_l3.c_str(), header_l3.size());

        _circuit_file << "hello?";

        _circuit_file.write("test", 4);
    }

    void conclude() {}

    void add_input(Variable &input) {
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
    void add(Variable& input1, Variable& input2, Variable& output) {
        printf("> Addition...\n");
    }
};
