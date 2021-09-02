#include <CircuitTester.hpp>

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
                uint8_t _number_inputs = 0;

            protected:
                BristolTester();

                virtual void read_header() override;

            public:
                BristolTester(const std::string& circuit);

                virtual ~BristolTester();

                virtual void run(const std::string& inputs) override;
            };
        }
    }
}
