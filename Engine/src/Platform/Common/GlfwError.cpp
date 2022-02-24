#include <Core/Layer.hpp>

namespace ant::Common
{

    void GlfwErrorCallback(int error_code, const char *description)
    {
        CORE_ERROR("GLFW ERROR: {0}", description);
    }

} // namespace ant::Common