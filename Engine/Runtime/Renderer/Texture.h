#pragma once

#include "Core.h"

namespace Engine
{
    class Texture
    {
    public:
        virtual ~Texture() = default;

        virtual void  Bind(const uint32_t& slot) const   = 0;
        virtual void  UnBind(const uint32_t& slot) const = 0;
        virtual void* GetTextureID() const               = 0;
    };

    class Texture2D : public Texture
    {
    public:
        static Ref<Texture2D> Create();
        static Ref<Texture2D> Create(const std::string& path);

        virtual uint32_t GetWidth() const    = 0;
        virtual uint32_t GetHeight() const   = 0;
        virtual uint32_t GetChannels() const = 0;

        virtual void Load(void* data, uint32_t width, uint32_t height) = 0;
    };

    class CubeMap : public Texture
    {
    public:
        static Ref<CubeMap> Create(const std::string& path);
    };
} // namespace Engine
