#ifndef OPENGL_SHADER_H
#define OPENGL_SHADER_H


#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>

#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#define GL_GLEXT_PROTOTYPES
#include <SDL_opengl.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <vector>

class OpenGLShader
{
public:
    OpenGLShader(const std::string &vertexSrc, const std::string &fragmentSrc);
    ~OpenGLShader();

    void Bind() const;
    void Unbind() const;

    void UploadUniformInt(const std::string &name, int value);

    void UploadUniformFloat(const std::string &name, float value);
    void UploadUniformFloat2(const std::string &name, const glm::vec2 &value);
    void UploadUniformFloat3(const std::string &name, const glm::vec3 &value);
    void UploadUniformFloat4(const std::string &name, const glm::vec4 &value);

    void UploadUniformMat3(const std::string &name, const glm::mat3 &matrix);
    void UploadUniformMat4(const std::string &name, const glm::mat4 &matrix);

    uint32_t m_RendererID;

private:
};

#endif