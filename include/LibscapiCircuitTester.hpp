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
            
            public:
                LibscapiTester(const std::string& circuit);
            };
        }
    }
}
