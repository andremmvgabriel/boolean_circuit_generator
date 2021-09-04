#pragma once

#include <stdint.h>

namespace gabe
{
    namespace circuits
    {
        class Wire
        {
        public:
            uint64_t label = 0x00;
            uint8_t value = 0x00;
        };
    }
}
