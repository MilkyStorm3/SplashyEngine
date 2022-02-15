#include "Pch.h"
#include "debug/GlErrorHandler.hpp"

#include <Gl.h>

namespace ant
{
    void GlErrorHandler::ErrorFunc(GLenum source, GLenum type, GLuint id, GLenum severity,
                                                 GLsizei length, const GLchar *message, const void *userParam)
    {
        // static std::string formatStr = "OpenGl error: id = {0}, type = {1}, severity = {2}, source = {3}, message = {4}";
        std::string severityStr = GetSeverityStr(severity);

        if (severityStr == "HIGH")
        {
            CORE_ERROR("OpenGl error: id = {0}, type = {1}, severity = {2}, source = {3}, message = {4}", id, GetTypeStr(type), severityStr, GetSourceStr(source), message);
            CORE_BREAK();
        }
        else if (severityStr == "NOTIFICATION")
        {
            CORE_INFO("OpenGl error: id = {0}, type = {1}, severity = {2}, source = {3}, message = {4}", id, GetTypeStr(type), severityStr, GetSourceStr(source), message);
        }
        else
        {
            CORE_WARN("OpenGl error: id = {0}, type = {1}, severity = {2}, source = {3}, message = {4}", id, GetTypeStr(type), severityStr, GetSourceStr(source), message);
        }
    }

    std::string GlErrorHandler::GetTypeStr(GLenum type)
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

    std::string GlErrorHandler::GetSourceStr(GLenum source)
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
            return "UNKNOWN";
            break;

        default:
            return "UNKNOWN";
            break;
        }
    }

    std::string GlErrorHandler::GetSeverityStr(GLenum severity)
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

} // namespace ant