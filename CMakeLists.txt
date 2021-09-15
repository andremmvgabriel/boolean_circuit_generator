cmake_minimum_required(VERSION 3.10)

# Set the project name (and version?)
project(BooleanCircuitGenerator)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 14)

# Changes the binary, static lib, and libraries default paths of the CMake build, respectively
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/bin)
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/lib)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/lib)

INCLUDE_DIRECTORIES ( include )

set( src_generator_lib
    src/CircuitGenerator.cpp
    src/CircuitTester.cpp
    src/BristolCircuitGenerator.cpp
    src/BristolCircuitTester.cpp
    src/LibscapiCircuitGenerator.cpp
    src/LibscapiCircuitTester.cpp
    src/Variable.cpp
    src/UnsignedVariable.cpp
    src/SignedVariable.cpp
)
add_library( BooleanCircuits STATIC ${src_generator_lib} )

add_subdirectory( tests )
