#define NOMINMAX

#include <Engine.h>

#include <future>
#include <thread>

#include "Octree/Octree.h"
#include "ply_io.h"

namespace Engine {

class ExampleLayer : public Layer {
   public:
    ExampleLayer() : Layer("Example") {
        // ------------ Octree -------- //

        m_Octree_future =
            std::make_shared<std::future<Ref<Octree> > >(std::async(
                std::launch::deferred,
                [](const std::string& name) {
                    return std::make_shared<Octree>(load_ply(name), 10);
                },
                "Assert/longdress_vox10_1300.ply"));

        auto octree = m_Octree_future->get();
        m_Octree = octree;
        Log::Trace("Octree built successfully.");

        // ------------ Game -------- //
        Log::Trace("Init game meta.");

        m_Camera = std::make_shared<PerspectiveCamera>(45.0f, 1.778f, 0.1f,
                                                       3000.0f * 8);
        m_Camera->SetPosition(m_Camera_Position);
        m_Camera->SetCameraTranslationSpeed(300.0f);
        m_Camera->SetCameraRotationSpeed(10.0f);

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

        for (int i = 0; i <= 10; ++i) {
            Ref<VertexArray> vertexArray;

            // ------------ OpenGL Box -------- //

            float box_vertices[36 * 5] = {
                -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,  //
                0.5f,  -0.5f, -0.5f, 1.0f, 0.0f,  //
                0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,  //
                0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,  //
                -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f,  //
                -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,  //

                -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,  //
                0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,  //
                0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  //
                0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  //
                -0.5f, 0.5f,  0.5f,  0.0f, 1.0f,  //
                -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,  //

                -0.5f, 0.5f,  0.5f,  1.0f, 0.0f,  //
                -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f,  //
                -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  //
                -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  //
                -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,  //
                -0.5f, 0.5f,  0.5f,  1.0f, 0.0f,  //

                0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  //
                0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,  //
                0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,  //
                0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,  //
                0.5f,  -0.5f, 0.5f,  0.0f, 0.0f,  //
                0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  //

                -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  //
                0.5f,  -0.5f, -0.5f, 1.0f, 1.0f,  //
                0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,  //
                0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,  //
                -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,  //
                -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  //

                -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f,  //
                0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,  //
                0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  //
                0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  //
                -0.5f, 0.5f,  0.5f,  0.0f, 0.0f,  //
                -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f,  //
            };

            Ref<VertexBuffer> box_VertexBuffer =
                VertexBuffer::Create(box_vertices, sizeof(box_vertices), 36);
            box_VertexBuffer->SetLayout({
                {0, ShaderDataType::Float3, "a_Position"},
                {1, ShaderDataType::Float2, "a_TexCoord"},
                //{2, ShaderDataType::Float3, "a_Offset"},
                //{3, ShaderDataType::Float4, "a_Color"},
            });

            vertexArray = VertexArray::Create();
            vertexArray->AddVertexBuffer(box_VertexBuffer, false);

            // ------------ OpenGL Octree -------- //

            std::vector<float> points;
            for (const Ref<OctreeNode>& p : octree->GetLevelNodes(i)) {
                points.push_back(float(p->GetX()));
                points.push_back(float(p->GetY()));
                points.push_back(float(p->GetZ()));
                points.push_back(float(p->GetR()) / 255.0);
                points.push_back(float(p->GetG()) / 255.0);
                points.push_back(float(p->GetB()) / 255.0);
                points.push_back(1.0f);
            }

            // points = std::vector<float>{0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0,
            // 0, 1};

            Ref<VertexBuffer> octree_VertexBuffer = VertexBuffer::Create(
                &points[0], sizeof(float) * points.size(), points.size() / 7.0);
            octree_VertexBuffer->SetLayout({
                //{0, ShaderDataType::Float3, "a_Position"},
                //{1, ShaderDataType::Float2, "a_TexCoord"},
                {2, ShaderDataType::Float3, "a_Offset"},
                {3, ShaderDataType::Float4, "a_Color"},
            });

            vertexArray->AddVertexBuffer(octree_VertexBuffer, true);

            m_Octree_VertexArrays.push_back(vertexArray);
        }

        // ---------------Shader--------------- //
        m_ShaderLibrary.Load("TextureShader", "Assert/vertex.glsl",
                             "Assert/fragment.glsl", "Path");
        m_ShaderLibrary.Load("BoxShader", "Assert/box_vertex.glsl",
                             "Assert/box_fragment.glsl", "Path");
        m_ShaderLibrary.Load("OctreeShader", "Assert/octree_vertex.glsl",
                             "Assert/octree_fragment.glsl", "Path");

        // ---------------Texture--------------- //
        m_Texture = Texture2D::Create("Assert/Checkerboard.png");
        m_Icons_Texture = Texture2D::Create("Assert/Icons.png");
    }

    void OnAttach() override {
        threadObj = std::thread([=]() {});
    }

    void OnDetach() override { threadObj.join(); }

    void OnUpdate() override {
        m_Camera->OnUpdate(m_LayerUpdateMeta.m_timeStep);

        // Render
        RenderCommand::SetClearColor(backGroundColor);
        RenderCommand::Clear();

        Renderer::BeginScene(m_Camera);

        m_Tree_Transform = glm::translate(glm::mat4(1.0f), m_Tree_Position);

        auto m_Shader = m_ShaderLibrary.Get("TextureShader");

        Renderer::SetShaderUniform(m_Shader, "u_Color",
                                   glm::vec4(0.9f, 0.7f, 0.9f, 1.0f));
        Renderer::SetShaderUniform(m_Shader, "u_Texture", 0);

        m_Icons_Texture->Bind(0);
        Renderer::Submit(m_Tree_VertexArray, m_Shader, m_Tree_Transform);
        m_Icons_Texture->UnBind(0);

        this->RenderOctree(m_Octree);

        Renderer::EndScene();
    }

    void OnImGuiRender() override {
        float timeStep = m_LayerUpdateMeta.m_timeStep;
        float nowTime = m_LayerUpdateMeta.m_nowTime;

        Gui::Begin("Octree Rendering.");
        Gui::Text("Time Step: {0}", timeStep);
        Gui::Text("Now Time: {0}", nowTime);

        // ImGui::ColorEdit4("backGroundColor",
        // glm::value_ptr(backGroundColor)); ImGui::SliderFloat("transSpeed",
        // &transSpeed, 0.0f, 6.0f); ImGui::SliderFloat("rotSpeed", &rotSpeed,
        // 0.0f, 6.0f);

        // ImGui::SliderFloat3("m_Tree_Position",
        // glm::value_ptr(m_Tree_Position),
        //                     -1.0f, 1.0f);

        ImGui::DragFloat3("m_Camera_Position",
                          glm::value_ptr(m_Camera_Position), -0.0f, 3000.0f);

        uint32_t nowLevel_min = 0;
        uint32_t nowLevel_max = 10;
        ImGui::SliderScalar("nowLevel", ImGuiDataType_U32, &nowLevel,
                            &nowLevel_min, &nowLevel_max);

        Gui::End();

        this->RenderOctreeGui(m_Octree);
    }

    void OnEvent(Event& event) override { m_Camera->OnEvent(event); }

    void RenderOctree(const Ref<Octree>& octree) {
        if (octree == nullptr) {
            return;
        } else {
            auto transform =
                glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));

            auto m_Shader = m_ShaderLibrary.Get("OctreeShader");

            Renderer::SetShaderUniform(m_Shader, "u_Color",
                                       glm::vec4(0.3f, 0.0f, 0.0f, 1.0f));

            float scale = (1 << (octree->GetMaxLevel() - nowLevel));
            Renderer::SetShaderUniform(m_Shader, "u_Scale", scale);

            Renderer::DrawArray(m_Octree_VertexArrays[nowLevel], m_Shader,
                                transform);
            return;
        }
    }

    void RenderOctreeGui(const Ref<Octree>& octree) {
        if (octree == nullptr) {
            ImGui::Begin("Octree");
            ImGui::Text("Loading Octree.");
            ImGui::End();
            return;
        } else {
            // threadObj.join();
            ImGui::Begin("Octree");
            ImGui::Text("NumMaxLevel: %d", octree->GetMaxLevel());
            ImGui::Text("NumNodes: %d", octree->GetNumNodes());
            ImGui::Text("NumLeafs: %d", octree->GetNumLeafs());

            for (int i = 0; i <= octree->GetMaxLevel(); ++i) {
                ImGui::Text("Laver %d has %d Nodes", i,
                            (int)octree->GetLevelNumNodes(i));
            }
            ImGui::End();
            return;
        }
    }

   private:
    Ref<Camera> m_Camera;
    glm::vec3 m_Camera_Position{224.0f, 453.0f, 729.0f};

    ShaderLibrary m_ShaderLibrary;
    Ref<Texture2D> m_Texture;
    Ref<Texture2D> m_Icons_Texture;

    Ref<VertexArray> m_Tree_VertexArray;
    glm::vec3 m_Tree_Position{0.0f, 0.0f, 0.0f};
    glm::mat4 m_Tree_Transform{1.0f};

    std::thread threadObj;
    Ref<std::future<Ref<Octree> > > m_Octree_future;
    Ref<Octree> m_Octree;

    std::vector<Ref<VertexArray> > m_Octree_VertexArrays;

   private:
    glm::vec4 backGroundColor{0.7f, 0.7f, 0.7f, 1.0f};

    uint32_t nowLevel = 10;
};

class Sandbox : public Application {
   public:
    Sandbox() : Application("OctreeExample", 1600, 900) {
        Log::Trace("Sandbox Initialization.");
        PushLayer(std::make_shared<ExampleLayer>());
        Log::Trace("Sandbox Initialization Success.");
    }

    ~Sandbox() { Log::Trace("Sandbox Shutdown."); }
};

Scope<Application> CreateApplication() { return std::make_unique<Sandbox>(); }
}  // namespace Engine
