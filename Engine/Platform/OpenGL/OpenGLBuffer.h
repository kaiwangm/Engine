#pragma once

#include "Buffer.h"

namespace Engine
{
    static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Float:    return GL_FLOAT;
			case ShaderDataType::Float2:   return GL_FLOAT;
			case ShaderDataType::Float3:   return GL_FLOAT;
			case ShaderDataType::Float4:   return GL_FLOAT;
			case ShaderDataType::Mat3:     return GL_FLOAT;
			case ShaderDataType::Mat4:     return GL_FLOAT;
			case ShaderDataType::Int:      return GL_INT;
			case ShaderDataType::Int2:     return GL_INT;
			case ShaderDataType::Int3:     return GL_INT;
			case ShaderDataType::Int4:     return GL_INT;
			case ShaderDataType::Bool:     return GL_BOOL;
		}

		return 0;
	}
    
    class OpenGLVertexBuffer : public VertexBuffer
    {
        public:
            OpenGLVertexBuffer(float* vertices, uint32_t size);
            virtual ~OpenGLVertexBuffer();

            virtual void Bind() const;
            virtual void UnBind() const;

            virtual void SetLayout(const BufferLayout& layout);
            virtual const BufferLayout& GetLayout() const;
        
        private:
            uint32_t m_RendererID;
    };

    class OpenGLIndexBuffer : public IndexBuffer
    {
        public:
            OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
            virtual ~OpenGLIndexBuffer();

            virtual void Bind() const;
            virtual void UnBind() const;

            virtual uint32_t GetCount() const { return m_Count; };
        
        private:
            uint32_t m_RendererID;
            uint32_t m_Count;
    };
}