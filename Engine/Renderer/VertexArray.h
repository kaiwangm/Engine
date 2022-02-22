#pragma once

#include "Core.h"
#include "Buffer.h"

namespace Engine
{
    class VertexArray
    {
        public:
            virtual ~VertexArray() {}

            virtual void Bind() const = 0;
            virtual void UnBind() const = 0;

            virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& buffer) = 0;
            virtual void AddIndexBuffer(const std::shared_ptr<IndexBuffer>& buffer) = 0;

            virtual const std::vector<std::shared_ptr<VertexBuffer> >& GetVertexBuffers() const = 0;
            virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const = 0;

            static VertexArray* Create();
    };
}