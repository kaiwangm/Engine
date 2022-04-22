#include "OpenGLVertexArray.h"

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

    OpenGLVertexArray::OpenGLVertexArray() { glCreateVertexArrays(1, &m_RendererID); }

    void OpenGLVertexArray::Bind() const { glBindVertexArray(m_RendererID); }

    void OpenGLVertexArray::UnBind() const { glBindVertexArray(0); }

    void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& buffer, const bool instance)
    {
        glBindVertexArray(m_RendererID);

        const auto& layout = buffer->GetLayout();
        for (const auto& element : layout)
        {
            glEnableVertexAttribArray(element.GetIndex());

            auto type = ShaderDataTypeToOpenGLBaseType(element.GetGLType());

            buffer->Bind();
            switch (type)
            {
                case GL_FLOAT:
                    glVertexAttribPointer(element.GetIndex(),
                                          element.GetComponentCount(), //
                                          type,                        //
                                          element.GetNormalized() ? GL_TRUE : GL_FALSE,
                                          layout.GetStride(),
                                          element.GetOffset());
                    break;
                case GL_INT:
                    glVertexAttribIPointer(element.GetIndex(),
                                           element.GetComponentCount(), //
                                           type,
                                           layout.GetStride(),
                                           element.GetOffset());
                    break;
                case GL_BOOL:
                    glVertexAttribIPointer(element.GetIndex(),
                                           element.GetComponentCount(), //
                                           type,
                                           layout.GetStride(),
                                           element.GetOffset());
                    break;
            }
            buffer->UnBind();

            if (instance)
            {
                glVertexAttribDivisor(element.GetIndex(), 1);
            }
        }

        m_VertexBuffers.push_back(buffer);
        glBindVertexArray(0);
    }

    void OpenGLVertexArray::AddIndexBuffer(const Ref<IndexBuffer>& buffer)
    {
        glBindVertexArray(m_RendererID);
        buffer->Bind();

        m_IndexBuffer = buffer;

        glBindVertexArray(0);
    }

} // namespace Engine