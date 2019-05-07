#pragma once

namespace engine
{
	/// \brief Wrapper around a element buffer object. 
	/// Used to improve rendering efficiency by specifying 
	/// the order by which the gpu will use the vertex data 
	/// specified in the buffer.
	class index_buffer
	{
	public:
		index_buffer() = default;
		index_buffer(const uint32* data, uint32 count);
		~index_buffer();

		void create();
		void bind() const;
		void unbind() const;
		/// \brief Adds index data to the index buffer
		/// @param data Buffer of data to add (unsigned int only for now).
		/// @param count Number of elements added.
		void add_data(const uint32* data, uint32 count);
		void send_to_gpu();

	private:
		uint32 m_id{ 0 };
		/// \brief Number of indices this object has.
		uint32 m_count{0};
		/// \brief Index data to be sent to the gpu.
		std::vector<unsigned char>  m_data{};
	};
}
