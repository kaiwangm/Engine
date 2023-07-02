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
    OpenGLVertexBuffer::OpenGLVertexBuffer(const void* vertices, uint32_t size, uint32_t count) : VertexBuffer(count)
    {
        glCreateBuffers(1, &m_RendererID);
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    }

    OpenGLVertexBuffer::~OpenGLVertexBuffer() { glDeleteBuffers(1, &m_RendererID); }

    void OpenGLVertexBuffer::Bind() const { glBindBuffer(GL_ARRAY_BUFFER, m_RendererID); }

    void OpenGLVertexBuffer::UnBind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }

    void OpenGLVertexBuffer::SetLayout(const BufferLayout& layout) { this->layout = layout; }

    const BufferLayout& OpenGLVertexBuffer::GetLayout() const { return layout; }

    // IndexBuffer

    OpenGLIndexBuffer::OpenGLIndexBuffer(const uint32_t* indices, uint32_t count) : m_Count(count)
    {
        glCreateBuffers(1, &m_RendererID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
    }

    OpenGLIndexBuffer::~OpenGLIndexBuffer() { glDeleteBuffers(1, &m_RendererID); }

    void OpenGLIndexBuffer::Bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID); }

    void OpenGLIndexBuffer::UnBind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

    uint32_t OpenGLIndexBuffer::GetCount() const { return m_Count; };

    OpenGLFrameRenderBuffer::OpenGLFrameRenderBuffer() : m_Width(0), m_Height(0)
    {
        glGenFramebuffers(1, &m_FrameBuffer_RendererID);
        glGenTextures(1, &m_Texture_RendererID);
        glGenRenderbuffers(1, &m_RenderBuffer_RendererID);
    }

    OpenGLFrameRenderBuffer::~OpenGLFrameRenderBuffer()
    {
        glDeleteFramebuffers(1, &m_FrameBuffer_RendererID);
        glDeleteTextures(1, &m_Texture_RendererID);
        glDeleteRenderbuffers(1, &m_RenderBuffer_RendererID);
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
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
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

    OpenGLGeometryBuffer::OpenGLGeometryBuffer() : m_Width(0), m_Height(0)
    {
        glGenFramebuffers(1, &m_FrameBuffer_RendererID);
        glGenRenderbuffers(1, &m_RenderBuffer_RendererID);

        glGenTextures(1, &m_Position_RendererID);
        glGenTextures(1, &m_Normal_RendererID);
        glGenTextures(1, &m_Albedo_RendererID);
        glGenTextures(1, &m_Opacity_RendererID);
        glGenTextures(1, &m_Depth_RendererID);
        glGenTextures(1, &m_Roughness_RendererID);
        glGenTextures(1, &m_Metallic_RendererID);
    }

    OpenGLGeometryBuffer::~OpenGLGeometryBuffer()
    {
        glDeleteTextures(1, &m_Metallic_RendererID);
        glDeleteTextures(1, &m_Roughness_RendererID);
        glDeleteTextures(1, &m_Depth_RendererID);
        glDeleteTextures(1, &m_Opacity_RendererID);
        glDeleteTextures(1, &m_Albedo_RendererID);
        glDeleteTextures(1, &m_Normal_RendererID);
        glDeleteTextures(1, &m_Position_RendererID);

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

            glBindTexture(GL_TEXTURE_2D, m_Position_RendererID);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_Position_RendererID, 0);
            glBindTexture(GL_TEXTURE_2D, 0);

            glBindTexture(GL_TEXTURE_2D, m_Normal_RendererID);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_Normal_RendererID, 0);
            glBindTexture(GL_TEXTURE_2D, 0);

            glBindTexture(GL_TEXTURE_2D, m_Albedo_RendererID);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_Albedo_RendererID, 0);
            glBindTexture(GL_TEXTURE_2D, 0);

            glBindTexture(GL_TEXTURE_2D, m_Opacity_RendererID);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_FLOAT, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, m_Opacity_RendererID, 0);
            glBindTexture(GL_TEXTURE_2D, 0);

            glBindTexture(GL_TEXTURE_2D, m_Depth_RendererID);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_FLOAT, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, m_Depth_RendererID, 0);
            glBindTexture(GL_TEXTURE_2D, 0);

            glBindTexture(GL_TEXTURE_2D, m_Roughness_RendererID);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_FLOAT, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT5, GL_TEXTURE_2D, m_Roughness_RendererID, 0);
            glBindTexture(GL_TEXTURE_2D, 0);

            glBindTexture(GL_TEXTURE_2D, m_Metallic_RendererID);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_FLOAT, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT6, GL_TEXTURE_2D, m_Metallic_RendererID, 0);
            glBindTexture(GL_TEXTURE_2D, 0);

            GLuint attachments[7] = {GL_COLOR_ATTACHMENT0,
                                     GL_COLOR_ATTACHMENT1,
                                     GL_COLOR_ATTACHMENT2,
                                     GL_COLOR_ATTACHMENT3,
                                     GL_COLOR_ATTACHMENT4,
                                     GL_COLOR_ATTACHMENT5,
                                     GL_COLOR_ATTACHMENT6};
            glDrawBuffers(7, attachments);

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

    void* OpenGLGeometryBuffer::GetPositionTextureID() const { return (void*)(uint64_t)m_Position_RendererID; }

    void* OpenGLGeometryBuffer::GetNormalTextureID() const { return (void*)(uint64_t)m_Normal_RendererID; }

    void* OpenGLGeometryBuffer::GetAlbedoTextureID() const { return (void*)(uint64_t)m_Albedo_RendererID; }

    void* OpenGLGeometryBuffer::GetOpacityTextureID() const { return (void*)(uint64_t)m_Opacity_RendererID; }

    void* OpenGLGeometryBuffer::GetDepthTextureID() const { return (void*)(uint64_t)m_Depth_RendererID; }

    void* OpenGLGeometryBuffer::GetRoughnessTextureID() const { return (void*)(uint64_t)m_Roughness_RendererID; }

    void* OpenGLGeometryBuffer::GetMetallicTextureID() const { return (void*)(uint64_t)m_Metallic_RendererID; }

    void OpenGLGeometryBuffer::BindPositionTexture(const uint32_t& slot) const
    {
        glBindTextureUnit(slot, m_Position_RendererID);
    }

    void OpenGLGeometryBuffer::BindNormalTexture(const uint32_t& slot) const
    {
        glBindTextureUnit(slot, m_Normal_RendererID);
    }

    void OpenGLGeometryBuffer::BindAlbedoTexture(const uint32_t& slot) const
    {
        glBindTextureUnit(slot, m_Albedo_RendererID);
    }

    void OpenGLGeometryBuffer::BindOpacityTexture(const uint32_t& slot) const
    {
        glBindTextureUnit(slot, m_Opacity_RendererID);
    }

    void OpenGLGeometryBuffer::BindDepthTexture(const uint32_t& slot) const
    {
        glBindTextureUnit(slot, m_Depth_RendererID);
    }

    void OpenGLGeometryBuffer::BindRoughnessTexture(const uint32_t& slot) const
    {
        glBindTextureUnit(slot, m_Roughness_RendererID);
    }

    void OpenGLGeometryBuffer::BindMetallicTexture(const uint32_t& slot) const
    {
        glBindTextureUnit(slot, m_Metallic_RendererID);
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