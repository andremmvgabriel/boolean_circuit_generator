add_executable( BristolCircuit
    generate_bristol_circuit.cpp
)
target_link_libraries( BristolCircuit
    BooleanCircuits
)

add_executable( BristolCircuitTest
    test_bristol_circuit.cpp
)
target_link_libraries( BristolCircuitTest
    BooleanCircuits
)

add_executable( LibscapiCircuitTest
    test_libscapi_circuit.cpp
)
target_link_libraries( LibscapiCircuitTest
    BooleanCircuits
)