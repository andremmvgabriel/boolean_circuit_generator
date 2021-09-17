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



add_executable( Generate_LibscapiCircuit
    generate_libscapi_circuit.cpp
)
target_link_libraries( Generate_LibscapiCircuit
    BooleanCircuits
)



add_executable( Test_LibscapiCircuit
    test_libscapi_circuit.cpp
)
target_link_libraries( Test_LibscapiCircuit
    BooleanCircuits
)






####

add_executable( Average_Circuit
    average_function.cpp
)
target_link_libraries( Average_Circuit
    BooleanCircuits
)


