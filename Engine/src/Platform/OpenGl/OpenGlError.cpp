#include <GL/glew.h>
#include <string>
#include "Core/Logger.hpp"
#include "Core/Core.hpp"

namespace ant::OpenGl::Error
{

    static std::string GetTypeStr(GLenum type)
    {
        switch (type)
        {
        case GL_DEBUG_TYPE_ERROR:
            return "ERROR";
            break;

        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            return "DEPRECATED BEHAVIOR";
            break;

        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            return "UDEFINED BEHAVIOR";
            break;

        case GL_DEBUG_TYPE_PORTABILITY:
            return "PORTABILITY";
            break;

        case GL_DEBUG_TYPE_PERFORMANCE:
            return "PERFORMANCE";
            break;

        case GL_DEBUG_TYPE_OTHER:
            return "OTHER";
            break;

        case GL_DEBUG_TYPE_MARKER:
            return "MARKER";
            break;

        default:
            return "UNKNOWN";
            break;
        }
    }

    static std::string GetSourceStr(GLenum source)
    {
        switch (source)
        {
        case GL_DEBUG_SOURCE_API:
            return "API";
            break;

        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            return "WINDOW SYSTEM";
            break;

        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            return "SHADER COMPILER";
            break;

        case GL_DEBUG_SOURCE_THIRD_PARTY:
            return "THIRD PARTY";
            break;

        case GL_DEBUG_SOURCE_APPLICATION:
            return "APPLICATION";
            break;

        case GL_DEBUG_SOURCE_OTHER:
            return "OTHER";
            break;

        default:
            return "UNKNOWN";
            break;
        }
    }

    static std::string GetSeverityStr(GLenum severity)
    {
        switch (severity)
        {
        case GL_DEBUG_SEVERITY_HIGH:
            return "HIGH";
            break;

        case GL_DEBUG_SEVERITY_MEDIUM:
            return "MEDIUM";
            break;

        case GL_DEBUG_SEVERITY_LOW:
            return "LOW";
            break;

        case GL_DEBUG_SEVERITY_NOTIFICATION:
            return "NOTIFICATION";
            break;

        default:
            return "UNKNOWN";
            break;
        }
    }

    void ErrorFunc(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
    {

        if (severity == GL_DEBUG_SEVERITY_HIGH)
        {
            CORE_ERROR("[OpenGl]-> id = {0}, type = {1}, source = {2}, message = {3}", id, GetTypeStr(type), GetSourceStr(source), message);
            CORE_BREAK();
        }
        else if (severity == GL_DEBUG_SEVERITY_NOTIFICATION)
        {
            CORE_INFO("[OpenGl]-> id = {0}, type = {1}, source = {2}, message = {3}", id, GetTypeStr(type), GetSourceStr(source), message);
        }
        else
        {
            std::string severityStr = GetSeverityStr(severity);
            CORE_WARN("[OpenGl]-> id = {0}, type = {1}, severity = {2}, source = {3}, message = {4}", id, GetTypeStr(type), severityStr, GetSourceStr(source), message);
        }
    }

} // namespace ant::OpenGl::Error
