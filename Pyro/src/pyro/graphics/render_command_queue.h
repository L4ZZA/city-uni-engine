#pragma once

namespace pyro
{

	class RenderCommandQueue
	{
	public:
		RenderCommandQueue() = default;
		RenderCommandQueue(uint32 size);
		~RenderCommandQueue();

		void Init(uint32 size);
		void* Allocate(uint32 size);
		void Execute();

	private:
		byte*	m_cmd_queue{ nullptr };
		byte*	m_queue_ptr{ nullptr };
		uint32	m_cmd_count{ 0 };

	};

}
