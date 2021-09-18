#include <LibscapiCircuitGenerator.hpp>
#include <LibscapiCircuitTester.hpp>

typedef gabe::circuits::generator::LibscapiGenerator LibGen;
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

static const uint8_t sbox[256] = {
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

static const uint8_t rcon[255] = {
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

    uVar control(1);

    for (int i = 0; i < 256; i++) {
        uVar cur_index = circuit.create_constant( 8, (uint8_t)i );
        uVar cur_sbox_value = circuit.create_constant( 8, sbox[i] );

        circuit.equal( index, cur_index, control );
        circuit.multiplexer( control, output, cur_sbox_value, output );
    }

    return output;
}

void key_expansion(LibGen& circuit, std::vector<uVar>& key, std::vector<uVar>& round_key) {

    // First round key is the key itself
    for (int i = 0; i < Nk; i++) {
        round_key[(i * 4) + 0] = key[(i * 4) + 0];
        round_key[(i * 4) + 1] = key[(i * 4) + 1];
        round_key[(i * 4) + 2] = key[(i * 4) + 2];
        round_key[(i * 4) + 3] = key[(i * 4) + 3];
    }

    // Used for column/row operations
    std::vector<uVar> tempa( 4, uVar(8) );

    // The other round keys are found from the previous keys
    for (int i = 4; i < (Nb * (Nr + 1)); i++) {
        for (int j = 0; j < 4; j++) {
            tempa[j] = round_key[ (i - 1) * 4 + j ];
        }

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

            circuit.XOR( tempa[0], circuit.create_constant(8, rcon[i/Nk]), tempa[0] );
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

void encrypt(LibGen& circuit, std::vector<uVar>& key, std::vector<uVar>& plain_text, std::vector<uVar>& cipher_text) {
    //add_round_key();
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

    circuit_generator.start();

    key_expansion(circuit_generator, key, round_key);
    encrypt(circuit_generator, round_key, plain_text, cipher_text);

    circuit_generator.conclude();

    // Adds the outputs
    for (auto & byte : cipher_text) circuit_generator.add_output(byte);
}

typedef uint8_t state_t[4][4];

void test2(state_t test) {}

void test() {
    /* uint8_t values[16] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };

    test2(values);
    
    https://github.com/haslab/CircGen/blob/master/cdg/test/bcircuits/crypto/aes-tab-sbox.c
    */
}

int main(int argc, char* argv[]) {
    if (argc <= 1) generate();
    else test();

    return 0;
}