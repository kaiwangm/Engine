#include <Engine.h>

namespace Engine {

class ExampleLayer : public Layer {
   public:
    ExampleLayer() : Layer("Example") {
        m_Camera.reset(new OrthographicCamera(-1.6f, 1.6f, -0.9f, 0.9f));
        // ------------ OpenGL Triangle -------- //

        float Triangle_vertices[3 * 9] = {
            -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,  //
            0.5f,  -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f,  //
            0.0f,  0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.5f, 1.0f,  //
        };
        uint32_t Triangle_indices[3] = {0, 1, 2};

        // Set Vertex Buffer
        Ref<VertexBuffer> Triangle_VertexBuffer =
            VertexBuffer::Create(Triangle_vertices, sizeof(Triangle_vertices));
        Triangle_VertexBuffer->SetLayout({
            {ShaderDataType::Float3, "a_Position"},
            {ShaderDataType::Float4, "a_Color"},
            {ShaderDataType::Float2, "a_TexCoord"},
        });

        // Set Index Buffer
        Ref<IndexBuffer> Triangle_IndexBuffer = IndexBuffer::Create(
            Triangle_indices, sizeof(Triangle_indices) / sizeof(uint32_t));

        // Set Vertex Arrays
        m_Triangle_VertexArray = VertexArray::Create();
        m_Triangle_VertexArray->AddVertexBuffer(Triangle_VertexBuffer);
        m_Triangle_VertexArray->AddIndexBuffer(Triangle_IndexBuffer);

        // ---------------Square--------------- //

        float Square_vertices[4 * 9] = {
            -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,  //
            0.5f,  -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,  //
            0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,  //
            -0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,  //
        };
        uint32_t Square_indices[6] = {0, 1, 2, 2, 3, 0};

        // Set Vertex Buffer
        Ref<VertexBuffer> Square_VertexBuffer =
            VertexBuffer::Create(Square_vertices, sizeof(Square_vertices));
        Square_VertexBuffer->SetLayout({
            {ShaderDataType::Float3, "a_Position"},
            {ShaderDataType::Float4, "a_Color"},
            {ShaderDataType::Float2, "a_TexCoord"},
        });

        // Set Index Buffer
        Ref<IndexBuffer> Square_IndexBuffer = IndexBuffer::Create(
            Square_indices, sizeof(Square_indices) / sizeof(uint32_t));

        // Set Vertex Arrays
        m_Square_VertexArray = VertexArray::Create();
        m_Square_VertexArray->AddVertexBuffer(Square_VertexBuffer);
        m_Square_VertexArray->AddIndexBuffer(Square_IndexBuffer);

        // Set Shader
        std::string vertexSrc = R"(
                #version 330 core

                layout(location = 0) in vec3 a_Position;
                layout(location = 1) in vec4 a_Color;
                layout(location = 2) in vec2 a_TexCoord;

                uniform mat4 u_ViewProjection;
                uniform mat4 u_Transform;

                out vec4 v_Color;
                out vec2 v_TexCoord;

                void main()
                {   
                    v_Color = a_Color;
                    v_TexCoord = a_TexCoord;
                    gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0f);
                }
            )";

        std::string fragmentSrc = R"(
                #version 330 core

                layout(location = 0) out vec4 color;

                in vec4 v_Color;
                in vec2 v_TexCoord;

                uniform vec4 u_Color;

                uniform sampler2D u_Texture;

                void main()
                {
                    color = texture(u_Texture, v_TexCoord);
                }
            )";

        m_Shader = Shader::Create(vertexSrc, fragmentSrc);

        m_Texture = Texture2D::Create("Assert/Checkerboard.png");
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

        m_Triangle_Transform =
            glm::translate(glm::mat4(1.0f), m_Triangle_Position);
        m_Square_Transform = glm::translate(glm::mat4(1.0f), m_Square_Position);

        Renderer::SetShaderUniform(m_Shader, "u_Color",
                                   glm::vec4(0.9f, 0.9f, 0.9f, 1.0f));
        Renderer::SetShaderUniform(m_Shader, "u_Texture", 0);

        m_Texture->Bind(0);
        Renderer::Submit(m_Square_VertexArray, m_Shader, m_Square_Transform);
        m_Texture->UnBind(0);

        Renderer::SetShaderUniform(m_Shader, "u_Color",
                                   glm::vec4(0.9f, 0.7f, 0.9f, 1.0f));
        Renderer::SetShaderUniform(m_Shader, "u_Texture", 0);

        m_Texture->Bind(0);
        Renderer::Submit(m_Triangle_VertexArray, m_Shader,
                         m_Triangle_Transform);
        m_Texture->UnBind(0);

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

        ImGui::SliderFloat3("m_Triangle_Position",
                            glm::value_ptr(m_Triangle_Position), -1.0f, 1.0f);
        ImGui::SliderFloat3("m_Square_Position",
                            glm::value_ptr(m_Square_Position), -1.0f, 1.0f);

        ImGui::End();
    }

    void OnEvent(Event& event) override {}

   private:
    Ref<OrthographicCamera> m_Camera;
    Ref<Shader> m_Shader;
    Ref<Texture2D> m_Texture;

    Ref<VertexArray> m_Triangle_VertexArray;
    glm::vec3 m_Triangle_Position{0.0f, 0.0f, 0.0f};
    glm::mat4 m_Triangle_Transform{1.0f};

    Ref<VertexArray> m_Square_VertexArray;
    glm::vec3 m_Square_Position{0.0f, 0.0f, 0.0f};
    glm::mat4 m_Square_Transform{1.0f};

   private:
    glm::vec4 backGroundColor{0.7f, 0.7f, 0.7f, 1.0f};
    float transSpeed = 3.0;
    float rotSpeed = 3.0;
};

class Sandbox : public Application {
   public:
    Sandbox() {
        ENGINE_TRACE("Sandbox Initialization.");
        PushLayer(std::make_shared<ExampleLayer>());
        ENGINE_TRACE("Sandbox Initialization Success.");
    }

    ~Sandbox() { ENGINE_TRACE("Sandbox Shutdown."); }
};

Scope<Application> CreateApplication() { return std::make_unique<Sandbox>(); }
}  // namespace Engine
