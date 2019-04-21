#include "pyro_pch.h"
#include "render_command_queue.h"

#include "commands/render_command.hpp"

namespace pyro 
{

	RenderCommandQueue::RenderCommandQueue(uint32 size)
	{
		Init(size);
	}


	RenderCommandQueue::~RenderCommandQueue()
	{
		delete[] m_cmd_queue;
	}

	void RenderCommandQueue::Init(uint32 size)
	{
		m_cmd_queue = new byte[size];
		m_queue_ptr = m_cmd_queue; // Set current queue pointer to start of memory space
		memset(m_cmd_queue, 0, size);
	}

	void* RenderCommandQueue::Allocate(uint32 size)
	{
		// Allocate and save function pointer
		//*(RenderCommandFn*)m_cmd_queuePtr = fn;
		//m_cmd_queuePtr += sizeof(RenderCommandFn);

		// Save data block size
		(*reinterpret_cast<uint32*>(m_queue_ptr)) = size;
		m_queue_ptr += sizeof(uint32);

		// And skip ahead to end of data block for next function
		m_queue_ptr += size;

		// Increment the command counter
		++m_cmd_count;

		return m_queue_ptr - size; // Return start of memory block
	}

	void RenderCommandQueue::Execute()
	{
		PYRO_RQ_TRACE("RenderCommandQueue::Execute -- {0} commands, {1} bytes", cmdCount, (m_cmd_queuePtr - m_cmd_queue));

		byte* buffer = m_cmd_queue;

		for (uint32 i = 0; i < m_cmd_count; ++i) {
			uint32 size = *reinterpret_cast<uint32*>(buffer);
			buffer += sizeof(size);

			// Retrieve command functor
			auto* cmd = reinterpret_cast<render_command*>(buffer);// function = *(RenderCommandFn*)buffer;

			// Invoke queued command
			cmd->execute();

			// Cleanup command
			//cmd->Destroy();
			cmd->~render_command();

			// Move to next command
			buffer += size;
		}

		m_queue_ptr = m_cmd_queue; // Reset to beginning for next buffer
		m_cmd_count = 0;
	}

}
