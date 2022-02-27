#include "OpenGLVertexArray.h"

namespace Engine {
static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
    switch (type) {
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
    }

    return 0;
}

OpenGLVertexArray::OpenGLVertexArray() {
    glCreateVertexArrays(1, &m_RendererID);
}

void OpenGLVertexArray::Bind() const { glBindVertexArray(m_RendererID); }

void OpenGLVertexArray::UnBind() const { glBindVertexArray(0); }

void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& buffer) {
    glBindVertexArray(m_RendererID);
    buffer->Bind();

    uint32_t index = 0;

    const auto& layout = buffer->GetLayout();
    for (const auto& element : layout) {
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(
            index, element.GetComponentCount(),
            ShaderDataTypeToOpenGLBaseType(element.GetGLType()),
            element.GetNormalized() ? GL_TRUE : GL_FALSE, layout.GetStride(),
            element.GetOffset());
        index++;
    }

    m_VertexBuffers.push_back(buffer);
}

void OpenGLVertexArray::AddIndexBuffer(const Ref<IndexBuffer>& buffer) {
    glBindVertexArray(m_RendererID);
    buffer->Bind();

    m_IndexBuffer = buffer;
}

}  // namespace Engine