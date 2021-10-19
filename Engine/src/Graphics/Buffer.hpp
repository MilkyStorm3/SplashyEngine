#pragma once
#include <stdint.h>
#include <array>
#include "Graphics/VertexLayout.hpp"

namespace ant
{
	enum class BufferType : uint8_t
	{
		incorrect = 0,
		VertexBuffer,
		IndexBuffer
	};

	class Buffer
	{
	public:
		void Bind();
		virtual void Create() = 0;
		virtual ~Buffer();

	protected:
		Buffer(BufferType bufferType);
		void BufferData(void *array, size_t size, bool dynamicDraw = false);
		virtual void BindVertexArrayObj() = 0;

		uint32_t m_glId;
		int32_t m_glBufferType = -1;
	};

	class IndexBuffer
		: public Buffer
	{
	public:
		IndexBuffer() : Buffer(BufferType::IndexBuffer) {}
		~IndexBuffer() {}
		virtual void Create() override;
		inline size_t GetCount() const { return m_size; }
		virtual inline void BindVertexArrayObj() override {}

		template <size_t N>
		inline void UploadData(std::array<uint32_t, N> &array, bool dynamicDraw = false)
		{
			BufferData(array.data(), array.size(), dynamicDraw);
			m_size = array.size();
		}

		inline void UploadData(uint32_t *array, size_t size, bool dynamicDraw = false)
		{
			BufferData(array, size, dynamicDraw);
			m_size = size;
		}

		inline void UploadData(std::pair<uint32_t *, size_t> data, bool dynamicDraw = false)
		{
			BufferData(data.first, data.second, dynamicDraw);
			m_size = data.second;
		}

	private:
		size_t m_size;
	};

	class VertexBuffer
		: public Buffer
	{
	private:
		class GlVertexArray
		{
		public:
			GlVertexArray() {}
			~GlVertexArray();

			void Bind();
			void Create();

		private:
			uint32_t m_glId;
		};

	public:
		VertexBuffer();
		~VertexBuffer() {}
		virtual void Create() override;

		template <size_t N>
		inline void UploadData(std::array<float, N> &array, bool dynamicDraw = false)
		{
			BufferData(array.data(), array.size(), dynamicDraw);
		}

		inline void UploadData(float *array, size_t size, bool dynamicDraw = false)
		{
			BufferData(array, size, dynamicDraw);
		}
		inline VertexBufferLayout &GetLayoutRef() { return m_layout; }
		inline void PushLayoutAttribute(AttributeType attribute)
		{
			m_layout.PushAttribute(attribute);
			UpdateLayout();
		}

		void UpdateLayout(); // temporary function

	private:
		virtual inline void BindVertexArrayObj() override;

	protected:
		VertexBufferLayout m_layout;
		GlVertexArray m_glVertexArray;
	};

	class VertexArrayPrimitive
	{
	public:
		VertexArrayPrimitive() {}
		~VertexArrayPrimitive() {}

		void Bind();
		inline IndexBuffer &GetIndexBuffer() { return m_indexBuffer; }
		inline VertexBufferLayout &GetLayoutRef() { return m_vertexBuffer.GetLayoutRef(); }

		inline void UpdateLayout() { m_vertexBuffer.UpdateLayout(); }

		inline void UploadData(std::pair<float *, size_t> data, bool dynamicDraw = false) { m_vertexBuffer.UploadData(data.first, data.second, dynamicDraw); }
		inline void UploadData(float *array, size_t size, bool dynamicDraw = false) { m_vertexBuffer.UploadData(array, size, dynamicDraw); }
		template <size_t N>
		inline void UploadData(std::array<float, N> &array, bool dynamicDraw = false)
		{
			m_vertexBuffer.UploadData(array, dynamicDraw);
		}

		inline void PushLayoutAttribute(AttributeType attribute) { m_vertexBuffer.PushLayoutAttribute(attribute); }
		inline void SetLayout(const VertexBufferLayout &vbl)
		{
			m_vertexBuffer.GetLayoutRef() = vbl;
			m_vertexBuffer.UpdateLayout();
		}

	private:
		VertexBuffer m_vertexBuffer;
		IndexBuffer m_indexBuffer;
	};

} // namespace ant
