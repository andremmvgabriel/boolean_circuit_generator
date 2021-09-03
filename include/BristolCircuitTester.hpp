#include <CircuitTester.hpp>

#include <vector>

namespace gabe
{
    namespace circuits
    {
        namespace test
        {
            class BristolTester : public TesterAbs
            {
            protected:
                uint64_t _number_wires = 0;
                uint32_t _number_input_parties = 0;
                uint32_t _number_output_parties = 0;

                std::vector<uint32_t> _number_wires_input_parties;
                std::vector<uint32_t> _number_wires_output_parties;

            protected:
                BristolTester();

                virtual void read_header() override;
                virtual void read_inputs(const std::string& inputs) override;
                virtual void execute_circuit() override;

            public:
                BristolTester(const std::string& circuit);

                virtual ~BristolTester();
            };
        }
    }
}
