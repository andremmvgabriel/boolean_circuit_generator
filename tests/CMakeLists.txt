add_executable( BristolCircuit
    generate_bristol_circuit.cpp
)
target_link_libraries( BristolCircuit
    BooleanCircuitGenerator
)