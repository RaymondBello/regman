#ifndef OPENGL_BUFFER_H
#define OPENGL_BUFFER_H

#include "utils.h"

#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>

#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#define GL_GLEXT_PROTOTYPES
#include <SDL_opengl.h>
#endif

#include <memory>

class OpenGLVertexBuffer
{
public:
    OpenGLVertexBuffer(float *vertices, uint32_t size);
    ~OpenGLVertexBuffer();

    void Bind();
    void Unbind();

    const BufferLayout &GetLayout() const { return m_Layout; }
    void SetLayout(const BufferLayout &layout) { m_Layout = layout; }

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

    uint32_t GetCount() const { return m_Count; }

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