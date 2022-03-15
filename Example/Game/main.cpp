#define ENGINE_USE_ENTRYPOINT
#include <Engine.h>

namespace Engine {
class ExampleLayer : public Layer {
   public:
    ExampleLayer() : Layer("Example"), m_IsWindowFocused(false) {
        m_Camera = std::make_shared<PerspectiveCamera>(45.0f, 1.778f, 0.1f,
                                                       3000.0f * 8);
        m_Camera->SetPosition({0.0f, 0.0f, 3.0f});
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
            VertexBuffer::Create(Tree_vertices, sizeof(Tree_vertices), 4);
        Tree_VertexBuffer->SetLayout({
            {0, ShaderDataType::Float3, "a_Position"},
            {1, ShaderDataType::Float4, "a_Color"},
            {2, ShaderDataType::Float2, "a_TexCoord"},
        });

        // Set Index Buffer
        Ref<IndexBuffer> Tree_IndexBuffer = IndexBuffer::Create(
            Tree_indices, sizeof(Tree_indices) / sizeof(uint32_t));

        // Set Vertex Arrays
        m_Tree_VertexArray = VertexArray::Create();
        m_Tree_VertexArray->AddVertexBuffer(Tree_VertexBuffer, false);
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
            VertexBuffer::Create(Square_vertices, sizeof(Square_vertices), 4);
        Square_VertexBuffer->SetLayout({
            {0, ShaderDataType::Float3, "a_Position"},
            {1, ShaderDataType::Float4, "a_Color"},
            {2, ShaderDataType::Float2, "a_TexCoord"},
        });

        // Set Index Buffer
        Ref<IndexBuffer> Square_IndexBuffer = IndexBuffer::Create(
            Square_indices, sizeof(Square_indices) / sizeof(uint32_t));

        // Set Vertex Arrays
        m_Square_VertexArray = VertexArray::Create();
        m_Square_VertexArray->AddVertexBuffer(Square_VertexBuffer, false);
        m_Square_VertexArray->AddIndexBuffer(Square_IndexBuffer);

        // Set Shader
        m_ShaderLibrary.Load("TextureShader", "Assert/vertex.glsl",
                             "Assert/fragment.glsl", "Path");
        m_ShaderLibrary.Load("TextureShader_uv", "Assert/vertex_uv.glsl",
                             "Assert/fragment_uv.glsl", "Path");

        m_Texture = Texture2D::Create("Assert/Checkerboard.png");
        m_Icons_Texture = Texture2D::Create("Assert/Icons.png");

        // Set FrameRenderBuffer
        m_FrameRenderBuffer = FrameRenderBuffer::Create();
        m_FrameRenderBuffer_uv = FrameRenderBuffer::Create();
        // m_FrameRenderBuffer->SetViewPort(800, 600);

        m_Scene = std::make_shared<Scene>();

        auto& tree = Scene::CreateEntity(m_Scene, "tree");

        tree.AddComponent<TransformComponent>();
        tree.AddComponent<RenderableMeshComponent>(
            m_Tree_VertexArray, m_ShaderLibrary.Get("TextureShader"),
            m_Icons_Texture);
        tree.GetComponent<TransformComponent>().Translation = {0.0f, 0.0f,
                                                               0.3f};

        auto& square = Scene::CreateEntity(m_Scene, "square");

        square.AddComponent<TransformComponent>();
        square.AddComponent<RenderableMeshComponent>(
            m_Square_VertexArray, m_ShaderLibrary.Get("TextureShader"),
            m_Texture);
    }

    void OnAttach() override {}

    void OnDetach() override {}

    void OnUpdate() override {
        if (m_IsWindowFocused == true)
            m_Camera->OnUpdate(m_LayerUpdateMeta.m_timeStep);

        // Render
        Renderer::BeginScene(m_Camera, m_FrameRenderBuffer);

        RenderCommand::SetClearColor(backGroundColor);
        RenderCommand::Clear();

        m_Scene->OnUpdateRuntime(m_LayerUpdateMeta.m_timeStep);

        Renderer::EndScene(m_FrameRenderBuffer);

        // Render
        Renderer::BeginScene(m_Camera, m_FrameRenderBuffer_uv);

        RenderCommand::SetClearColor(backGroundColor);
        RenderCommand::Clear();

        m_Scene->OnUpdateRuntime(m_LayerUpdateMeta.m_timeStep);

        Renderer::EndScene(m_FrameRenderBuffer_uv);
    }

    void OnImGuiRender() override {
        float timeStep = m_LayerUpdateMeta.m_timeStep;
        float nowTime = m_LayerUpdateMeta.m_nowTime;

        m_Scene->OnUpdateRuntimeGui(timeStep, nowTime);

        // Gui::ColorEdit4("backGroundColor", backGroundColor);
        //  ImGui::SliderFloat("transSpeed", &transSpeed, 0.0f, 6.0f);
        //  ImGui::SliderFloat("rotSpeed", &rotSpeed, 0.0f, 6.0f);

        Gui::Begin("Camera");
        Gui::SliderFloat3("m_Camera_Position", m_Camera->GetPosition(), -10.0f,
                          10.0f);
        Gui::SliderFloat("m_Camera_Rotation", m_Camera->GetRotation(), -180.0f,
                         180.0f);

        Gui::End();

        Gui::ShowImNodesDemoWindow();
        ImGui::ShowDemoWindow();

        ImGui::Begin("ViewPort :: Color");
        {
            // Using a Child allow to fill all the space of the window.
            // It also alows customization
            ImGui::BeginChild("Render");
            m_IsWindowFocused = ImGui::IsWindowFocused();
            // Log::Trace("Focused: {0}", ImGui::IsWindowFocused());
            //  Get the size of the child (i.e. the whole draw size of the
            //  windows).
            ImVec2 wsize = ImGui::GetWindowSize();
            // Because I use the texture from OpenGL, I need to invert the V
            // from the UV.
            m_FrameRenderBuffer->SetViewPort((uint32_t)wsize.x,
                                             (uint32_t)wsize.y);
            ImGui::Image(m_FrameRenderBuffer->GetTextureID(), wsize,
                         ImVec2(0, 1), ImVec2(1, 0));
            ImGui::EndChild();
        }
        ImGui::End();

        ImGui::Begin("ViewPort :: UV");
        {
            // Using a Child allow to fill all the space of the window.
            // It also alows customization
            ImGui::BeginChild("Render");
            // Get the size of the child (i.e. the whole draw size of the
            // windows).
            ImVec2 wsize = ImGui::GetWindowSize();
            // Because I use the texture from OpenGL, I need to invert the V
            // from the UV.
            m_FrameRenderBuffer_uv->SetViewPort((uint32_t)wsize.x,
                                                (uint32_t)wsize.y);
            ImGui::Image(m_FrameRenderBuffer_uv->GetTextureID(), wsize,
                         ImVec2(0, 1), ImVec2(1, 0));

            ImGui::EndChild();
        }
        ImGui::End();

        ImGui::ShowExampleAppLog(NULL);
    }

    void OnEvent(Event& event) override { m_Camera->OnEvent(event); }

   private:
    Ref<Scene> m_Scene;
    Ref<Camera> m_Camera;
    ShaderLibrary m_ShaderLibrary;
    Ref<Texture2D> m_Texture;
    Ref<Texture2D> m_Icons_Texture;

    Ref<VertexArray> m_Tree_VertexArray;
    glm::vec3 m_Tree_Position{0.0f, 0.0f, 1.0f};
    glm::mat4 m_Tree_Transform{1.0f};

    Ref<VertexArray> m_Square_VertexArray;
    glm::vec3 m_Square_Position{0.0f, 0.0f, 0.0f};
    glm::mat4 m_Square_Transform{1.0f};

    Ref<FrameRenderBuffer> m_FrameRenderBuffer;
    Ref<FrameRenderBuffer> m_FrameRenderBuffer_uv;

   private:
    glm::vec4 backGroundColor{0.7f, 0.7f, 0.7f, 1.0f};
    bool m_IsWindowFocused;
};

class Sandbox : public Application {
   public:
    Sandbox() : Application("Sandbox", 2400, 1500) {
        Log::Trace("Sandbox Initialization.");
        PushLayer(std::make_shared<DockSpaceLayer>(m_Running));
        PushLayer(std::make_shared<ExampleLayer>());
        Log::Trace("Sandbox Initialization Success.");
    }

    ~Sandbox() { Log::Trace("Sandbox Shutdown."); }
};

Scope<Application> CreateApplication() { return std::make_unique<Sandbox>(); }
}  // namespace Engine
