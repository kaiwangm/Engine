#include <Engine.h>

namespace Engine {

class ExampleLayer : public Layer {
   public:
    ExampleLayer() : Layer("Example") {
        m_Camera.reset(new OrthographicCamera(-1.6f, 1.6f, -0.9f, 0.9f));
        // ------------ OpenGL Triangle -------- //

        float Triangle_vertices[3 * 7] = {
            -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,  //
            0.5f,  -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,  //
            0.0f,  0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 1.0f,  //
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
            -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,  //
            0.5f,  -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,  //
            0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,  //
            -0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 1.0f, 1.0f,  //
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

                uniform mat4 u_ViewProjection;

                out vec3 v_Position;
                out vec4 v_Color;

                void main()
                {   
                    v_Position = a_Position;
                    v_Color = a_Color;
                    gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
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
    }

    void OnAttach() override {}

    void OnDetach() override {}
    
    void OnUpdate() override {
        // Input
        float timeStep = m_LayerUpdateMeta.m_timeStep;
        auto camera_pos = m_Camera->GetPosition();
        auto camera_rot = m_Camera->GetRotation();

        if (Input::IsKeyPressed(GLFW_KEY_A)) {
            camera_pos.x -= transSpeed * timeStep;
        }
        if (Input::IsKeyPressed(GLFW_KEY_D)) {
            camera_pos.x += transSpeed * timeStep;
        }
        if (Input::IsKeyPressed(GLFW_KEY_S)) {
            camera_pos.y -= transSpeed * timeStep;
        }
        if (Input::IsKeyPressed(GLFW_KEY_W)) {
            camera_pos.y += transSpeed * timeStep;
        }
        if (Input::IsKeyPressed(GLFW_KEY_E)) {
            camera_rot -= rotSpeed * timeStep;
        }
        if (Input::IsKeyPressed(GLFW_KEY_Q)) {
            camera_rot += rotSpeed * timeStep;
        }

        m_Camera->SetRotation(camera_rot);
        m_Camera->SetPosition(camera_pos);

        // Render
        RenderCommand::SetClearColor(backGroundColor);
        RenderCommand::Clear();

        Renderer::BeginScene(m_Camera);

        Renderer::Submit(m_Square_VertexArray, m_Shader);
        Renderer::Submit(m_Triangle_VertexArray, m_Shader);

        Renderer::EndScene();
    }

    void OnImGuiRender() override {
        float timeStep = m_LayerUpdateMeta.m_timeStep;
        float nowTime = m_LayerUpdateMeta.m_nowTime;

        ImGui::Begin("Example Layer");
        ImGui::Text("Hello, a Triangle and a Square !");
        ImGui::Text("Time Step: %f", timeStep);
        ImGui::Text("Now Time: %f", nowTime);

        ImGui::ColorEdit4("backGroundColor", glm::value_ptr(backGroundColor));
        ImGui::SliderFloat("transSpeed", &transSpeed, 0.0f, 6.0f);
        ImGui::SliderFloat("rotSpeed", &rotSpeed, 0.0f, 6.0f);

        ImGui::End();
    }

    void OnEvent(Event& event) override {}

   private:
    Ref<OrthographicCamera> m_Camera;
    Ref<Shader> m_Shader;

    Ref<VertexArray> m_Triangle_VertexArray;
    Ref<VertexArray> m_Square_VertexArray;

   private:
    glm::vec4 backGroundColor{0.7f, 0.7f, 0.7f, 1.0f};
    float transSpeed = 3.0;
    float rotSpeed = 3.0;
};

class Sandbox : public Application {
   public:
    Sandbox() {
        ENGINE_TRACE("Sandbox Initialization.");
        PushLayer(new ExampleLayer());
        ENGINE_TRACE("Sandbox Initialization Success.");
    }

    ~Sandbox() { ENGINE_TRACE("Sandbox Shutdown."); }
};

Scope<Application> CreateApplication() { return std::make_unique<Sandbox>(); }
}  // namespace Engine
