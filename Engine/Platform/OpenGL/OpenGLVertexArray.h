#pragma once

#include "VertexArray.h"

namespace Engine {
class OpenGLVertexArray : public VertexArray {
   public:
    OpenGLVertexArray();
    virtual ~OpenGLVertexArray() {}

    virtual void Bind() const override;
    virtual void UnBind() const override;

    virtual void AddVertexBuffer(const Ref<VertexBuffer>& buffer) override;
    virtual void AddIndexBuffer(const Ref<IndexBuffer>& buffer) override;

    virtual const std::vector<Ref<VertexBuffer> >& GetVertexBuffers() const {
        return m_VertexBuffers;
    };
    virtual const Ref<IndexBuffer>& GetIndexBuffer() const {
        return m_IndexBuffer;
    };

   private:
    uint32_t m_RendererID;
    std::vector<Ref<VertexBuffer> > m_VertexBuffers;
    Ref<IndexBuffer> m_IndexBuffer;
};
}  // namespace Engine