#pragma once
#include <Engine/Runtime/Renderer/Buffer.h>
#include <Engine/Runtime/Core/Core.h>

namespace Engine
{
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
            default:
                Log::Core_Error("Unknown DataType!");
                return -1;
        }

        return 0;
    }

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

        virtual uint32_t GetCount() const override { return m_Count; };

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

        virtual void BindPositionTexture(const uint32_t& slot) const override;
        virtual void BindNormalTexture(const uint32_t& slot) const override;
        virtual void BindAlbedoTexture(const uint32_t& slot) const override;
        virtual void BindOpacityTexture(const uint32_t& slot) const override;
        virtual void BindDepthTexture(const uint32_t& slot) const override;
        virtual void UnBindTexture(const uint32_t& slot) const override;

    private:
        uint32_t m_Width;
        uint32_t m_Height;
        GLuint   m_FrameBuffer_RendererID;
        // GLuint   m_Texture_RendererID;
        GLuint m_RenderBuffer_RendererID;

        GLuint m_Position_RendererID;
        GLuint m_Normal_RendererID;
        GLuint m_Albedo_RendererID;
        GLuint m_Opacity_RendererID;
        GLuint m_Depth_RendererID;
    };

} // namespace Engine