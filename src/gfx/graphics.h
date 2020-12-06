#pragma once
#include <memory>
#include <windows.h>

namespace omix {

enum class ResourceType
{
    Unknown,
    Texture,
    Buffer,
};

class Resource
{
public:
    ResourceType m_type;
};

enum class TextureType
{
    Texture1D,
    Texture2D,
    Texture3D,
    CubeMap,
};

enum class TextureFormat
{
    RGBA_U8
};

class Texture : public Resource
{
public:
    using Ref = std::shared_ptr<Texture>;
};

class Graphics
{
    using ThisClass = Graphics;
public:
    enum class API
    {
        OpenGL_33,
        OpenGL_45,
        Direct3D_11,
        Direct3D_12,
        Vulkan,
        Metal,
    };
    virtual Texture::Ref CreateTexture() = 0;
    virtual void Commit() {}
};

}
