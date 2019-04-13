#pragma once

namespace  pyro
{
    class textured_model;

    class renderer
    {
    public:
        void prepare() const;

        void render(const textured_model &model) const;
    };
}
