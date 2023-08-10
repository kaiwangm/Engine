#pragma once
#include <Engine/Runtime/Core/Core.h>
#include <Engine/Runtime/Renderer/Texture.h>
#include <stb_image.h>

namespace Engine
{
    class OpenGLTexture2D : public Texture2D
    {
    public:
        OpenGLTexture2D();
        OpenGLTexture2D(const std::string& path);
        virtual ~OpenGLTexture2D();

        virtual void Buffer() override;
        virtual void Clear() override;

        virtual uint32_t GetWidth() const override { return m_Width; }
        virtual uint32_t GetHeight() const override { return m_Height; }
        virtual uint32_t GetChannels() const override { return m_Channels; }

        virtual void Bind(const uint32_t& slot) const override;
        virtual void UnBind(const uint32_t& slot) const override;

        virtual void* GetTextureID() const override;

    private:
        GLenum   internalFormate = 0;
        GLenum   dataFormate     = 0;
        stbi_uc* data;
        uint32_t m_Width, m_Height, m_Channels;
        GLuint   m_TextureID;
    };

    class OpenGLTexture3D : public Texture3D
    {
    public:
        OpenGLTexture3D();
        OpenGLTexture3D(const uint32_t& width, const uint32_t& height, const uint32_t& depth);
        virtual ~OpenGLTexture3D() { glDeleteTextures(1, &m_TextureID); }

        virtual uint32_t GetWidth() const override { return m_Width; }
        virtual uint32_t GetHeight() const override { return m_Height; }
        virtual uint32_t GetDepth() const override { return m_Depth; }
        virtual uint32_t GetChannels() const override { return m_Channels; }

        virtual void Bind(const uint32_t& slot) const override;
        virtual void UnBind(const uint32_t& slot) const override;

        virtual void* GetTextureID() const override;

    private:
        uint32_t m_Width, m_Height, m_Depth, m_Channels;
        GLuint   m_TextureID;
    };

    class OpenGLCubeMap : public CubeMap
    {
    public:
        OpenGLCubeMap(const std::string& path);
        virtual ~OpenGLCubeMap();

        virtual void  Bind(const uint32_t& slot) const override;
        virtual void  UnBind(const uint32_t& slot) const override;
        virtual void* GetTextureID() const override;

        virtual void BindIrradianceTexture(const uint32_t& slot) const override;
        virtual void BindPrefilterTexture(const uint32_t& slot) const override;
        virtual void BindBrdfLutTexture(const uint32_t& slot) const override;

        virtual void* GetIrradianceTextureID() const override;
        virtual void* GetPrefilterTextureID() const override;
        virtual void* GetBrdfLutTextureID() const override;

    private:
        std::string m_Path;
        GLuint      m_CubeMap, m_Irradiance, m_Prefilter;
        GLuint      m_BrdfLut;

    private:
        std::vector<glm::vec3> sh_data;

    public:
        virtual void ComputeIrradianceTexture() override;
        virtual void ComputeSphereHarmonicsParameters() override;
        virtual void ComputePrefilterTexture() override;

    public:
        virtual const std::vector<glm::vec3>& GetSphereHarmonicsParametersRef() const override { return sh_data; }
    };
} // namespace Engine