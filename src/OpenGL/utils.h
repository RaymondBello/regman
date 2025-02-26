#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <cstdint>

#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>


enum class ObjectType
{
    NONE = 0,
    // Camera
    CAMERA,
    // Light
    DIRECTIONAL_LIGHT,
    SPOT_LIGHT,
    POINT_LIGHT,
    // Model
    MODEL,
    // Geometry
    CUBE,
    PYRAMID,
    TEX_PYRAMID,
    SPHERE,
    CUBE_MAP,
    QUAD,
    // Simulation
    SIMULATION
};

const std::unordered_map<ObjectType, std::string> ObjectTypeStringMap = {
    {ObjectType::CAMERA, "Camera"},
    {ObjectType::DIRECTIONAL_LIGHT, "Directional Light"},
    {ObjectType::SPOT_LIGHT, "Spot Light"},
    {ObjectType::POINT_LIGHT, "Point Light"},
    {ObjectType::MODEL, "Model"},
    {ObjectType::CUBE, "Cube"},
    {ObjectType::PYRAMID, "Pyramid"},
    {ObjectType::TEX_PYRAMID, "Textured Pyramid"},
    {ObjectType::SPHERE, "Sphere"},
    {ObjectType::CUBE_MAP, "Cube Map"},
    {ObjectType::QUAD, "Quad"},
    {ObjectType::SIMULATION, "Simulation"}};

enum class ShaderDataType
{
    None = 0,
    Float,
    Float2,
    Float3,
    Float4,
    Mat3,
    Mat4,
    Int,
    Int2,
    Int3,
    Int4,
    Bool
};

static uint32_t ShaderDataTypeSize(ShaderDataType type)
{
    switch (type)
    {
    case ShaderDataType::Float:
        return 4;
    case ShaderDataType::Float2:
        return 4 * 2;
    case ShaderDataType::Float3:
        return 4 * 3;
    case ShaderDataType::Float4:
        return 4 * 4;
    case ShaderDataType::Mat3:
        return 4 * 3 * 3;
    case ShaderDataType::Mat4:
        return 4 * 4 * 4;
    case ShaderDataType::Int:
        return 4;
    case ShaderDataType::Int2:
        return 4 * 2;
    case ShaderDataType::Int3:
        return 4 * 3;
    case ShaderDataType::Int4:
        return 4 * 4;
    case ShaderDataType::Bool:
        return 1;
    case ShaderDataType::None:
        return -1;
    }

    return 0;
}

struct BufferElement
{
    std::string Name;
    ShaderDataType Type;
    uint32_t Size;
    uint32_t Offset;
    bool Normalized;

    BufferElement() {}

    BufferElement(ShaderDataType type, const std::string &name, bool normalized = false)
        : Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalized)
    {
    }

    uint32_t GetComponentCount() const
    {
        switch (Type)
        {
        case ShaderDataType::Float:
            return 1;
        case ShaderDataType::Float2:
            return 2;
        case ShaderDataType::Float3:
            return 3;
        case ShaderDataType::Float4:
            return 4;
        case ShaderDataType::Mat3:
            return 3 * 3;
        case ShaderDataType::Mat4:
            return 4 * 4;
        case ShaderDataType::Int:
            return 1;
        case ShaderDataType::Int2:
            return 2;
        case ShaderDataType::Int3:
            return 3;
        case ShaderDataType::Int4:
            return 4;
        case ShaderDataType::Bool:
            return 1;
        case ShaderDataType::None:
            return -1;
        }

        return 0;
    }
};

class BufferLayout
{
public:
    BufferLayout() {}

    BufferLayout(const std::initializer_list<BufferElement> &elements)
        : m_Elements(elements)
    {
        CalculateOffsetsAndStride();
    }

    inline uint32_t GetStride() const { return m_Stride; }
    inline const std::vector<BufferElement> &GetElements() const { return m_Elements; }

    std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
    std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
    std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
    std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }

private:
    void CalculateOffsetsAndStride()
    {
        uint32_t offset = 0;
        m_Stride = 0;
        for (auto &element : m_Elements)
        {
            element.Offset = offset;
            offset += element.Size;
            m_Stride += element.Size;
        }
    }

private:
    std::vector<BufferElement> m_Elements;
    uint32_t m_Stride = 0;
};

static std::string getAssetsFolder()
{
#ifndef __EMSCRIPTEN__
    return "assets/";
#else
    return "assets/";
#endif
}

static std::string getShaderSrc(std::string file)
{
    // Open a file and read the content into a char *
    std::string content;
    std::ifstream fileStream(file, std::ios::in);

    if (!fileStream.is_open())
    {
        printf("Error: Could not read file: %s\n", file.c_str());
        return "";
    }

    std::string line = "";
    while (!fileStream.eof())
    {
        std::getline(fileStream, line);
        content.append(line + "\n");
    }
    fileStream.close();

    // std::string filename = file.substr(file.find_last_of("/") + 1);
    printf("Info: Loaded shader: %s\n", file.c_str());

    return content;
}

#define vShdrSrc(name) getShaderSrc(getAssetsFolder() + "shaders/" #name ".vert")
#define fShdrSrc(name) getShaderSrc(getAssetsFolder() + "shaders/" #name ".frag")

#endif