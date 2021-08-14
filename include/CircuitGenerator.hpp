#pragma once

#include <iostream>
#include <fstream>

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

private:
    CircuitGenerator() {}

public:
    CircuitGenerator(CircuitType circuit_type) : _circuit_type(circuit_type) {
        printf("> Circuit Generator object created.\n");
        printf("   - Target type: %s.\n\n",
            circuit_type == CircuitType::BRISTOL ? "Bristol" : "None"
        );
    }

    void start() {}

    void conclude() {}

    void add_input(Variable &input) {
    }

    void add_output(Variable &output) {
    }

    void add(Variable& input1, Variable& input2, Variable& output) {
        printf("> Addition...\n");
    }
};
