#pragma once

#define VERTEX_BUFFER

#ifdef VERTEX_BUFFER

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

        /*! @brief Uploads vertex data.
        *
        *  This is a wrapper function for setting vertex buffer data.
        *  Layout needs to be set first
        *
        *  @param[in] data Pointer to data.
        *  @param[in] size Size of data (RAW BYTES)
        */
        void UploadData(float *data, size_t size);
        inline VertexBufferLayout &GetLayout() { return m_layout; }

    public: //?overloads
        template <size_t _size>
        void UploadData(std::array<float, _size> &array)
        {
            UploadData(array.data(), array.size() * sizeof(float));
        }

    private:
        uint32_t m_glId;
        uint32_t m_vertexArrayGlId;
        VertexBufferLayout m_layout;
    };

} // namespace ant

#endif