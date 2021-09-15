#pragma once

#include <Variable.hpp>

namespace gabe
{
    namespace circuits
    {
        class SignedVariable : public Variable
        {
        public:
            SignedVariable();
            SignedVariable(uint8_t n_bits);

            virtual ~SignedVariable();
        };
    }
}
