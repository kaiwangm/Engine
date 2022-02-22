#pragma once

#include "VertexArray.h"

namespace Engine
{
    class OpenGLVertexArray : public VertexArray
    {
        public:
            OpenGLVertexArray();
            virtual ~OpenGLVertexArray() {}

            virtual void Bind() const override;
            virtual void UnBind() const override;

            virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& buffer) override;
            virtual void AddIndexBuffer(const std::shared_ptr<IndexBuffer>& buffer) override;

            virtual const std::vector<std::shared_ptr<VertexBuffer> >& GetVertexBuffers() const { return m_VertexBuffers; };
            virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const { return m_IndexBuffer; };

        private:
            uint32_t m_RendererID;
            std::vector<std::shared_ptr<VertexBuffer> > m_VertexBuffers;
            std::shared_ptr<IndexBuffer> m_IndexBuffer;

    };
}