#pragma once
#include <stdint.h>
#include <stddef.h>
#include <array>
#include "VertexLayout.hpp"

// #include <GlTypes.h>


namespace ant
{

    class VertexBuffer
    {
    public:
        VertexBuffer();
        ~VertexBuffer();

        void Bind();
        void UnBind();
        void UploadData(float *data, size_t size);
        inline VertexBufferLayout &GetLayout() { return m_layout; }

    public: //?overloads
        template <size_t _size>
        void UploadData(std::array<float, _size> &array)
        {
            UploadData(array.data(), array.size());
        }

    private:
        uint32_t m_glId;
        uint32_t m_vertexArrayGlId;
        VertexBufferLayout m_layout;
    };

} // namespace ant
