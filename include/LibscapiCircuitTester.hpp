#include <CircuitTester.hpp>

namespace gabe
{
    namespace circuits
    {
        namespace test
        {
            class LibscapiTester : public TesterAbs
            {
            protected:
                LibscapiTester();

                virtual void _read_header() override;
                virtual void _print_results() override;
            
            public:
                LibscapiTester(const std::string& circuit);

                virtual ~LibscapiTester();
            };
        }
    }
}
