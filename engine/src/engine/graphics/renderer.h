#pragma once

namespace engine
{
    class entity;
    class static_shader;

    class renderer
    {
    public:
		void init(static_shader &shader);
        void prepare() const;
        void render(const entity &entity, static_shader &shader) const;

		inline static const float s_fov = 70.f;
		inline static const float s_near = 0.1f;
		inline static const float s_far = 1000.f;
    };
}
