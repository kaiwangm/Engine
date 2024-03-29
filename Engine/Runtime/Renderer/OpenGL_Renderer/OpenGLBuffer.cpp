#include <Engine/Runtime/Renderer/OpenGL_Renderer/OpenGLBuffer.h>
#include <random>

namespace Engine
{
    GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
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

    // VertexBuffer
    OpenGLVertexBuffer::OpenGLVertexBuffer() { glCreateBuffers(1, &m_RendererID); }

    // VertexBuffer
    OpenGLVertexBuffer::OpenGLVertexBuffer(const void* vertices, uint32_t size, uint32_t count) : VertexBuffer(count)
    {
        glCreateBuffers(1, &m_RendererID);
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_DYNAMIC_DRAW);
    }

    OpenGLVertexBuffer::~OpenGLVertexBuffer() { glDeleteBuffers(1, &m_RendererID); }

    void OpenGLVertexBuffer::Bind() const { glBindBuffer(GL_ARRAY_BUFFER, m_RendererID); }

    void OpenGLVertexBuffer::UnBind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }

    void OpenGLVertexBuffer::SetLayout(const BufferLayout& layout) { this->layout = layout; }

    const BufferLayout& OpenGLVertexBuffer::GetLayout() const { return layout; }

    // IndexBuffer
    OpenGLIndexBuffer::OpenGLIndexBuffer() { glCreateBuffers(1, &m_RendererID); }

    OpenGLIndexBuffer::OpenGLIndexBuffer(const uint32_t* indices, uint32_t count) : m_Count(count)
    {
        glCreateBuffers(1, &m_RendererID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_DYNAMIC_DRAW);
    }

    OpenGLIndexBuffer::~OpenGLIndexBuffer() { glDeleteBuffers(1, &m_RendererID); }

    void OpenGLIndexBuffer::Bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID); }

    void OpenGLIndexBuffer::UnBind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

    uint32_t OpenGLIndexBuffer::GetCount() const { return m_Count; };

    void OpenGLIndexBuffer::SetCount(uint32_t count) { m_Count = count; }

    OpenGLFrameRenderBuffer::OpenGLFrameRenderBuffer() : m_Width(0), m_Height(0)
    {
        glGenFramebuffers(1, &m_FrameBuffer_RendererID);
        glGenTextures(1, &m_Texture_RendererID);
        glGenRenderbuffers(1, &m_RenderBuffer_RendererID);
    }

    OpenGLFrameRenderBuffer::~OpenGLFrameRenderBuffer()
    {
        glDeleteRenderbuffers(1, &m_RenderBuffer_RendererID);
        glDeleteTextures(1, &m_Texture_RendererID);
        glDeleteFramebuffers(1, &m_FrameBuffer_RendererID);
    }

    void OpenGLFrameRenderBuffer::Bind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer_RendererID);

        // glBindTexture(GL_TEXTURE_2D, m_Texture_RendererID);
        //  glBindTexture(GL_TEXTURE_2D, 0);

        // glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBuffer_RendererID);
        //  glBindRenderbuffer(GL_RENDERBUFFER, 0);
    }

    void OpenGLFrameRenderBuffer::UnBind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        // glBindTexture(GL_TEXTURE_2D, 0);
        // glBindRenderbuffer(GL_RENDERBUFFER, 0);
    }

    void OpenGLFrameRenderBuffer::SetViewPort(uint32_t width, uint32_t height)
    {
        if (width != m_Width || height != m_Height)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer_RendererID);

            glBindTexture(GL_TEXTURE_2D, m_Texture_RendererID);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_Texture_RendererID, 0);
            glBindTexture(GL_TEXTURE_2D, 0);

            glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBuffer_RendererID);
            glFramebufferRenderbuffer(
                GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RenderBuffer_RendererID);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
            glBindRenderbuffer(GL_RENDERBUFFER, 0);

            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            m_Width  = width;
            m_Height = height;
        }

        return;
    }

    void* OpenGLFrameRenderBuffer::GetTextureID() const { return (void*)(uint64_t)m_Texture_RendererID; }

    uint32_t OpenGLFrameRenderBuffer::GetWidth() const { return m_Width; }

    uint32_t OpenGLFrameRenderBuffer::GetHeight() const { return m_Height; }

    void OpenGLFrameRenderBuffer::BindTexture(const uint32_t& slot) const
    {
        glBindTextureUnit(slot, m_Texture_RendererID);
    }

    void OpenGLFrameRenderBuffer::UnBindTexture(const uint32_t& slot) const { glBindTextureUnit(slot, 0); }

    OpenGLShadowMapBuffer::OpenGLShadowMapBuffer() : m_Width(0), m_Height(0)
    {
        glGenFramebuffers(1, &m_FrameBuffer_RendererID);
        glGenTextures(1, &m_Texture_RendererID);
    }

    OpenGLShadowMapBuffer::~OpenGLShadowMapBuffer()
    {
        glDeleteTextures(1, &m_Texture_RendererID);
        glDeleteFramebuffers(1, &m_FrameBuffer_RendererID);
    }

    void OpenGLShadowMapBuffer::Bind() const { glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer_RendererID); }

    void OpenGLShadowMapBuffer::UnBind() const { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

    void OpenGLShadowMapBuffer::SetViewPort(uint32_t width, uint32_t height)
    {
        if (width != m_Width || height != m_Height)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer_RendererID);

            glBindTexture(GL_TEXTURE_2D, m_Texture_RendererID);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);     // GL_NEAREST
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);     // GL_NEAREST
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER); // GL_CLAMP_TO_BORDER
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER); // GL_CLAMP_TO_BORDER
            float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
            glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_Texture_RendererID, 0);
            glDrawBuffer(GL_NONE);
            glReadBuffer(GL_NONE);
            glBindTexture(GL_TEXTURE_2D, 0);

            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            m_Width  = width;
            m_Height = height;
        }

        return;
    }

    void* OpenGLShadowMapBuffer::GetTextureID() const { return (void*)(uint64_t)m_Texture_RendererID; }

    uint32_t OpenGLShadowMapBuffer::GetWidth() const { return m_Width; }

    uint32_t OpenGLShadowMapBuffer::GetHeight() const { return m_Height; }

    void OpenGLShadowMapBuffer::BindTexture(const uint32_t& slot) const
    {
        glBindTextureUnit(slot, m_Texture_RendererID);
    }

    void OpenGLShadowMapBuffer::UnBindTexture(const uint32_t& slot) const { glBindTextureUnit(slot, 0); }

    OpenGLShadowCubeMapBuffer::OpenGLShadowCubeMapBuffer() : m_Width(0), m_Height(0)
    {
        glGenFramebuffers(1, &m_FrameBuffer_RendererID);
        glGenTextures(1, &m_Texture_RendererID);
    }

    OpenGLShadowCubeMapBuffer::~OpenGLShadowCubeMapBuffer()
    {
        glDeleteTextures(1, &m_Texture_RendererID);
        glDeleteFramebuffers(1, &m_FrameBuffer_RendererID);
    }

    void OpenGLShadowCubeMapBuffer::BindTop() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer_RendererID);
        glFramebufferTexture2D(
            GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, m_Texture_RendererID, 0);
    }

    void OpenGLShadowCubeMapBuffer::BindBottom() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer_RendererID);
        glFramebufferTexture2D(
            GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, m_Texture_RendererID, 0);
    }

    void OpenGLShadowCubeMapBuffer::BindLeft() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer_RendererID);
        glFramebufferTexture2D(
            GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, m_Texture_RendererID, 0);
    }

    void OpenGLShadowCubeMapBuffer::BindRight() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer_RendererID);
        glFramebufferTexture2D(
            GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_CUBE_MAP_POSITIVE_X, m_Texture_RendererID, 0);
    }

    void OpenGLShadowCubeMapBuffer::BindFront() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer_RendererID);
        glFramebufferTexture2D(
            GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, m_Texture_RendererID, 0);
    }

    void OpenGLShadowCubeMapBuffer::BindBack() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer_RendererID);
        glFramebufferTexture2D(
            GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, m_Texture_RendererID, 0);
    }

    void OpenGLShadowCubeMapBuffer::UnBind() const { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

    void OpenGLShadowCubeMapBuffer::SetViewPort(uint32_t width, uint32_t height)
    {
        if (width != m_Width || height != m_Height)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer_RendererID);

            glBindTexture(GL_TEXTURE_CUBE_MAP, m_Texture_RendererID);
            for (unsigned int i = 0; i < 6; ++i)
            {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                             0,
                             GL_DEPTH_COMPONENT,
                             width,
                             height,
                             0,
                             GL_DEPTH_COMPONENT,
                             GL_FLOAT,
                             NULL);
            }
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // GL_NEAREST
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // GL_NEAREST
            // glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER); // GL_CLAMP_TO_BORDER
            // glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER); // GL_CLAMP_TO_BORDER
            // glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER); // GL_CLAMP_TO_BORDER
            float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
            glTexParameterfv(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BORDER_COLOR, borderColor);
            glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_Texture_RendererID, 0);
            glDrawBuffer(GL_NONE);
            glReadBuffer(GL_NONE);
            glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            m_Width  = width;
            m_Height = height;
        }

        return;
    }

    void* OpenGLShadowCubeMapBuffer::GetTextureID() const { return (void*)(uint64_t)m_Texture_RendererID; }

    uint32_t OpenGLShadowCubeMapBuffer::GetWidth() const { return m_Width; }

    uint32_t OpenGLShadowCubeMapBuffer::GetHeight() const { return m_Height; }

    void OpenGLShadowCubeMapBuffer::BindTexture(const uint32_t& slot) const
    {
        glBindTextureUnit(slot, m_Texture_RendererID);
    }

    void OpenGLShadowCubeMapBuffer::UnBindTexture(const uint32_t& slot) const { glBindTextureUnit(slot, 0); }

    OpenGLGeometryBuffer::OpenGLGeometryBuffer() : m_Width(0), m_Height(0)
    {
        glGenFramebuffers(1, &m_FrameBuffer_RendererID);
        glGenRenderbuffers(1, &m_RenderBuffer_RendererID);

        glGenTextures(1, &m_ViewPosition_RendererID);
        glGenTextures(1, &m_ViewNormal_RendererID);
        glGenTextures(1, &m_Diffuse_RendererID);
        glGenTextures(1, &m_Depth_RendererID);
        glGenTextures(1, &m_Roughness_RendererID);
        glGenTextures(1, &m_Specular_RendererID);
        glGenTextures(1, &m_WorldPosition_RendererID);
        glGenTextures(1, &m_WorldNormal_RendererID);
        glGenTextures(1, &m_Emissive_RendererID);
    }

    OpenGLGeometryBuffer::~OpenGLGeometryBuffer()
    {
        glDeleteTextures(1, &m_Emissive_RendererID);
        glDeleteTextures(1, &m_WorldNormal_RendererID);
        glDeleteTextures(1, &m_WorldPosition_RendererID);
        glDeleteTextures(1, &m_Specular_RendererID);
        glDeleteTextures(1, &m_Roughness_RendererID);
        glDeleteTextures(1, &m_Depth_RendererID);
        glDeleteTextures(1, &m_Diffuse_RendererID);
        glDeleteTextures(1, &m_ViewNormal_RendererID);
        glDeleteTextures(1, &m_ViewPosition_RendererID);

        glDeleteRenderbuffers(1, &m_RenderBuffer_RendererID);
        glDeleteFramebuffers(1, &m_FrameBuffer_RendererID);
    }

    void OpenGLGeometryBuffer::Bind() const { glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer_RendererID); }

    void OpenGLGeometryBuffer::UnBind() const { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

    void OpenGLGeometryBuffer::SetViewPort(uint32_t width, uint32_t height)
    {
        if (width != m_Width || height != m_Height)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer_RendererID);

            glBindTexture(GL_TEXTURE_2D, m_ViewPosition_RendererID);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ViewPosition_RendererID, 0);
            glBindTexture(GL_TEXTURE_2D, 0);

            glBindTexture(GL_TEXTURE_2D, m_ViewNormal_RendererID);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_ViewNormal_RendererID, 0);
            glBindTexture(GL_TEXTURE_2D, 0);

            glBindTexture(GL_TEXTURE_2D, m_Diffuse_RendererID);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_Diffuse_RendererID, 0);
            glBindTexture(GL_TEXTURE_2D, 0);

            glBindTexture(GL_TEXTURE_2D, m_Depth_RendererID);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_FLOAT, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, m_Depth_RendererID, 0);
            glBindTexture(GL_TEXTURE_2D, 0);

            glBindTexture(GL_TEXTURE_2D, m_Roughness_RendererID);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_FLOAT, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, m_Roughness_RendererID, 0);
            glBindTexture(GL_TEXTURE_2D, 0);

            glBindTexture(GL_TEXTURE_2D, m_Specular_RendererID);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT5, GL_TEXTURE_2D, m_Specular_RendererID, 0);
            glBindTexture(GL_TEXTURE_2D, 0);

            glBindTexture(GL_TEXTURE_2D, m_WorldPosition_RendererID);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT6, GL_TEXTURE_2D, m_WorldPosition_RendererID, 0);
            glBindTexture(GL_TEXTURE_2D, 0);

            glBindTexture(GL_TEXTURE_2D, m_WorldNormal_RendererID);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT7, GL_TEXTURE_2D, m_WorldNormal_RendererID, 0);
            glBindTexture(GL_TEXTURE_2D, 0);

            glBindTexture(GL_TEXTURE_2D, m_Emissive_RendererID);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            // glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT8, GL_TEXTURE_2D, m_Emissive_RendererID, 0);
            glBindTexture(GL_TEXTURE_2D, 0);

            const int num_attachments              = 8;
            GLuint    attachments[num_attachments] = {
                GL_COLOR_ATTACHMENT0,
                GL_COLOR_ATTACHMENT1,
                GL_COLOR_ATTACHMENT2,
                GL_COLOR_ATTACHMENT3,
                GL_COLOR_ATTACHMENT4,
                GL_COLOR_ATTACHMENT5,
                GL_COLOR_ATTACHMENT6,
                GL_COLOR_ATTACHMENT7,
                // GL_COLOR_ATTACHMENT8,
            };
            glDrawBuffers(num_attachments, attachments);

            glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBuffer_RendererID);
            glFramebufferRenderbuffer(
                GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RenderBuffer_RendererID);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
            glBindRenderbuffer(GL_RENDERBUFFER, 0);

            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            m_Width  = width;
            m_Height = height;
        }

        return;
    }

    uint32_t OpenGLGeometryBuffer::GetWidth() const { return m_Width; }

    uint32_t OpenGLGeometryBuffer::GetHeight() const { return m_Height; }

    void* OpenGLGeometryBuffer::GetViewPositionTextureID() const { return (void*)(uint64_t)m_ViewPosition_RendererID; }

    void* OpenGLGeometryBuffer::GetViewNormalTextureID() const { return (void*)(uint64_t)m_ViewNormal_RendererID; }

    void* OpenGLGeometryBuffer::GetDiffuseTextureID() const { return (void*)(uint64_t)m_Diffuse_RendererID; }

    void* OpenGLGeometryBuffer::GetDepthTextureID() const { return (void*)(uint64_t)m_Depth_RendererID; }

    void* OpenGLGeometryBuffer::GetRoughnessTextureID() const { return (void*)(uint64_t)m_Roughness_RendererID; }

    void* OpenGLGeometryBuffer::GetSpecularTextureID() const { return (void*)(uint64_t)m_Specular_RendererID; }

    void* OpenGLGeometryBuffer::GetWorldPositionTextureID() const
    {
        return (void*)(uint64_t)m_WorldPosition_RendererID;
    }

    void* OpenGLGeometryBuffer::GetWorldNormalTextureID() const { return (void*)(uint64_t)m_WorldNormal_RendererID; }

    void* OpenGLGeometryBuffer::GetEmissiveTextureID() const { return (void*)(uint64_t)m_Emissive_RendererID; }

    void OpenGLGeometryBuffer::BindViewPositionTexture(const uint32_t& slot) const
    {
        glBindTextureUnit(slot, m_ViewPosition_RendererID);
    }

    void OpenGLGeometryBuffer::BindViewNormalTexture(const uint32_t& slot) const
    {
        glBindTextureUnit(slot, m_ViewNormal_RendererID);
    }

    void OpenGLGeometryBuffer::BindDiffuseTexture(const uint32_t& slot) const
    {
        glBindTextureUnit(slot, m_Diffuse_RendererID);
    }

    void OpenGLGeometryBuffer::BindDepthTexture(const uint32_t& slot) const
    {
        glBindTextureUnit(slot, m_Depth_RendererID);
    }

    void OpenGLGeometryBuffer::BindRoughnessTexture(const uint32_t& slot) const
    {
        glBindTextureUnit(slot, m_Roughness_RendererID);
    }

    void OpenGLGeometryBuffer::BindSpecularTexture(const uint32_t& slot) const
    {
        glBindTextureUnit(slot, m_Specular_RendererID);
    }

    void OpenGLGeometryBuffer::BindWorldPositionTexture(const uint32_t& slot) const
    {
        glBindTextureUnit(slot, m_WorldPosition_RendererID);
    }

    void OpenGLGeometryBuffer::BindWorldNormalTexture(const uint32_t& slot) const
    {
        glBindTextureUnit(slot, m_WorldNormal_RendererID);
    }

    void OpenGLGeometryBuffer::BindEmissiveTexture(const uint32_t& slot) const
    {
        glBindTextureUnit(slot, m_Emissive_RendererID);
    }

    void OpenGLGeometryBuffer::UnBindTexture(const uint32_t& slot) const { glBindTextureUnit(slot, 0); }

    OpenGLSSAOBuffer::OpenGLSSAOBuffer() : m_Width(0), m_Height(0), m_Radius(1.0f), m_Bias(0.002f), m_Power(1.0f)
    {
        glGenFramebuffers(1, &m_FrameBuffer_RendererID);
        glGenRenderbuffers(1, &m_RenderBuffer_RendererID);

        glGenTextures(1, &m_SSAO_RendererID);

        // Sample kernel
        std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0); // generates random floats between 0.0 and 1.0
        std::default_random_engine              generator;
        for (GLuint i = 0; i < 256; ++i)
        {
            glm::vec3 sample(
                randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, randomFloats(generator));
            sample = glm::normalize(sample);
            sample *= randomFloats(generator);
            GLfloat scale = GLfloat(i) / 256.0;

            // Scale samples s.t. they're more aligned to center of kernel
            scale = lerp(0.1f, 1.0f, scale * scale);
            sample *= scale;
            ssaoKernel.push_back(sample);
        }

        // Noise texture
        for (GLuint i = 0; i < 16; i++)
        {
            glm::vec3 noise(randomFloats(generator) * 2.0 - 1.0,
                            randomFloats(generator) * 2.0 - 1.0,
                            0.0f); // rotate around z-axis (in tangent space)
            ssaoNoise.push_back(noise);
        }
        glGenTextures(1, &noiseTexture);
        glBindTexture(GL_TEXTURE_2D, noiseTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 4, 4, 0, GL_RGB, GL_FLOAT, &ssaoNoise[0]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    OpenGLSSAOBuffer::~OpenGLSSAOBuffer()
    {
        glDeleteTextures(1, &m_SSAO_RendererID);

        glDeleteRenderbuffers(1, &m_RenderBuffer_RendererID);
        glDeleteFramebuffers(1, &m_FrameBuffer_RendererID);
    }

    void OpenGLSSAOBuffer::Bind() const { glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer_RendererID); }

    void OpenGLSSAOBuffer::UnBind() const { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

    void OpenGLSSAOBuffer::SetViewPort(uint32_t width, uint32_t height)
    {
        if (width != m_Width || height != m_Height)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer_RendererID);

            glBindTexture(GL_TEXTURE_2D, m_SSAO_RendererID);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_FLOAT, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_SSAO_RendererID, 0);
            glBindTexture(GL_TEXTURE_2D, 0);

            GLuint attachments[1] = {GL_COLOR_ATTACHMENT0};
            glDrawBuffers(1, attachments);

            glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBuffer_RendererID);
            glFramebufferRenderbuffer(
                GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RenderBuffer_RendererID);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
            glBindRenderbuffer(GL_RENDERBUFFER, 0);

            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            m_Width  = width;
            m_Height = height;
        }

        return;
    }

    uint32_t OpenGLSSAOBuffer::GetWidth() const { return m_Width; }

    uint32_t OpenGLSSAOBuffer::GetHeight() const { return m_Height; }

    float& OpenGLSSAOBuffer::GetRadiusRef() { return m_Radius; }

    float& OpenGLSSAOBuffer::GetBiasRef() { return m_Bias; }

    float& OpenGLSSAOBuffer::GetPowerRef() { return m_Power; }

    void* OpenGLSSAOBuffer::GetSSAOTextureID() const { return (void*)(uint64_t)m_SSAO_RendererID; }

    void* OpenGLSSAOBuffer::GetNoiseTextureID() const { return (void*)(uint64_t)noiseTexture; }

    void OpenGLSSAOBuffer::BindSSAOTexture(const uint32_t& slot) const { glBindTextureUnit(slot, m_SSAO_RendererID); }

    void OpenGLSSAOBuffer::BindNoiseTexture(const uint32_t& slot) const { glBindTextureUnit(slot, noiseTexture); }

    void OpenGLSSAOBuffer::UnBindTexture(const uint32_t& slot) const { glBindTextureUnit(slot, 0); }

    std::vector<glm::vec3>& OpenGLSSAOBuffer::GetSSAOKernel() { return ssaoKernel; }

    GLfloat OpenGLSSAOBuffer::lerp(GLfloat a, GLfloat b, GLfloat f) { return a + f * (b - a); }
} // namespace Engine