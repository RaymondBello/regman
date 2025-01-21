#include "opengl_buffer.h"



/////////////////////////////////////////////////////////////////////////////
// VertexBuffer /////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

OpenGLVertexBuffer::OpenGLVertexBuffer(float *vertices, uint32_t size)
{
    glGenBuffers(1, &m_RendererID);
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

OpenGLVertexBuffer::~OpenGLVertexBuffer()
{
    glDeleteBuffers(1, &m_RendererID);
}

void OpenGLVertexBuffer::Bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}

void OpenGLVertexBuffer::Unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

/////////////////////////////////////////////////////////////////////////////
// IndexBuffer //////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t *indices, uint32_t count)
    : m_Count(count)
{
    glGenBuffers(1, &m_RendererID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
}

OpenGLIndexBuffer::~OpenGLIndexBuffer()
{
    glDeleteBuffers(1, &m_RendererID);
}

void OpenGLIndexBuffer::Bind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
}

void OpenGLIndexBuffer::Unbind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

/////////////////////////////////////////////////////////////////////////////
// Vertex Array //////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
{
    switch (type)
    {
    case ShaderDataType::Float:
        return GL_FLOAT;
    case ShaderDataType::Float2:
        return GL_FLOAT;
    case ShaderDataType::Float3:
        return GL_FLOAT;
    case ShaderDataType::Float4:
        return GL_FLOAT;
    case ShaderDataType::Mat3:
        return GL_FLOAT;
    case ShaderDataType::Mat4:
        return GL_FLOAT;
    case ShaderDataType::Int:
        return GL_INT;
    case ShaderDataType::Int2:
        return GL_INT;
    case ShaderDataType::Int3:
        return GL_INT;
    case ShaderDataType::Int4:
        return GL_INT;
    case ShaderDataType::Bool:
        return GL_BOOL;
    case ShaderDataType::None:
        return -1;
    }

    return 0;
}

OpenGLVertexArray::OpenGLVertexArray()
{
    #ifndef __EMSCRIPTEN__
    glGenVertexArrays(1, &m_RendererID);
    #endif
}

OpenGLVertexArray::~OpenGLVertexArray()
{
    #ifndef __EMSCRIPTEN__
    glDeleteVertexArrays(1, &m_RendererID);
    #endif
}

void OpenGLVertexArray::Bind()
{
    #ifndef __EMSCRIPTEN__
    glBindVertexArray(m_RendererID);
    #endif
}

void OpenGLVertexArray::Unbind()
{
    #ifndef __EMSCRIPTEN__
    glBindVertexArray(0);
    #endif
}

void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<OpenGLVertexBuffer> &vertexBuffer)
{
    assert(vertexBuffer->GetLayout().GetElements().size());

    #ifndef __EMSCRIPTEN__
    glBindVertexArray(m_RendererID);
    #endif
    vertexBuffer->Bind();

    uint32_t index = 0;
    const auto &layout = vertexBuffer->GetLayout();
    for (const auto &element : layout)
    {
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index,
                              element.GetComponentCount(),
                              ShaderDataTypeToOpenGLBaseType(element.Type),
                              element.Normalized ? GL_TRUE : GL_FALSE,
                              layout.GetStride(),
                              (const void *)(uintptr_t)element.Offset);
        index++;
    }

    m_VertexBuffers.push_back(vertexBuffer);
}

void OpenGLVertexArray::SetIndexBuffer(const std::shared_ptr<OpenGLIndexBuffer> &indexBuffer)
{
    #ifndef __EMSCRIPTEN__
    glBindVertexArray(m_RendererID);
    #endif
    indexBuffer->Bind();

    m_IndexBuffer = indexBuffer;
}