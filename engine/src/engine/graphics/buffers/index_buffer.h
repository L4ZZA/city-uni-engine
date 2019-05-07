#pragma once

namespace engine
{
	class index_buffer
	{
	public:
		index_buffer();
		~index_buffer();

		void bind() const;
		void unbind() const;

	private:
		unsigned int m_id{ 0 };
	};
}
