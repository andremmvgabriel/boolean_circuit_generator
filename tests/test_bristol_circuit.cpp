#include <iostream>

#include <fstream>
#include <vector>
#include <string>

#include <unordered_map>

class CircuitTester
{
public:
    void run(const std::string &circuit, const std::string &inputs) {
        // Opens the files
        std::ifstream circuit_file ( circuit, std::ios::in );
        std::ifstream inputs_file ( inputs, std::ios::in );

        std::string line;
        size_t found = std::string::npos;

        uint64_t number_gates = 0;
        uint64_t number_wires = 0;
        std::vector<int> input_wires;
        std::vector<int> outputs_wires;
        std::unordered_map<uint64_t, uint8_t> wires_values;

        // 1st line
        std::getline(circuit_file, line);
        found = line.find(" ");
        std::string gates(line.begin(), line.begin()+found);
        std::string wires(line.begin()+found, line.end());
        number_gates = std::stoi(gates);
        number_gates = std::stoi(wires);

        for (int i = 0; i < number_gates; i++) {
            wires_values[i] = 0x00;
        }

        // 2nd line
        std::getline(circuit_file, line);
        found = line.find(" ");
        std::string number_input_parties(line.begin(), line.begin()+found);
        input_wires.resize( std::stoi(number_input_parties) );
        line = std::string(line.begin()+found+1, line.end());
        found = line.find(" ");
        std::string input_size1(line.begin(), line.begin()+found);
        std::string input_size2(line.begin()+found+1, line.end());
        input_wires.at(0) = std::stoi(input_size1);
        input_wires.at(1) = std::stoi(input_size2);

        // 3rd line
        std::getline(circuit_file, line);
        found = line.find(" ");
        std::string number_output_parties(line.begin(), line.begin()+found);
        outputs_wires.resize( std::stoi(number_output_parties) );
        line = std::string(line.begin()+found+1, line.end());
        outputs_wires.at(0) = std::stoi(line);

        // 4th line
        std::getline(circuit_file, line);

        printf("> Circuit Info:\n");
        printf("   - Number gates: %d\n", number_gates);
        printf("   - Number wires: %d\n", number_wires);
        /*
        for (int i = 0; i < input_wires.at(0); i++) {
            std::getline(inputs_file, line);
            wires_values.at(i) = (uint8_t)(std::stoi(line));
        }

        for (int i = 0; i < input_wires.at(1); i++) {
            std::getline(inputs_file, line);
            wires_values.at(i + input_wires.at(0)) = (uint8_t)(std::stoi(line));
        }

        while (std::getline(circuit_file, line)) {
            int n_in_wires = std::stoi( std::string(line.begin(), line.begin()+1) );
            int n_out_wires = std::stoi( std::string(line.begin()+2, line.begin()+3) );
            line = std::string(line.begin()+4, line.end());
            if (n_in_wires == 1) {
                found = line.find(" ");
                uint64_t wire1 = (uint64_t)(int)std::stoi(std::string(line.begin(), line.begin()+found));
                line = std::string(line.begin()+found+1, line.end());
                found = line.find(" ");
                uint64_t out_wire = (uint64_t)(int)std::stoi(std::string(line.begin(), line.begin()+found));
                std::string gate = std::string(line.begin()+found+1, line.end());

                if (gate == "INV") {
                    wires_values.at(out_wire) = !wires_values.at(wire1);
                }
            } else if (n_in_wires == 2)
            {
                found = line.find(" ");
                uint64_t wire1 = (uint64_t)(int)std::stoi(std::string(line.begin(), line.begin()+found));
                line = std::string(line.begin()+found+1, line.end());
                found = line.find(" ");
                uint64_t wire2 = (uint64_t)(int)std::stoi(std::string(line.begin(), line.begin()+found));
                line = std::string(line.begin()+found+1, line.end());
                found = line.find(" ");
                uint64_t out_wire = (uint64_t)(int)std::stoi(std::string(line.begin(), line.begin()+found));
                std::string gate = std::string(line.begin()+found+1, line.end());

                if (gate == "XOR") {
                    wires_values.at(out_wire) = wires_values.at(wire1) ^ wires_values.at(wire2);
                } else if (gate == "AND") {
                    wires_values.at(out_wire) = wires_values.at(wire1) & wires_values.at(wire2);
                } else if (gate == "or") {
                    wires_values.at(out_wire) = wires_values.at(wire1) | wires_values.at(wire2);
                }
            }
        }

        for (int i = number_output_parties[0]; i > 0; i--) {
            printf("%d", wires_values.at(number_wires - i));
        } printf("\n"); */

        // Closes the files
        circuit_file.close();
        inputs_file.close();
    }
};

int main(int argc, char* argv[]) {
    CircuitTester circuit_tester;

    circuit_tester.run(argv[1], argv[2]);

    return 0;
}