#ifndef OPENGL_TEXTURE_H
#define OPENGL_TEXTURE_H


#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>

#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#define GL_GLEXT_PROTOTYPES
#include <SDL_opengl.h>
#endif

#include <stb_image.h>

#include <string>
#include <stdio.h>

class OpenGLTexture
{
public:
    OpenGLTexture(const std::string &texPath, GLint texSlot);
    ~OpenGLTexture() {};

    void setMinMagFilter(GLint filter);
    void setSTWrap(GLint wrap);

    void Bind() const;
    void UnBind() const;
    void Delete() const;

    GLuint m_RendererID;
    GLuint m_TexSlot;

    int m_width,
        m_height, m_numChannels;
    unsigned char *texData = NULL;
};

#endif