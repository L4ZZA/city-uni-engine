#pragma once

namespace  pyro
{
    class entity;
    class static_shader;

    class renderer
    {
    public:
        void prepare() const;

        void render(const entity &model, static_shader &shader);
    };
}
