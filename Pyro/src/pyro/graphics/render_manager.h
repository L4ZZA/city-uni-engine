#pragma once

#include "pyro/core/manager.h"

#include "render_command_queue.h"

namespace pyro::graphics 
{

	class RenderManager : public pyro::Manager
	{
	public:
		typedef void(*RenderCommandFn)(void*);

		RenderManager();
		virtual ~RenderManager();

		static std::shared_ptr<RenderManager> Create();

		void Init() override;
		void Shutdown() override;

		void* SubmitToQueue(uint32 size);
		void Render();

		void SetClearColor(float r, float g, float b);
		void ClearBuffer();

		inline static std::shared_ptr<RenderManager> Get() { return RenderManager::instance; }

	private:
		static std::shared_ptr <RenderManager> instance;

		RenderCommandQueue commandQueue;
	};

}
