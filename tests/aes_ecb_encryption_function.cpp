#include <LibscapiCircuitGenerator.hpp>
#include <LibscapiCircuitTester.hpp>

typedef gabe::circuits::generator::LibscapiGenerator LibGen;
typedef gabe::circuits::test::LibscapiTester LibTest;
typedef gabe::circuits::UnsignedVariable uVar;

#include <stdint.h>

// The number of columns comprising a state in AES. This is a constant in AES.
#define Nb 4
// The number of 32 bit words in a key.
#define Nk 4
// Key length in bytes [128 bit]
#define KEYLEN 16
// The number of rounds in AES Cipher.
#define Nr 10

const uint8_t sbox[256] = {
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
};

const uint8_t rcon[255] = {
    0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 
    0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 
    0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 
    0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 
    0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 
    0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 
    0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 
    0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 
    0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 
    0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 
    0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 
    0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 
    0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 
    0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 
    0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 
    0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb 
};

//

uVar get_sbox_value(LibGen& circuit, uVar& index) {
    uVar output = circuit.create_constant( 8, 0x00 );

    for (int i = 0; i < 256; i++) {
        uVar control(1);
        uVar cur_index = circuit.create_constant( 8, (uint8_t)i );
        uVar cur_sbox_value = circuit.create_constant( 8, sbox[i] );

        circuit.equal( index, cur_index, control );
        circuit.multiplexer( control, output, cur_sbox_value, output );
    }

    return output;
}

void array_to_matrix(std::vector<uVar>& array, std::vector<std::vector<uVar>>& matrix) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            matrix[i][j] = array[i * 4 + j];
        }
    }
}

void matrix_to_array(std::vector<std::vector<uVar>>& matrix, std::vector<uVar>& array) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            array[i * 4 + j] = matrix[i][j];
        }
    }
}

void add_round_key(LibGen& circuit, uint8_t round, std::vector<uVar>& round_key, std::vector<std::vector<uVar>>& state) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            circuit.XOR( state[i][j], round_key[round * Nb * 4 + i * Nb + j], state[i][j] );
        }
    }
}

void sub_bytes(LibGen& circuit, std::vector<std::vector<uVar>>& state) {
    for(uint8_t i = 0; i < 4; ++i) {
        for(uint8_t j = 0; j < 4; ++j) {
            state[j][i] = get_sbox_value( circuit, state[j][i] );
        }
    }
}

void shift_rows(std::vector<std::vector<uVar>>& state)
{
    uVar temp;

    // Rotate first row 1 columns to left  
    temp        = state[0][1];
    state[0][1] = state[1][1];
    state[1][1] = state[2][1];
    state[2][1] = state[3][1];
    state[3][1] = temp;

    // Rotate second row 2 columns to left  
    temp        = state[0][2];
    state[0][2] = state[2][2];
    state[2][2] = temp;

    temp        = state[1][2];
    state[1][2] = state[3][2];
    state[3][2] = temp;

    // Rotate third row 3 columns to left
    temp        = state[0][3];
    state[0][3] = state[3][3];
    state[3][3] = state[2][3];
    state[2][3] = state[1][3];
    state[1][3] = temp;
}

uVar x_time(LibGen& circuit, uVar& value) {
    uVar value1 = circuit.create_constant(8, 0x00);
    for (int i = 1; i < 8; i++) { value1.wires[i] = value.wires[i-1]; }

    uVar hex_0x1b = circuit.create_constant( 8, 0x1b );

    uVar value2 = circuit.create_constant( 8, 0x00 );
    value2.wires[0] = value.wires[7];

    uVar mult_16(16);
    circuit.multiplication( value2, hex_0x1b, mult_16 );

    uVar mult_8(8);
    for (int i = 0; i < 8; i++) { mult_8.wires[i] = mult_16.wires[i]; }

    circuit.XOR( value1, mult_8, value1 );

    return value1;
}

void mix_columns(LibGen& circuit, std::vector<std::vector<uVar>>& state) {
    uVar tmp(8), tm(8), t(8);

    for(uint8_t i = 0; i < 4; ++i) {  
        t   = state[i][0];

        //tmp = state[i][0] ^ state[i][1] ^ state[i][2] ^ state[i][3] ;
        circuit.XOR( state[i][0], state[i][1], tmp );
        circuit.XOR( tmp, state[i][2], tmp );
        circuit.XOR( tmp, state[i][3], tmp );

        //tm  = state[i][0] ^ state[i][1] ; tm = xtime(tm);  state[i][0] ^= tm ^ tmp ;
        circuit.XOR( state[i][0], state[i][1], tm );
        tm = x_time(circuit, tm);
        circuit.XOR( tm, tmp, state[i][0] );

        //tm  = state[i][1] ^ state[i][2] ; tm = xtime(tm);  state[i][1] ^= tm ^ tmp ;
        circuit.XOR( state[i][1], state[i][2], tm );
        tm = x_time(circuit, tm);
        circuit.XOR( tm, tmp, state[i][1] );

        //tm  = state[i][2] ^ state[i][3] ; tm = xtime(tm);  state[i][2] ^= tm ^ tmp ;
        circuit.XOR( state[i][2], state[i][3], tm );
        tm = x_time(circuit, tm);
        circuit.XOR( tm, tmp, state[i][2] );

        //tm  = state[i][3] ^ t ;           tm = xtime(tm);  state[i][3] ^= tm ^ tmp ;
        circuit.XOR( state[i][3], t, tm );
        tm = x_time(circuit, tm);
        circuit.XOR( tm, tmp, state[i][3] );
    }
}

void key_expansion(LibGen& circuit, std::vector<uVar>& key, std::vector<uVar>& round_key) {

    // First round key is the key itself
    for (int i = 0; i < Nk; ++i) {
        round_key[(i * 4) + 0] = key[(i * 4) + 0];
        round_key[(i * 4) + 1] = key[(i * 4) + 1];
        round_key[(i * 4) + 2] = key[(i * 4) + 2];
        round_key[(i * 4) + 3] = key[(i * 4) + 3];
    }

    // Used for column/row operations
    std::vector<uVar> tempa( 4, uVar(8) );

    // The other round keys are found from the previous keys
    for (int i = 4; i < (Nb * (Nr + 1)); ++i) {
        for (int j = 0; j < 4; ++j) { tempa[j] = round_key[ (i - 1) * 4 + j ]; }

        if (i % Nk == 0) {
            // Rotates word
            uVar k = tempa[0];
            tempa[0] = tempa[1];
            tempa[1] = tempa[2];
            tempa[2] = tempa[3];
            tempa[3] = k;

            // Sub word
            tempa[0] = get_sbox_value(circuit, tempa[0]);
            tempa[1] = get_sbox_value(circuit, tempa[1]);
            tempa[2] = get_sbox_value(circuit, tempa[2]);
            tempa[3] = get_sbox_value(circuit, tempa[3]);

            uVar rcon_value = circuit.create_constant(8, rcon[i/Nk]);

            circuit.XOR( tempa[0], rcon_value, tempa[0] );
        }
        else if (Nk > 6 && i % Nk == 4) {
            // Sub word
            tempa[0] = get_sbox_value(circuit, tempa[0]);
            tempa[1] = get_sbox_value(circuit, tempa[1]);
            tempa[2] = get_sbox_value(circuit, tempa[2]);
            tempa[3] = get_sbox_value(circuit, tempa[3]);
        }

        circuit.XOR( round_key[(i - Nk) * 4 + 0] , tempa[0], round_key[i * 4 + 0] );
        circuit.XOR( round_key[(i - Nk) * 4 + 1] , tempa[1], round_key[i * 4 + 1] );
        circuit.XOR( round_key[(i - Nk) * 4 + 2] , tempa[2], round_key[i * 4 + 2] );
        circuit.XOR( round_key[(i - Nk) * 4 + 3] , tempa[3], round_key[i * 4 + 3] );
    }
}

void encrypt(LibGen& circuit, std::vector<uVar>& round_key, std::vector<std::vector<uVar>>& state) {
    add_round_key(circuit, 0, round_key, state);

    for (uint8_t round = 1; round < Nr; ++round) {
        sub_bytes(circuit, state);
        shift_rows(state);
        mix_columns(circuit, state);
        add_round_key(circuit, round, round_key, state);
    }

    sub_bytes(circuit, state);
    shift_rows(state);
    add_round_key(circuit, Nr, round_key, state);
}

/******************/
/* Main Functions */
/******************/

void generate() {
    // Circuit generator
    LibGen circuit_generator(
        "LibscapiCircuit_aes_ecb_encryption.txt",
        { 128, 128 },
        { 128 }
    );

    // Input and Output variables
    std::vector<uVar> key( KEYLEN, uVar(8) );
    std::vector<uVar> plain_text( KEYLEN, uVar(8) );
    std::vector<uVar> cipher_text( KEYLEN, uVar(8) );

    // Other variables
    std::vector<uVar> round_key( KEYLEN*(Nr+1), uVar(8) );

    // Adds the inputs
    for (auto & byte : key) circuit_generator.add_input(byte);
    for (auto & byte : plain_text) circuit_generator.add_input(byte);

    // Temporary Matrices
    std::vector<std::vector<uVar>> plain_text_matrix(4, std::vector<uVar>(4));
    std::vector<std::vector<uVar>> cipher_text_matrix(4, std::vector<uVar>(4));
    array_to_matrix(plain_text, plain_text_matrix);

    circuit_generator.start();

    key_expansion(circuit_generator, key, round_key);
    //encrypt(circuit_generator, round_key, plain_text, cipher_text);
    encrypt(circuit_generator, round_key, plain_text_matrix);

    matrix_to_array(plain_text_matrix, plain_text);

    for (auto & byte : plain_text) circuit_generator.INV(byte, byte);
    for (auto & byte : plain_text) circuit_generator.INV(byte, byte);

    circuit_generator.conclude();

    // Adds the outputs
    for (auto & byte : cipher_text) circuit_generator.add_output(byte);
}

void test() {
    /*
    plain-text:
        6bc1bee22e409f96e93d7e117393172a
        ae2d8a571e03ac9c9eb76fac45af8e51
        30c81c46a35ce411e5fbc1191a0a52ef
        f69f2445df4f9b17ad2b417be66c3710

    key:
        2b7e151628aed2a6abf7158809cf4f3c

    resulting cipher
        3ad77bb40d7a3660a89ecaf32466ef97 
        f5d3d58503b9699de785895a96fdbaaf 
        43b1cd7f598ece23881b00e3ed030688 
        7b0c785e27e8ad3f8223207104725dd4 
    */

    std::vector<uint8_t> key        = { 0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c };
    std::vector<uint8_t> plain_text = { 0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96, 0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a };

    std::vector<uint8_t> inputs;
    for (auto & val : key) {
        for (int i = 0; i < 8; i++) {
            inputs.push_back( (val >> i) & 0x01 );
        }
    }
    for (auto & val : plain_text) {
        for (int i = 0; i < 8; i++) {
            inputs.push_back( (val >> i) & 0x01 );
        }
    }

    LibTest circuit_tester( "LibscapiCircuit_aes_ecb_encryption.txt" );

    circuit_tester.run( inputs );
}

void test2() {
    std::string values = "00011100100011111101110101101110101110001110011011011111011110110110100010001100111010001111000010000001011100110000100111010011";
    std::vector<uint8_t> cipher(16, 0x00);

    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 8; j++) {
            std::string value_str = " ";
            value_str[0] = values[i * 8 + j];
            int value_i = std::stoi(value_str);
            uint8_t value = value_i;
            cipher[i] |= ( value << j );
        }
        printf("%d ", cipher[i]);
    } printf("\n");
}

int main(int argc, char* argv[]) {
    if (argc > 2) { test2(); return 0; }
    if (argc <= 1) generate();
    else test();

    return 0;
}