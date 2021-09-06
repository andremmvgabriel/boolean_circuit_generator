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
                BristolTester();

                virtual void _read_header() override;

            public:
                BristolTester(const std::string& circuit);

                virtual ~BristolTester();
            };
        }
    }
}
