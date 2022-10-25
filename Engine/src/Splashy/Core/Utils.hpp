#pragma once
#include <Core/Core.hpp>

namespace ant
{

    class Utils
    {
    public:
        Utils() = delete;
        ~Utils() = delete;

        static void LoadBinary(const std::filesystem::path &path, std::vector<uint32_t> *destination);
        static void SaveBinary(const std::filesystem::path &path, std::vector<uint32_t> *data);
    };

} // namespace ant