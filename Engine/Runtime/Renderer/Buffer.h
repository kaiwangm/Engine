#pragma once
#include <Engine/Runtime/Core/Core.h>

namespace Engine
{
    enum class ShaderDataType
    {
        None = 0,
        Float,
        Float2,
        Float3,
        Float4,
        Mat3,
        Mat4,
        Int,
        Int2,
        Int3,
        Int4,
        Bool
    };

    static uint32_t ShaderDataTypeSize(ShaderDataType type)
    {
        switch (type)
        {
            case ShaderDataType::Float:
                return 4;
            case ShaderDataType::Float2:
                return 4 * 2;
            case ShaderDataType::Float3:
                return 4 * 3;
            case ShaderDataType::Float4:
                return 4 * 4;
            case ShaderDataType::Mat3:
                return 4 * 3 * 3;
            case ShaderDataType::Mat4:
                return 4 * 4 * 4;
            case ShaderDataType::Int:
                return 4;
            case ShaderDataType::Int2:
                return 4 * 2;
            case ShaderDataType::Int3:
                return 4 * 3;
            case ShaderDataType::Int4:
                return 4 * 4;
            case ShaderDataType::Bool:
                return 1;
            default:
                Log::Core_Error("Unknown ShaderDataType!");
                return -1;
        }

        return 0;
    }

    struct BufferElement
    {
        int32_t        Index;
        std::string    Name;
        ShaderDataType Type;
        uint32_t       Size;
        size_t         Offset;
        bool           Normalized;

        BufferElement() = default;

        BufferElement(const uint32_t index, ShaderDataType type, const std::string& name, bool normalized = false) :
            Index(index), Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalized)
        {}

        uint32_t GetComponentCount() const
        {
            switch (Type)
            {
                case ShaderDataType::Float:
                    return 1;
                case ShaderDataType::Float2:
                    return 2;
                case ShaderDataType::Float3:
                    return 3;
                case ShaderDataType::Float4:
                    return 4;
                case ShaderDataType::Mat3:
                    return 3; // 3* float3
                case ShaderDataType::Mat4:
                    return 4; // 4* float4
                case ShaderDataType::Int:
                    return 1;
                case ShaderDataType::Int2:
                    return 2;
                case ShaderDataType::Int3:
                    return 3;
                case ShaderDataType::Int4:
                    return 4;
                case ShaderDataType::Bool:
                    return 1;
                default:
                    Log::Core_Error("Unknown ComponentCount!");
                    return -1;
            }

            return 0;
        }

        ShaderDataType GetGLType() const { return Type; }

        bool GetNormalized() const { return Normalized; }

        uint32_t    GetIndex() const { return Index; }
        const void* GetOffset() const { return (const void*)(Offset); }
    };

    class BufferLayout
    {
    public:
        BufferLayout() {}

        BufferLayout(std::initializer_list<BufferElement> elements) : m_Elements(elements)
        {
            CalculateOffsetsAndStride();
        }

        uint32_t                          GetStride() const { return m_Stride; }
        const std::vector<BufferElement>& GetElements() const { return m_Elements; }

        std::vector<BufferElement>::iterator       begin() { return m_Elements.begin(); }
        std::vector<BufferElement>::iterator       end() { return m_Elements.end(); }
        std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
        std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }

    private:
        void CalculateOffsetsAndStride()
        {
            size_t offset = 0;
            m_Stride      = 0;
            for (auto& element : m_Elements)
            {
                element.Offset = offset;
                offset += element.Size;
                m_Stride += element.Size;
            }
        }

    private:
        std::vector<BufferElement> m_Elements;
        uint32_t                   m_Stride = 0;
    };

    class VertexBuffer
    {
    public:
        VertexBuffer(uint32_t count) : m_Count(count) {}
        virtual ~VertexBuffer() = default;

        virtual void Bind() const   = 0;
        virtual void UnBind() const = 0;

        virtual void                SetLayout(const BufferLayout& layout) = 0;
        virtual const BufferLayout& GetLayout() const                     = 0;

        static Ref<VertexBuffer> Create(const void* vertices, uint32_t size, uint32_t count);

        uint32_t GetCount() { return m_Count; }

    protected:
        uint32_t     m_Count;
        BufferLayout layout;
    };

    class IndexBuffer
    {
    public:
        virtual ~IndexBuffer() {}

        virtual void Bind() const   = 0;
        virtual void UnBind() const = 0;

        virtual uint32_t GetCount() const = 0;

        static Ref<IndexBuffer> Create(const uint32_t* indices, uint32_t count);
    };

    class FrameRenderBuffer
    {
    public:
        virtual ~FrameRenderBuffer() {}
        virtual void Bind() const   = 0;
        virtual void UnBind() const = 0;

        virtual void  SetViewPort(uint32_t width, uint32_t height) = 0;
        virtual void* GetTextureID() const                         = 0;

        virtual uint32_t GetWidth() const  = 0;
        virtual uint32_t GetHeight() const = 0;

        static Ref<FrameRenderBuffer> Create();
    };

    class GeometryBuffer
    {
    public:
        virtual ~GeometryBuffer() {}
        virtual void Bind() const   = 0;
        virtual void UnBind() const = 0;

        virtual void SetViewPort(uint32_t width, uint32_t height) = 0;

        virtual uint32_t GetWidth() const             = 0;
        virtual uint32_t GetHeight() const            = 0;
        virtual void*    GetPositionTextureID() const = 0;
        virtual void*    GetNormalTextureID() const   = 0;
        virtual void*    GetAlbedoTextureID() const   = 0;
        virtual void*    GetOpacityTextureID() const  = 0;
        virtual void*    GetDepthTextureID() const    = 0;

        virtual void BindPositionTexture(const uint32_t& slot) const  = 0;
        virtual void BindNormalTexture(const uint32_t& slot) const    = 0;
        virtual void BindAlbedoTexture(const uint32_t& slot) const    = 0;
        virtual void BindOpacityTexture(const uint32_t& slot) const   = 0;
        virtual void BindDepthTexture(const uint32_t& slot) const     = 0;
        virtual void UnBindTexture(const uint32_t& slot) const = 0;

        static Ref<GeometryBuffer> Create();
    };
} // namespace Engine