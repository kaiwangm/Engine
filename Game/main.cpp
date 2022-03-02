#include <Engine.h>

namespace Engine {

class ExampleLayer : public Layer {
   public:
    ExampleLayer() : Layer("Example") {
        m_Camera.reset(new OrthographicCamera(-1.6f, 1.6f, -0.9f, 0.9f));
        // ------------ OpenGL Tree -------- //

        float Tree_vertices[4 * 9] = {
            -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,  //
            0.5f,  -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,  //
            0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,  //
            -0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,  //
        };
        uint32_t Tree_indices[6] = {0, 1, 2, 2, 3, 0};

        // Set Vertex Buffer
        Ref<VertexBuffer> Tree_VertexBuffer =
            VertexBuffer::Create(Tree_vertices, sizeof(Tree_vertices));
        Tree_VertexBuffer->SetLayout({
            {ShaderDataType::Float3, "a_Position"},
            {ShaderDataType::Float4, "a_Color"},
            {ShaderDataType::Float2, "a_TexCoord"},
        });

        // Set Index Buffer
        Ref<IndexBuffer> Tree_IndexBuffer = IndexBuffer::Create(
            Tree_indices, sizeof(Tree_indices) / sizeof(uint32_t));

        // Set Vertex Arrays
        m_Tree_VertexArray = VertexArray::Create();
        m_Tree_VertexArray->AddVertexBuffer(Tree_VertexBuffer);
        m_Tree_VertexArray->AddIndexBuffer(Tree_IndexBuffer);

        // ---------------Square--------------- //

        float Square_vertices[4 * 9] = {
            -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,  //
            0.5f,  -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,  //
            0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,  //
            -0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,  //
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
        shaderLibrary.Load("TextureShader", "Assert/vertex.glsl",
                           "Assert/fragment.glsl", "Path");

        m_Texture = Texture2D::Create("Assert/Checkerboard.png");
        m_Icons_Texture = Texture2D::Create("Assert/Icons.png");
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

        m_Tree_Transform = glm::translate(glm::mat4(1.0f), m_Tree_Position);
        m_Square_Transform = glm::translate(glm::mat4(1.0f), m_Square_Position);

        auto m_Shader = shaderLibrary.Get("TextureShader");
        
        Renderer::SetShaderUniform(m_Shader, "u_Color",
                                   glm::vec4(0.9f, 0.9f, 0.9f, 1.0f));
        Renderer::SetShaderUniform(m_Shader, "u_Texture", 0);

        m_Texture->Bind(0);
        Renderer::Submit(m_Square_VertexArray, m_Shader, m_Square_Transform);
        m_Texture->UnBind(0);

        Renderer::SetShaderUniform(m_Shader, "u_Color",
                                   glm::vec4(0.9f, 0.7f, 0.9f, 1.0f));
        Renderer::SetShaderUniform(m_Shader, "u_Texture", 0);

        m_Icons_Texture->Bind(0);
        Renderer::Submit(m_Tree_VertexArray, m_Shader, m_Tree_Transform);
        m_Icons_Texture->UnBind(0);

        Renderer::EndScene();
    }

    void OnImGuiRender() override {
        float timeStep = m_LayerUpdateMeta.m_timeStep;
        float nowTime = m_LayerUpdateMeta.m_nowTime;

        ImGui::Begin("Example Layer");
        ImGui::Text("Hello, a Tree and a Square !");
        ImGui::Text("Time Step: %f", timeStep);
        ImGui::Text("Now Time: %f", nowTime);

        ImGui::ColorEdit4("backGroundColor", glm::value_ptr(backGroundColor));
        ImGui::SliderFloat("transSpeed", &transSpeed, 0.0f, 6.0f);
        ImGui::SliderFloat("rotSpeed", &rotSpeed, 0.0f, 6.0f);

        ImGui::SliderFloat3("m_Tree_Position", glm::value_ptr(m_Tree_Position),
                            -1.0f, 1.0f);
        ImGui::SliderFloat3("m_Square_Position",
                            glm::value_ptr(m_Square_Position), -1.0f, 1.0f);

        ImGui::End();
    }

    void OnEvent(Event& event) override {}

   private:
    Ref<OrthographicCamera> m_Camera;
    ShaderLibrary shaderLibrary;
    Ref<Texture2D> m_Texture;
    Ref<Texture2D> m_Icons_Texture;

    Ref<VertexArray> m_Tree_VertexArray;
    glm::vec3 m_Tree_Position{0.0f, 0.0f, 0.0f};
    glm::mat4 m_Tree_Transform{1.0f};

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
