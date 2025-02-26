#include "opengl_texture.h"

OpenGLTexture::OpenGLTexture(const std::string &texPath, GLint texSlot)
{
    m_TexSlot = texSlot;

    stbi_set_flip_vertically_on_load(true);
    texData = stbi_load(texPath.c_str(), &m_width, &m_height, &m_numChannels, 0);
    if (!texData)
    {
        printf("Error: Failed to load texture %s\n", texPath.c_str());
        return;
    } else {
        printf("Info: Loaded texture %s\n", texPath.c_str());
    }

    glGenTextures(1, &m_RendererID);
    glActiveTexture(GL_TEXTURE0 + m_TexSlot);

    Bind();

    setMinMagFilter(GL_NEAREST);
    setSTWrap(GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(texData);

    UnBind();
}

void OpenGLTexture::setMinMagFilter(GLint filter)
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
}

void OpenGLTexture::setSTWrap(GLint wrap)
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
}


void OpenGLTexture::Bind() const
{
    glBindTexture(GL_TEXTURE_2D, m_RendererID);
}
void OpenGLTexture::UnBind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}
void OpenGLTexture::Delete() const
{
    glDeleteTextures(1, &m_RendererID);
}