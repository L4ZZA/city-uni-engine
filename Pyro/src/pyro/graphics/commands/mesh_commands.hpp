#pragma once

#include "render_command.hpp"
#include "pyro/graphics/render_manager.h"

#include "pyro/defs.h"
#include "pyro/logger.h"

#include "glad/glad.h"


namespace pyro
{
	// --------------------------------------------------------------------------
	// -- RenderMesh                                                           --
	// --------------------------------------------------------------------------
	class RenderMesh : render_command
	{
	public:
		RenderMesh(uint32 vao, uint32 num)
			:vao(vao), num(num)
		{}

		void execute() override 
		{
			glBindVertexArray(vao);
			glDrawElements(GL_TRIANGLES, num, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);

			PYRO_RQ_TRACE("[RenderQueue] -- RenderMesh (vao {0}): {1} faces", vao, num);
		}

	private:
		uint32 vao;
		uint32 num;

	public: //STATIC CREATOR
		static void dispatch(uint32 vao, uint32 num) 
		{
			auto mem = render_manager::get()->send_command(sizeof(RenderMesh));
			new (mem) RenderMesh(vao, num);
		}
	};

}
