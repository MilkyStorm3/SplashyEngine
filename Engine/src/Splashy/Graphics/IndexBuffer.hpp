#pragma once
#include "Core/Core.hpp"
#include <array>

namespace ant
{

    class IndexBuffer
    {
    public:
        static Ref<IndexBuffer> Create();

        IndexBuffer() {}
        virtual ~IndexBuffer() {}
        IndexBuffer(const IndexBuffer &) = delete;

        virtual void Bind() const = 0;
        virtual void UnBind() const = 0;

        /*! @brief Uploads index data.
         *
         *  This is a wrapper function for setting element array buffer data
         *
         *  @param[in] data Pointer to data.
         *  @param[in] size Size of data (RAW BYTES)
         */
        virtual void UploadData(uint32_t *data, size_t size) = 0;

        /*!
         * @return Number of indices
         */
        virtual uint32_t GetCount() const = 0;

    public: // overloads
        template <size_t _size>
        inline void UploadData(std::array<uint32_t, _size> array)
        {
            UploadData(array.data(), array.size() * sizeof(uint32_t));
        }
    };

} // namespace ant