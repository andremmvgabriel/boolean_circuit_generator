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

                virtual void read_header() override;
            
            public:
                LibscapiTester(const std::string& circuit);

                virtual ~LibscapiTester();
            };
        }
    }
}
