#pragma once

namespace engine
{
	class model;
	class shader;
	class mesh;
	class entity;
    class static_shader;

    class renderer
    {
    public:
		void init(static_shader &shader);
		/// \brief 
        void prepare() const;
		/// \brief 
        void render(const entity &entity, static_shader &shader) const;
		/// \brief 
        void render_3d(const model &model, shader &shader) const;
		/// \brief 
        void render_3d(const mesh &mesh, shader &shader) const;

		inline static const float s_fov = 70.f;
		inline static const float s_near = 0.1f;
		inline static const float s_far = 1000.f;
    };
}
