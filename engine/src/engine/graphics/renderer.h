#pragma once

namespace engine
{
	class camera;
	class model;
	class shader;
	class mesh;
	
    class renderer
    {
    public:
		void init(camera &camera, shader &shader);
		/// \brief 
        void prepare() const;
		/// \brief 
        void render_3d(const model &model, shader &shader) const;
		/// \brief 
        void render_3d(const mesh &mesh, shader &shader) const;

		inline static const float s_fov = 70.f;
		inline static const float s_near = 0.1f;
		inline static const float s_far = 1000.f;
    };
}
