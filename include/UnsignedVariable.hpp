#pragma once

#include <Variable.hpp>

namespace gabe
{
    namespace circuits
    {
        class UnsignedVariable : public Variable
        {
        public:
            UnsignedVariable();
            UnsignedVariable(uint8_t n_bits);

            virtual ~UnsignedVariable();
        };
    }
}
