#include <iostream>

#include <fstream>
#include <vector>
#include <string>
#include <sstream>

#include <unordered_map>

class Circuit
{
public:
    virtual ~Circuit() {}
};

class CircuitTester : virtual public Circuit
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
        std::stringstream first_line(line);
        first_line >> number_gates >> number_wires;

        // 2nd line
        std::getline(circuit_file, line);
        std::stringstream second_line(line);
        int number_input_parties;
        second_line >> number_input_parties;
        for (int i = 0; i < number_input_parties; i++) {
            int party_wires;
            second_line >> party_wires;
            input_wires.push_back( party_wires );
        }

        // 3rd line
        std::getline(circuit_file, line);
        std::stringstream third_line(line);
        int number_output_parties;
        third_line >> number_output_parties;
        for (int i = 0; i < number_output_parties; i++) {
            int party_wires;
            third_line >> party_wires;
            outputs_wires.push_back( party_wires );
        }

        // 4th line
        std::getline(circuit_file, line); // Ignores this line

        printf("> Circuit Info:\n");
        printf("   - Number gates: %d\n", number_gates);
        printf("   - Number wires: %d\n", number_wires);
        printf("   - Number input parties: %d\n", input_wires.size());
        for (int i = 0; i < input_wires.size(); i++) {
            printf("      - Party %d number wires: %d\n", i, input_wires.at(i));
        }
        printf("   - Number output parties: %d\n", outputs_wires.size());
        for (int i = 0; i < outputs_wires.size(); i++) {
            printf("      - Party %d number wires: %d\n", i, outputs_wires.at(i));
        }

        for (int i = 0; i < input_wires.size(); i++) {
            for (int j = 0; j < input_wires.at(i); j++) {
                std::getline(inputs_file, line);
                wires_values[i == 0 ? j : j + i * input_wires.at(i-1)] = (uint8_t)(std::stoi(line));
            }
        }

        printf("\n");

        for (int i = 0; i < input_wires.size(); i++) {
            printf("> Party %d wires values: ", i);
            for (int j = 0; j < input_wires.at(i); j++) {
                //printf("%d ", wires_values.at(i == 0 ? j : j + i * input_wires.at(i-1)));
                printf("%d ", wires_values.at(i == 0 ? j : j + i * input_wires.at(i-1)));
            } printf("\n");
        }

        // Performing the circuit (ON THE LOOP)
        while (std::getline(circuit_file, line)) {
            std::stringstream gate_info (line);

            int number_input_wires;
            int number_output_wires;

            gate_info >> number_input_wires >> number_output_wires;

            // ATM ONLY CONSIDERING 1 OUTPUT WIRE ALWAYS
            if (number_input_wires == 1) {
                uint64_t wire_in;
                uint64_t wire_out;
                std::string gate;
                gate_info >> wire_in >> wire_out >> gate;
                uint8_t value_wire_in = wires_values.at(wire_in);
                if (gate == "INV") {
                    wires_values[wire_out] = !value_wire_in;
                }
                printf("   - %s: a(%ld)=%d ; o(%ld)=%d\n", gate.c_str(), wire_in, wires_values.at(wire_in), wire_out, wires_values.at(wire_out));
            } else if (number_input_wires == 2) {
                uint64_t wire_in1;
                uint64_t wire_in2;
                uint64_t wire_out;
                std::string gate;
                gate_info >> wire_in1 >> wire_in2 >> wire_out >> gate;
                uint8_t value_wire_in1 = wires_values.at(wire_in1);
                uint8_t value_wire_in2 = wires_values.at(wire_in2);
                if (gate == "AND") {
                    wires_values[wire_out] = value_wire_in1 & value_wire_in2;
                } else if (gate == "OR") {
                    wires_values[wire_out] = value_wire_in1 | value_wire_in2;
                } else if (gate == "XOR") {
                    wires_values[wire_out] = value_wire_in1 ^ value_wire_in2;
                }
                printf("   - %s: a(%ld)=%d ; b(%ld)=%d ; o(%ld)=%d\n", gate.c_str(), wire_in1, wires_values.at(wire_in1), wire_in2, wires_values.at(wire_in2), wire_out, wires_values.at(wire_out));
            }
        }

        printf("> Output wires values : ");
        for (int j = outputs_wires.at(0); j > 0 ; j--) {
            printf("%d ", wires_values.at( number_wires - j ) );
        } printf("\n");

        /*
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