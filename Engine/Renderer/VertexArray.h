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

            virtual void AddVertexBuffer(const Ref<VertexBuffer>& buffer) = 0;
            virtual void AddIndexBuffer(const Ref<IndexBuffer>& buffer) = 0;

            virtual const std::vector<Ref<VertexBuffer> >& GetVertexBuffers() const = 0;
            virtual const Ref<IndexBuffer>& GetIndexBuffer() const = 0;

            static VertexArray* Create();
    };
}