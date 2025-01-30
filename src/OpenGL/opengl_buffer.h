#ifndef OPENGL_BUFFER_H
#define OPENGL_BUFFER_H

#include "utils.h"

#include <memory>
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>

#ifdef __EMSCRIPTEN__
#include <SDL_opengles2.h>
#endif

#define GL_GLEXT_PROTOTYPES
#include <SDL_opengl.h>


class OpenGLVertexBuffer
{
public:
    OpenGLVertexBuffer(float *vertices, uint32_t size);
    ~OpenGLVertexBuffer();

    void Bind();
    void Unbind();

    BufferLayout &GetLayout()  { return m_Layout; }
    void SetLayout( BufferLayout &layout) { m_Layout = layout; }

private:
    uint32_t m_RendererID;
    BufferLayout m_Layout;
};

class OpenGLIndexBuffer
{
public:
    OpenGLIndexBuffer(uint32_t *indices, uint32_t count);
    ~OpenGLIndexBuffer();

    void Bind();
    void Unbind();

    uint32_t GetCount() { return m_Count; }

private:
    uint32_t m_RendererID;
    uint32_t m_Count;
};

class OpenGLVertexArray
{
public:
    OpenGLVertexArray();
    ~OpenGLVertexArray();

    void Bind();
    void Unbind();

    void AddVertexBuffer(const std::shared_ptr<OpenGLVertexBuffer> &vertexBuffer);
    void SetIndexBuffer(const std::shared_ptr<OpenGLIndexBuffer> &indexBuffer);

    const std::vector<std::shared_ptr<OpenGLVertexBuffer>> &GetVertexBuffers() const { return m_VertexBuffers; }
    const std::shared_ptr<OpenGLIndexBuffer> &GetIndexBuffer() const { return m_IndexBuffer; }

private:
    uint32_t m_RendererID;
    std::vector<std::shared_ptr<OpenGLVertexBuffer>> m_VertexBuffers;
    std::shared_ptr<OpenGLIndexBuffer> m_IndexBuffer;
};

#endif