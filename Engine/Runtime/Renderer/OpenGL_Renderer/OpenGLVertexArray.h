#pragma once
#include <Engine/Runtime/Core/Core.h>
#include <Engine/Runtime/Renderer/VertexArray.h>

namespace Engine
{
    class OpenGLVertexArray : public VertexArray
    {
    public:
        OpenGLVertexArray();
        virtual ~OpenGLVertexArray() {}

        virtual void Bind() const override;
        virtual void UnBind() const override;

        virtual void AddVertexBuffer(const Ref<VertexBuffer>& buffer, const bool instance) override;
        virtual void AddIndexBuffer(const Ref<IndexBuffer>& buffer) override;

        virtual const Ref<IndexBuffer>& GetIndexBuffer() const override { return m_IndexBuffer; };

    private:
        GLuint            m_RendererID;
        Ref<VertexBuffer> m_VertexBuffer;
        Ref<IndexBuffer>  m_IndexBuffer;
    };
} // namespace Engine