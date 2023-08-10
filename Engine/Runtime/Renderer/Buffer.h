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
        VertexBuffer() {}
        VertexBuffer(uint32_t count) {}
        virtual ~VertexBuffer() = default;

        virtual void Bind() const   = 0;
        virtual void UnBind() const = 0;

        virtual void                SetLayout(const BufferLayout& layout) = 0;
        virtual const BufferLayout& GetLayout() const                     = 0;

        static Ref<VertexBuffer> Create();
        static Ref<VertexBuffer> Create(const void* vertices, uint32_t size, uint32_t count);

    protected:
        BufferLayout layout;
    };

    class IndexBuffer
    {
    public:
        virtual ~IndexBuffer() {}

        virtual void Bind() const   = 0;
        virtual void UnBind() const = 0;

        virtual uint32_t GetCount() const         = 0;
        virtual void     SetCount(uint32_t count) = 0;

        static Ref<IndexBuffer> Create();
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

        virtual void BindTexture(const uint32_t& slot) const   = 0;
        virtual void UnBindTexture(const uint32_t& slot) const = 0;

        static Ref<FrameRenderBuffer> Create();
    };

    class ShadowMapBuffer
    {
    public:
        virtual ~ShadowMapBuffer() {}
        virtual void Bind() const   = 0;
        virtual void UnBind() const = 0;

        virtual void  SetViewPort(uint32_t width, uint32_t height) = 0;
        virtual void* GetTextureID() const                         = 0;

        virtual uint32_t GetWidth() const  = 0;
        virtual uint32_t GetHeight() const = 0;

        virtual void BindTexture(const uint32_t& slot) const   = 0;
        virtual void UnBindTexture(const uint32_t& slot) const = 0;

        static Ref<ShadowMapBuffer> Create();
    };

    class ShadowCubeMapBuffer
    {
    public:
        virtual ~ShadowCubeMapBuffer() {}
        virtual void BindTop() const    = 0;
        virtual void BindBottom() const = 0;
        virtual void BindLeft() const   = 0;
        virtual void BindRight() const  = 0;
        virtual void BindFront() const  = 0;
        virtual void BindBack() const   = 0;
        virtual void UnBind() const     = 0;

        virtual void  SetViewPort(uint32_t width, uint32_t height) = 0;
        virtual void* GetTextureID() const                         = 0;

        virtual uint32_t GetWidth() const  = 0;
        virtual uint32_t GetHeight() const = 0;

        virtual void BindTexture(const uint32_t& slot) const   = 0;
        virtual void UnBindTexture(const uint32_t& slot) const = 0;

        static Ref<ShadowCubeMapBuffer> Create();
    };

    class GeometryBuffer
    {
    public:
        virtual ~GeometryBuffer() {}
        virtual void Bind() const   = 0;
        virtual void UnBind() const = 0;

        virtual void SetViewPort(uint32_t width, uint32_t height) = 0;

        virtual uint32_t GetWidth() const                  = 0;
        virtual uint32_t GetHeight() const                 = 0;
        virtual void*    GetViewPositionTextureID() const  = 0;
        virtual void*    GetViewNormalTextureID() const    = 0;
        virtual void*    GetDiffuseTextureID() const        = 0;
        virtual void*    GetDepthTextureID() const         = 0;
        virtual void*    GetRoughnessTextureID() const     = 0;
        virtual void*    GetSpecularTextureID() const      = 0;
        virtual void*    GetWorldPositionTextureID() const = 0;
        virtual void*    GetWorldNormalTextureID() const   = 0;
        virtual void*    GetEmissiveTextureID() const      = 0;

        virtual void BindViewPositionTexture(const uint32_t& slot) const  = 0;
        virtual void BindViewNormalTexture(const uint32_t& slot) const    = 0;
        virtual void BindDiffuseTexture(const uint32_t& slot) const        = 0;
        virtual void BindDepthTexture(const uint32_t& slot) const         = 0;
        virtual void BindRoughnessTexture(const uint32_t& slot) const     = 0;
        virtual void BindSpecularTexture(const uint32_t& slot) const      = 0;
        virtual void BindWorldPositionTexture(const uint32_t& slot) const = 0;
        virtual void BindWorldNormalTexture(const uint32_t& slot) const   = 0;
        virtual void BindEmissiveTexture(const uint32_t& slot) const      = 0;
        virtual void UnBindTexture(const uint32_t& slot) const            = 0;

        static Ref<GeometryBuffer> Create();
    };

    class SSAOBuffer
    {
    public:
        virtual ~SSAOBuffer() {}
        virtual void Bind() const   = 0;
        virtual void UnBind() const = 0;

        virtual void SetViewPort(uint32_t width, uint32_t height) = 0;

        virtual uint32_t GetWidth() const  = 0;
        virtual uint32_t GetHeight() const = 0;
        virtual float&   GetRadiusRef()    = 0;
        virtual float&   GetBiasRef()      = 0;
        virtual float&   GetPowerRef()     = 0;

        virtual void* GetSSAOTextureID() const  = 0;
        virtual void* GetNoiseTextureID() const = 0;

        virtual void BindSSAOTexture(const uint32_t& slot) const  = 0;
        virtual void BindNoiseTexture(const uint32_t& slot) const = 0;
        virtual void UnBindTexture(const uint32_t& slot) const    = 0;

        virtual std::vector<glm::vec3>& GetSSAOKernel() = 0;

        static Ref<SSAOBuffer> Create();
    };
} // namespace Engine