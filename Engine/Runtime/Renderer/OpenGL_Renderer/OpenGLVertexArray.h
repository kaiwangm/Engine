#pragma once
#include "Core.h"
#include "../VertexArray.h"

namespace Engine {
class OpenGLVertexArray : public VertexArray {
   public:
    OpenGLVertexArray();
    virtual ~OpenGLVertexArray() {}

    virtual void Bind() const override;
    virtual void UnBind() const override;

    virtual void AddVertexBuffer(const Ref<VertexBuffer>& buffer,
                                 const bool instance) override;
    virtual void AddIndexBuffer(const Ref<IndexBuffer>& buffer) override;

    virtual const std::vector<Ref<VertexBuffer> >& GetVertexBuffers()
        const override {
        return m_VertexBuffers;
    };

    virtual const Ref<IndexBuffer>& GetIndexBuffer() const override {
        return m_IndexBuffer;
    };

   private:
    GLuint m_RendererID;
    std::vector<Ref<VertexBuffer> > m_VertexBuffers;
    Ref<IndexBuffer> m_IndexBuffer;
};
}  // namespace Engine