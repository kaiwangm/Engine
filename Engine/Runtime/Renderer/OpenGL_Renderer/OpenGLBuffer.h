#pragma once
#include <Engine/Runtime/Renderer/Buffer.h>
#include <Engine/Runtime/Core/Core.h>

namespace Engine
{
    static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type);

    class OpenGLVertexBuffer : public VertexBuffer
    {
    public:
        OpenGLVertexBuffer(const void* vertices, uint32_t size, uint32_t count);
        virtual ~OpenGLVertexBuffer();

        virtual void Bind() const override;
        virtual void UnBind() const override;

        virtual void                SetLayout(const BufferLayout& layout) override;
        virtual const BufferLayout& GetLayout() const override;

    private:
        GLuint m_RendererID;
    };

    class OpenGLIndexBuffer : public IndexBuffer
    {
    public:
        OpenGLIndexBuffer(const uint32_t* indices, uint32_t count);
        virtual ~OpenGLIndexBuffer();

        virtual void Bind() const override;
        virtual void UnBind() const override;

        virtual uint32_t GetCount() const override;

    private:
        GLuint   m_RendererID;
        uint32_t m_Count;
    };

    class OpenGLFrameRenderBuffer : public FrameRenderBuffer
    {
    public:
        OpenGLFrameRenderBuffer();
        virtual ~OpenGLFrameRenderBuffer();

        virtual void Bind() const override;
        virtual void UnBind() const override;

        virtual void  SetViewPort(uint32_t width, uint32_t height) override;
        virtual void* GetTextureID() const override;

        virtual uint32_t GetWidth() const override;
        virtual uint32_t GetHeight() const override;

    private:
        uint32_t m_Width;
        uint32_t m_Height;
        GLuint   m_FrameBuffer_RendererID;
        GLuint   m_Texture_RendererID;
        GLuint   m_RenderBuffer_RendererID;
    };

    class OpenGLGeometryBuffer : public GeometryBuffer
    {
    public:
        OpenGLGeometryBuffer();
        virtual ~OpenGLGeometryBuffer();

        virtual void Bind() const override;
        virtual void UnBind() const override;

        virtual void SetViewPort(uint32_t width, uint32_t height) override;

        virtual uint32_t GetWidth() const override;
        virtual uint32_t GetHeight() const override;
        virtual void*    GetPositionTextureID() const override;
        virtual void*    GetNormalTextureID() const override;
        virtual void*    GetAlbedoTextureID() const override;
        virtual void*    GetOpacityTextureID() const override;
        virtual void*    GetDepthTextureID() const override;
        virtual void*    GetRoughnessTextureID() const override;
        virtual void*    GetMetallicTextureID() const  override;

        virtual void BindPositionTexture(const uint32_t& slot) const override;
        virtual void BindNormalTexture(const uint32_t& slot) const override;
        virtual void BindAlbedoTexture(const uint32_t& slot) const override;
        virtual void BindOpacityTexture(const uint32_t& slot) const override;
        virtual void BindDepthTexture(const uint32_t& slot) const override;
        virtual void BindRoughnessTexture(const uint32_t& slot) const override;
        virtual void BindMetallicTexture(const uint32_t& slot) const override;
        virtual void UnBindTexture(const uint32_t& slot) const override;

    private:
        uint32_t m_Width;
        uint32_t m_Height;

        GLuint m_FrameBuffer_RendererID;
        GLuint m_RenderBuffer_RendererID;

        GLuint m_Position_RendererID;
        GLuint m_Normal_RendererID;
        GLuint m_Albedo_RendererID;
        GLuint m_Opacity_RendererID;
        GLuint m_Depth_RendererID;
        GLuint m_Roughness_RendererID;
        GLuint m_Metallic_RendererID;
    };

    class OpenGLSSAOBuffer : public SSAOBuffer
    {
    public:
        OpenGLSSAOBuffer();
        virtual ~OpenGLSSAOBuffer();

        virtual void Bind() const override;
        virtual void UnBind() const override;

        virtual void SetViewPort(uint32_t width, uint32_t height) override;

        virtual uint32_t GetWidth() const override;
        virtual uint32_t GetHeight() const override;
        virtual float&   GetRadiusRef() override;
        virtual float&   GetBiasRef() override;
        virtual float&   GetPowerRef() override;

        virtual void* GetSSAOTextureID() const override;
        virtual void* GetNoiseTextureID() const override;

        virtual void BindSSAOTexture(const uint32_t& slot) const override;
        virtual void BindNoiseTexture(const uint32_t& slot) const override;
        virtual void UnBindTexture(const uint32_t& slot) const override;

        virtual std::vector<glm::vec3>& GetSSAOKernel() override;

    public:
        float m_Radius;
        float m_Bias;
        float m_Power;

    private:
        uint32_t m_Width;
        uint32_t m_Height;

        GLuint m_FrameBuffer_RendererID;
        GLuint m_RenderBuffer_RendererID;

        GLuint m_SSAO_RendererID;

    private:
        std::vector<glm::vec3> ssaoKernel;
        std::vector<glm::vec3> ssaoNoise;
        GLuint                 noiseTexture;

    private:
        static GLfloat lerp(GLfloat a, GLfloat b, GLfloat f);
    };

} // namespace Engine