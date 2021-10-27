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
        void UploadData(uint32_t *data, size_t size);

        inline uint32_t GetIndiciesCount() const { return m_count; }

    public: //? override
        template <size_t _size>
        inline void UploadData(std::array<uint32_t, _size> array)
        {
            UploadData(array.data(), array.size());
        }

    private:
        uint32_t m_count;
        uint32_t m_glId;
    };

} // namespace ant
#endif