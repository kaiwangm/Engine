#pragma once
#include <Engine/Runtime/Core/Core.h>

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

        virtual void Buffer() = 0;
        virtual void Clear()  = 0;
    };

    class Texture3D : public Texture
    {
    public:
        static Ref<Texture3D> Create();
        static Ref<Texture3D> Create(const uint32_t& width, const uint32_t& height, const uint32_t& depth);

        virtual uint32_t GetWidth() const    = 0;
        virtual uint32_t GetHeight() const   = 0;
        virtual uint32_t GetDepth() const    = 0;
        virtual uint32_t GetChannels() const = 0;
    };

    class CubeMap : public Texture
    {
    public:
        static Ref<CubeMap> Create(const std::string& path = "");

        virtual void  Bind(const uint32_t& slot) const   = 0;
        virtual void  UnBind(const uint32_t& slot) const = 0;
        virtual void* GetTextureID() const               = 0;

        virtual void BindIrradianceTexture(const uint32_t& slot) const = 0;
        virtual void BindPrefilterTexture(const uint32_t& slot) const  = 0;
        virtual void BindBrdfLutTexture(const uint32_t& slot) const    = 0;

        virtual void* GetIrradianceTextureID() const = 0;
        virtual void* GetPrefilterTextureID() const  = 0;
        virtual void* GetBrdfLutTextureID() const    = 0;

        virtual void ComputeIrradianceTexture()         = 0;
        virtual void ComputeSphereHarmonicsParameters() = 0;
        virtual void ComputePrefilterTexture()          = 0;

        virtual const std::array<glm::vec3, 9>& GetSphereHarmonicsParametersRef() const = 0;
    };
} // namespace Engine
