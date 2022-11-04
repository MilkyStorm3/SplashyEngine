#include "Utils.hpp"

namespace ant
{

    void Utils::LoadBinary(const std::filesystem::path &path, std::vector<uint32_t> *destination)
    {
        size_t dataSize = std::filesystem::file_size(path);

        std::ifstream inputCache(path, std::ios::binary);
        if (inputCache.is_open())
        {
            destination->resize(dataSize / sizeof(uint32_t));
            inputCache.read((char *)destination->data(), dataSize);
            inputCache.close();
        }
    }

    void Utils::SaveBinary(const std::filesystem::path &path, std::vector<uint32_t> *data)
    {
        std::fstream outputCache(path, std::ios::binary | std::ios::out);

        if (outputCache.is_open())
        {
            outputCache.write((char *)data->data(), data->size() * sizeof(uint32_t));
            outputCache.flush();
            outputCache.close();
        }
    }

} // namespace ant