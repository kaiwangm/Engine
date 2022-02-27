#include <Engine.h>

namespace Engine {
class Sandbox : public Application {
   public:
    Ref<Shader> m_Shader;

    Ref<VertexArray> m_Triangle_VertexArray;
    Ref<VertexArray> m_Square_VertexArray;

    Sandbox() {
        ENGINE_TRACE("Sandbox Initialization.");

        // ------------ OpenGL Triangle -------- //

        float Triangle_vertices[3 * 7] = {
            -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.5f, -0.5f, 0.0f, 0.0f,
            0.0f,  1.0f,  1.0f, 0.0f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f,  1.0f,
        };
        uint32_t Triangle_indices[3] = {0, 1, 2};

        // Set Vertex Buffer
        Ref<VertexBuffer> Triangle_VertexBuffer;
        Triangle_VertexBuffer.reset(
            VertexBuffer::Create(Triangle_vertices, sizeof(Triangle_vertices)));
        Triangle_VertexBuffer->SetLayout(
            {{ShaderDataType::Float3, "a_Position"},
             {ShaderDataType::Float4, "a_Color"}});

        // Set Index Buffer
        Ref<IndexBuffer> Triangle_IndexBuffer;
        Triangle_IndexBuffer.reset(IndexBuffer::Create(
            Triangle_indices, sizeof(Triangle_indices) / sizeof(uint32_t)));

        // Set Vertex Arrays
        m_Triangle_VertexArray.reset(VertexArray::Create());
        m_Triangle_VertexArray->AddVertexBuffer(Triangle_VertexBuffer);
        m_Triangle_VertexArray->AddIndexBuffer(Triangle_IndexBuffer);

        // ---------------Square--------------- //

        float Square_vertices[4 * 7] = {
            -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.5f, -0.5f, 0.0f,
            1.0f,  1.0f,  1.0f, 1.0f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f,  0.0f,
            1.0f,  -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        };
        uint32_t Square_indices[6] = {0, 1, 2, 2, 3, 0};

        // Set Vertex Buffer
        Ref<VertexBuffer> Square_VertexBuffer;
        Square_VertexBuffer.reset(
            VertexBuffer::Create(Square_vertices, sizeof(Square_vertices)));
        Square_VertexBuffer->SetLayout({{ShaderDataType::Float3, "a_Position"},
                                        {ShaderDataType::Float4, "a_Color"}});

        // Set Index Buffer
        Ref<IndexBuffer> Square_IndexBuffer;
        Square_IndexBuffer.reset(IndexBuffer::Create(
            Square_indices, sizeof(Square_indices) / sizeof(uint32_t)));

        // Set Vertex Arrays
        m_Square_VertexArray.reset(VertexArray::Create());
        m_Square_VertexArray->AddVertexBuffer(Square_VertexBuffer);
        m_Square_VertexArray->AddIndexBuffer(Square_IndexBuffer);

        // Set Shader
        std::string vertexSrc = R"(
                #version 330 core

                layout(location = 0) in vec3 a_Position;
                layout(location = 1) in vec4 a_Color;

                out vec3 v_Position;
                out vec4 v_Color;

                void main()
                {   
                    v_Position = a_Position;
                    v_Color = a_Color;
                    gl_Position = vec4(a_Position, 1.0);
                }
            )";

        std::string fragmentSrc = R"(
                #version 330 core

                layout(location = 0) out vec4 color;

                in vec3 v_Position;
                in vec4 v_Color;

                void main()
                {
                    //color = vec4(v_Position * 0.5 + 0.5, 1.0);
                    color = v_Color;
                }
            )";

        m_Shader.reset(new Shader(vertexSrc, fragmentSrc));

        ENGINE_TRACE("Sandbox Initialization Success.");
    }

    ~Sandbox() { ENGINE_TRACE("Sandbox Shutdown."); }

    void ClientDraw() override {
        RenderCommand::SetClearColor({0.7f, 0.7f, 0.7f, 1.0f});
        RenderCommand::Clear();

        Renderer::BeginScene();

        Renderer::Submit(m_Square_VertexArray, m_Shader);
        Renderer::Submit(m_Triangle_VertexArray, m_Shader);

        Renderer::EndScene();
    }
};

Scope<Application> CreateApplication() { return std::make_unique<Sandbox>(); }
}  // namespace Engine
