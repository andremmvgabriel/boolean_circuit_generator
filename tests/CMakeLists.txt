add_executable( Generate_BristolCircuit
    generate_bristol_circuit.cpp
)
target_link_libraries( Generate_BristolCircuit
    BooleanCircuits
)

add_executable( Test_BristolCircuit
    test_bristol_circuit.cpp
)
target_link_libraries( Test_BristolCircuit
    BooleanCircuits
)

add_executable( Test_LibscapiCircuit
    test_libscapi_circuit.cpp
)
target_link_libraries( Test_LibscapiCircuit
    BooleanCircuits
)