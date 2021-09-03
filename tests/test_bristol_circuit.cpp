#include <BristolCircuitTester.hpp>

int main(int argc, char* argv[]) {
    gabe::circuits::test::BristolTester bristol_tester( argv[1] );

    bristol_tester.run( argv[2] );

    return 0;
}