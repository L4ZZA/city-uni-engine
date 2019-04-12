#pragma once
#include "raw_model.h"

namespace  pyro
{
    class renderer
    {
    public:
        void prepare() const;

        void render(raw_model model) const;
    };
}
