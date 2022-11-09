#pragma once

#include <array>
#include "VertexLayout.hpp"
#include <Core/Core.hpp>

namespace ant
{

    class VertexBuffer
    {
    public:
        static Ref<VertexBuffer> Create();

        VertexBuffer() {}
        virtual ~VertexBuffer() {}
        VertexBuffer(const VertexBuffer &) = delete;

        virtual void Bind() const = 0;
        virtual void UnBind() const = 0;

        /*! @brief Uploads vertex data.
         *
         *  This is a wrapper function for setting vertex buffer data.
         *  Layout needs to be set first
         *
         *  @param[in] data Pointer to data.
         *  @param[in] size Size of data (RAW BYTES)
         */
        virtual void UploadData(float *data, size_t size) = 0;
        virtual Ref<VertexLayout> GetLayout() = 0;
        virtual void SetLayout(Ref<VertexLayout> layout) = 0;
        virtual uint32_t GetRendererId() const = 0;
        
        // Size of internal array in bytes
        virtual size_t GetSize() const = 0;

    public:
        template <size_t _size>
        void UploadData(std::array<float, _size> &array)
        {
            UploadData(array.data(), array.size() * sizeof(float));
        }
    };

} // namespace ant
