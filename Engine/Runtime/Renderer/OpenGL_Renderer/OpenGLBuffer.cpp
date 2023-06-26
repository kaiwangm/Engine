#include <Engine/Runtime/Renderer/OpenGL_Renderer/OpenGLBuffer.h>

namespace Engine
{
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
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
    }

    void OpenGLFrameRenderBuffer::SetViewPort(uint32_t width, uint32_t height)
    {
        if (width != m_Width || height != m_Height)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer_RendererID);
            glBindTexture(GL_TEXTURE_2D, m_Texture_RendererID);
            glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBuffer_RendererID);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_Texture_RendererID, 0);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
            glFramebufferRenderbuffer(
                GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RenderBuffer_RendererID);

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glBindTexture(GL_TEXTURE_2D, 0);
            glBindRenderbuffer(GL_RENDERBUFFER, 0);

            m_Width  = width;
            m_Height = height;
        }

        return;
    }

    void* OpenGLFrameRenderBuffer::GetTextureID() const { return (void*)(uint64_t)m_Texture_RendererID; }

    uint32_t OpenGLFrameRenderBuffer::GetWidth() const { return m_Width; }

    uint32_t OpenGLFrameRenderBuffer::GetHeight() const { return m_Height; }
} // namespace Engine