#pragma once

#include "Buffer.h"
#include "Core.h"

namespace Engine
{
    class VertexArray
    {
    public:
        virtual ~VertexArray() {}

        virtual void Bind() const   = 0;
        virtual void UnBind() const = 0;

        virtual void AddVertexBuffer(const Ref<VertexBuffer>& buffer, const bool instance) = 0;
        virtual void AddIndexBuffer(const Ref<IndexBuffer>& buffer)                        = 0;

        virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const = 0;
        virtual const Ref<IndexBuffer>&               GetIndexBuffer() const   = 0;

        static Ref<VertexArray> Create();
    };
} // namespace Engine