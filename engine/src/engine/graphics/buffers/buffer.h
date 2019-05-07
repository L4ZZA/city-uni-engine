#pragma once

namespace engine
{
	class buffer
	{
	public:
		buffer() = default;
		~buffer();

		void create();
		void bind() const;
		void unbind() const;

		void add_data(const void* data, unsigned int size);
		void send_to_gpu();

	private:
		unsigned int m_id{ 0 };
		/// \brief Vertex data to be uploaded
		std::vector<unsigned char> m_vertex_data{};	
	};
}
