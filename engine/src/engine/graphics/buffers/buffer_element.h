#pragma once

namespace engine
{
	class buffer_element
	{
	public:
		buffer_element();
		~buffer_element();

		void bind() const;
		void unbind() const;

	private:
		unsigned int m_id{ 0 };
	};
}
