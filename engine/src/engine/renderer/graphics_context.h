#pragma once 

namespace engine
{
	/// \brief 
	class graphics_context
	{
	public:
		virtual void init() = 0;
		virtual void swap_buffers() = 0;
	};
}
