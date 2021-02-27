/*
==========================================
Copyright (c) 2020-2021 Ostrich Labs

OpenGL 4.0 interface
Debug methods
==========================================
*/

#include "gl4_renderer.h"
#include "../game/ost_version.h"

ostrich::ConsolePrinter ostrich::GL4Renderer::ms_DebugPrinter;

/////////////////////////////////////////////////
/////////////////////////////////////////////////
void ostrich::GL4Renderer::InitDebugExtension(ostrich::GL4Extensions &ext, ostrich::ConsolePrinter consoleprinter) {
    ext.glDebugMessageCallback(&ostrich::GL4Renderer::DebugMessageCallback, nullptr);
    ms_DebugPrinter = consoleprinter;

    ext.glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE); // enable everything for now
    ::glEnable(GL_DEBUG_OUTPUT);
    ::glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

    ms_DebugPrinter.WriteMessage(u8"OpenGL: Installed debug callback");
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
void ostrich::GL4Renderer::DebugMessageCallback(GLenum source, GLenum type, GLuint id,
    GLenum severity, GLsizei length, const GLchar *message, const void *userParam) {
    OST_UNUSED_PARAMETER(userParam);

    if (type == GL_DEBUG_TYPE_MARKER) {
        ms_DebugPrinter.DebugMessage(u8"OpenGL Debug Message: Marker >%<", { (const char *)(message) });
    }
    else if (type == GL_DEBUG_TYPE_PUSH_GROUP) {
        ms_DebugPrinter.DebugMessage(u8"OpenGL Debug Message: Push Group >%<", { (const char *)(message) });
    }
    else if (type == GL_DEBUG_TYPE_POP_GROUP) {
        ms_DebugPrinter.DebugMessage(u8"OpenGL Debug Message: Pop Group >%<", { (const char *)(message) });
    }
    else {

        std::string msg(u8"OpenGL Debug Message: Source=>");

        switch (source) {
        case GL_DEBUG_SOURCE_API:
            msg.append(u8"API");
            break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            msg.append(u8"System");
            break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            msg.append(u8"Shader compiler");
            break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:
            msg.append(u8"Third party");
            break;
        case GL_DEBUG_SOURCE_APPLICATION:
            msg.append(ostrich::g_EngineName);
            break;
        case GL_DEBUG_SOURCE_OTHER:
            msg.append(u8"Other");
            break;
        default:
            msg.append(u8"Unknown");
            break;
        }

        msg.append(u8"< Type=>");

        switch (type) {
        case GL_DEBUG_TYPE_ERROR:
            msg.append(u8"Error");
            break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            msg.append(u8"Deprecated behavior");
            break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            msg.append(u8"Undefined behavior");
            break;
        case GL_DEBUG_TYPE_PERFORMANCE:
            msg.append(u8"Performance");
            break;
        case GL_DEBUG_TYPE_PORTABILITY:
            msg.append(u8"Portability");
            break;
        case GL_DEBUG_TYPE_OTHER:
            msg.append(u8"Other");
            break;
        default:
            msg.append(u8"Unknown");
            break;
        }

        msg.append(u8"< ID=>%< Severity=>");

        switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH:
            msg.append(u8"HIGH");
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            msg.append(u8"MEDIUM");
            break;
        case GL_DEBUG_SEVERITY_LOW:
            msg.append(u8"LOW");
            break;
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            msg.append(u8"NOTIFICATION");
            break;
        default:
            msg.append(u8"UNKNOWN");
            break;
        }

        msg.append(u8"<");

        if ((message != nullptr) && (length > 0)) {
            msg.append(u8" Message=>");
            msg.append(message);
            msg.append(u8"<");
        }

        ms_DebugPrinter.DebugMessage(msg, { std::to_string(id) });
    }
}