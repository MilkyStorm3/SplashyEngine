#pragma once
#include <string>
#ifndef __glad_h_ // include after glad

typedef unsigned int GLenum;
typedef unsigned int GLuint;
typedef char GLchar;
typedef int GLsizei;

#endif

namespace ant
{
    class GlErrorHandler
    {
    public:
        ~GlErrorHandler() {}

        static void ErrorFunc(GLenum source, GLenum type, GLuint id, GLenum severity,
                                    GLsizei length, const GLchar *message, const void *userParam);
    private:
        GlErrorHandler() {}

        static std::string GetTypeStr(GLenum type);
        static std::string GetSourceStr(GLenum source);
        static std::string GetSeverityStr(GLenum severity);
    };

} // namespace ant
