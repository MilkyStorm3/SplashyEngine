#pragma once

#define INDEX_BUFFER

#ifdef INDEX_BUFFER

#include <stdint.h>
#include <stddef.h>
#include <array>

namespace ant
{

    class IndexBuffer
    {
    public:
        IndexBuffer();
        ~IndexBuffer();

        void Bind();
        void UnBind();

        /*! @brief Uploads index data.
        *
        *  This is a wrapper function for setting element array buffer data
        *
        *  @param[in] data Pointer to data.
        *  @param[in] size Size of data (RAW BYTES)
        */
        void UploadData(uint32_t *data, size_t size);

        /*! 
        * @return Number of indices    
        */
        inline uint32_t GetCount() const { return m_count; }

    public: //? override
        template <size_t _size>
        inline void UploadData(std::array<uint32_t, _size> array)
        {
            UploadData(array.data(), array.size() * sizeof(uint32_t));
        }

    private:
        uint32_t m_count;
        uint32_t m_glId;
    };

} // namespace ant
#endif