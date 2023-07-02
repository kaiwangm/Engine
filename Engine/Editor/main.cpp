#define ENGINE_USE_ENTRYPOINT
#include <Engine/Runtime/Engine.h>

namespace Engine
{
    class EditorLayer : public Layer
    {
    public:
        EditorLayer() : Layer("EditorLayer")
        {
            m_World = std::make_shared<UWorld>();

            m_World->LoadShader("TextureShader", "Assert/Shader/vertex.glsl", "Assert/Shader/fragment.glsl", "Path");
            m_World->LoadShader("TextureShader_normal",
                                "Assert/Shader/vertex_normal.glsl",
                                "Assert/Shader/fragment_normal.glsl",
                                "Path");
            m_World->LoadShader(
                "Animated", "Assert/Shader/vertex_animated.glsl", "Assert/Shader/fragment_animated.glsl", "Path");
            m_World->LoadShader(
                "Skybox", "Assert/Shader/vertex_skybox.glsl", "Assert/Shader/fragment_skybox.glsl", "Path");
            m_World->LoadShader(
                "BasicPbr", "Assert/Shader/vertex_basicpbr.glsl", "Assert/Shader/fragment_basicpbr.glsl", "Path");
            m_World->LoadShader(
                "OctreeShader", "Assert/Shader/octree_vertex.glsl", "Assert/Shader/octree_fragment.glsl", "Path");
            m_World->LoadShader(
                "TriangleShader", "Assert/Shader/triangle_vertex.glsl", "Assert/Shader/triangle_fragment.glsl", "Path");
            m_World->LoadShader(
                "Skeleton", "Assert/Shader/skeleton_vertex.glsl", "Assert/Shader/skeleton_fragment.glsl", "Path");
            m_World->LoadShader(
                "GBuffer", "Assert/Shader/gbuffer_vertex.glsl", "Assert/Shader/gbuffer_fragment.glsl", "Path");

            m_World->LoadShader("ViewGBufferPosition",
                                "Assert/Shader/screen_quad_vertex.glsl",
                                "Assert/Shader/viewport/position.glsl",
                                "Path");

            m_World->LoadShader("ViewGBufferNormal",
                                "Assert/Shader/screen_quad_vertex.glsl",
                                "Assert/Shader/viewport/normal.glsl",
                                "Path");

            m_World->LoadShader("ViewGBufferAlbedo",
                                "Assert/Shader/screen_quad_vertex.glsl",
                                "Assert/Shader/viewport/albedo.glsl",
                                "Path");

            m_World->LoadShader("ViewGBufferOpacity",
                                "Assert/Shader/screen_quad_vertex.glsl",
                                "Assert/Shader/viewport/opacity.glsl",
                                "Path");

            m_World->LoadShader("ViewGBufferDepth",
                                "Assert/Shader/screen_quad_vertex.glsl",
                                "Assert/Shader/viewport/depth.glsl",
                                "Path");

            m_World->LoadShader("ViewAO",
                                "Assert/Shader/screen_quad_vertex.glsl",
                                "Assert/Shader/viewport/ao.glsl",
                                "Path");

            m_World->LoadShader("ComputeAO",
                                "Assert/Shader/screen_quad_vertex.glsl",
                                "Assert/Shader/compute_ssao_fragment.glsl",
                                "Path");
            
            m_World->LoadShader("ViewGBufferRoughness",
                                "Assert/Shader/screen_quad_vertex.glsl",
                                "Assert/Shader/viewport/roughness.glsl",
                                "Path");

            m_World->LoadShader("ViewGBufferMetallic",
                                "Assert/Shader/screen_quad_vertex.glsl",
                                "Assert/Shader/viewport/metallic.glsl",
                                "Path");

            m_Camera    = std::make_shared<PerspectiveCamera>(60.0f, 1.778f, 0.1f, 800.0f);
            auto camera = m_World->AddActor<ACamera>("main camera", m_Camera);
            camera.GetTransformComponent().SetPosition(glm::vec3 {0.339f, 3.711f, 8.815f});
            camera.GetTransformComponent().SetRotation(glm::vec3 {-0.088f, -6.732f, 0.000f});
            camera.GetTransformComponent().SetScale(glm::vec3 {1.000f, 1.000f, 1.000f});

            auto skybox =
                m_World->AddActor<ASkybox>("skybox", "Assert/Skybox/TheSkyIsOnFire/the_sky_is_on_fire_8k.hdr");

            // auto board = m_World->AddActor<AStaticMesh>("board");
            // board.GetTransformComponent().SetPosition(glm::vec3 {-2.350f, 2.165f, 0.000f});

            // auto gallery = m_World->AddActor<AStaticMesh>("gallery", "Assert/Object/gallery/gallery.obj");

            // auto animan = m_World->AddActor<AAnimatedMesh>("animan", "Assert/Object/animan/model.dae");

            // animan.GetTransformComponent().SetPosition(glm::vec3 {1.655f, 0.685f, 0.120f});
            // animan.GetTransformComponent().SetRotation(glm::vec3 {-1.330f, 0.000f, 0.000f});
            // animan.GetTransformComponent().SetScale(glm::vec3 {0.300f, 0.300f, 0.300f});

            m_World->AddActor<AActor>("Actor");

            auto red_triangle = m_World->AddActor<AStaticMesh>("red_triangle", //
                                                               "Assert/Object/triangle/triangle.obj",
                                                               "TriangleShader",
                                                               "basic_red_triangle",
                                                               "basic_red_triangle");
            red_triangle.GetTransformComponent().SetPosition(glm::vec3 {1.0f, 5.5f, 0.0f});

            auto red_sphere = m_World->AddActor<AStaticMesh>("red_sphere", //
                                                             "Assert/Object/sphere/sphere.obj",
                                                             "BasicPbr",
                                                             "basic_red_sphere",
                                                             "basic_red_sphere");
            red_sphere.GetTransformComponent().SetPosition(glm::vec3 {1.0f, 1.5f, 0.0f});
            MBasicPbr* red_sphere_material = static_cast<MBasicPbr*>(red_sphere.GetMaterial());
            red_sphere_material->SetAlbedo(glm::vec3 {1.0f, 0.0f, 0.0f});
            red_sphere_material->SetMetallic(0.3f);
            red_sphere_material->SetRoughness(0.5f);

            auto brown_sphere = m_World->AddActor<AStaticMesh>("brown_sphere", //
                                                               "Assert/Object/sphere/sphere.obj",
                                                               "BasicPbr",
                                                               "basic_brown_sphere",
                                                               "basic_brown_sphere");
            brown_sphere.GetTransformComponent().SetPosition(glm::vec3 {3.0f, 1.5f, 0.0f});
            MBasicPbr* brown_sphere_material = static_cast<MBasicPbr*>(brown_sphere.GetMaterial());
            brown_sphere_material->SetAlbedo(glm::vec3 {0.77f, 0.37f, 0.12f});
            brown_sphere_material->SetMetallic(0.2f);
            brown_sphere_material->SetRoughness(0.8f);

            auto aqua_shpere = m_World->AddActor<AStaticMesh>("aqua_sphere", //
                                                              "Assert/Object/sphere/sphere.obj",
                                                              "BasicPbr",
                                                              "basic_aqua_sphere",
                                                              "basic_aqua_sphere");
            aqua_shpere.GetTransformComponent().SetPosition(glm::vec3 {5.0f, 1.5f, 0.0f});
            MBasicPbr* aqua_shpere_material = static_cast<MBasicPbr*>(aqua_shpere.GetMaterial());
            aqua_shpere_material->SetAlbedo(glm::vec3 {0.0f, 1.0f, 1.0f});
            aqua_shpere_material->SetMetallic(0.9f);
            aqua_shpere_material->SetRoughness(0.3f);

            auto wood_sphere = m_World->AddActor<AStaticMesh>("wood_sphere", //
                                                              "Assert/Object/sphere/sphere.obj",
                                                              "Texture",
                                                              "bamboo-wood-semigloss_sphere",
                                                              "bamboo-wood-semigloss");
            wood_sphere.GetTransformComponent().SetPosition(glm::vec3 {1.0f, 3.5f, 0.0f});

            auto metal_sphere = m_World->AddActor<AStaticMesh>("metal_sphere", //
                                                               "Assert/Object/sphere/sphere.obj",
                                                               "Texture",
                                                               "streaky-metal1_sphere",
                                                               "streaky-metal1");
            metal_sphere.GetTransformComponent().SetPosition(glm::vec3 {3.0f, 3.5f, 0.0f});

            auto plastic_sphere = m_World->AddActor<AStaticMesh>("plastic_sphere", //
                                                                 "Assert/Object/sphere/sphere.obj",
                                                                 "Texture",
                                                                 "scuffed-plastic_sphere",
                                                                 "scuffed-plastic");
            plastic_sphere.GetTransformComponent().SetPosition(glm::vec3 {5.0f, 3.5f, 0.0f});

            auto basetile_sphere = m_World->AddActor<AStaticMesh>("basetile_sphere", //
                                                                  "Assert/Object/sphere/sphere.obj",
                                                                  "Texture",
                                                                  "base-white-tile_sphere",
                                                                  "base-white-tile");
            basetile_sphere.GetTransformComponent().SetPosition(glm::vec3 {3.0f, 5.5f, 0.0f});

            auto blacktile_sphere = m_World->AddActor<AStaticMesh>("blacktile_sphere", //
                                                                   "Assert/Object/sphere/sphere.obj",
                                                                   "Texture",
                                                                   "metal_panel_sphere",
                                                                   "metal_panel");
            blacktile_sphere.GetTransformComponent().SetPosition(glm::vec3 {5.0f, 5.5f, 0.0f});

            auto wood_monkey = m_World->AddActor<AStaticMesh>(
                "wood_mmonkey", "Assert/Object/monkey/monkey.obj", "Texture", "monkey_wood", "bamboo-wood-semigloss");
            wood_monkey.GetTransformComponent().SetPosition(glm::vec3 {5.0f, 1.5f, 3.0f});

            auto ground = m_World->AddActor<AStaticMesh>("ground", //
                                                         "Assert/Object/ground/ground.obj",
                                                         "Texture",
                                                         "black-white-tile_sphere",
                                                         "black-white-tile");
            ground.GetTransformComponent().SetPosition(glm::vec3 {0.0f, 0.0f, 0.0f});

            auto teapot = m_World->AddActor<AStaticMesh>("teapot", //
                                                         "Assert/Object/teapot/teapot.obj",
                                                         "Texture",
                                                         "streaky-metal1_teapot",
                                                         "streaky-metal1");
            teapot.GetTransformComponent().SetPosition(glm::vec3 {5.0f, 0.8f, 5.8f});
            teapot.GetTransformComponent().SetScale(glm::vec3 {0.6f, 0.6f, 0.6f});

            auto skeleton = m_World->AddActor<ASkeleton>("Skeleton");
            skeleton.GetTransformComponent().SetPosition(glm::vec3 {1.0f, 0.8f, 3.0f});

            auto light0 = m_World->AddActor<APointLight>("point light_0");
            light0.GetTransformComponent().SetPosition(glm::vec3 {0.0f, 1.5f, 1.5f});

            auto light1 = m_World->AddActor<APointLight>("point light_1");
            light1.GetTransformComponent().SetPosition(glm::vec3 {1.0f, 1.5f, 1.5f});

            auto light2 = m_World->AddActor<APointLight>("point light_2");
            light2.GetTransformComponent().SetPosition(glm::vec3 {0.0f, 2.5f, 1.5f});

            auto light3 = m_World->AddActor<APointLight>("point light_3");
            light3.GetTransformComponent().SetPosition(glm::vec3 {1.0f, 2.5f, 1.5f});

            // auto pointcloud =
            //     m_World->AddActor<APointCloud>("pointcloud", "Assert/Object/longdress/longdress_vox10_1300.ply");
            // pointcloud.GetTransformComponent().SetPosition(glm::vec3 {0.0f, 0.0f, 0.0f});
            // pointcloud.GetTransformComponent().SetScale(glm::vec3 {0.01f, 0.01f, 0.01f});
        }

        void OnAttach() override {}

        void OnDetach() override {}

        void OnEvent(Event& event) override { m_Camera->OnEvent(event); }

        void TickLogic() override { m_World->TickLogic(m_LayerUpdateMeta.m_timeStep, m_LayerUpdateMeta.m_nowTime); }

        void TickRender() override { m_World->TickRender(m_LayerUpdateMeta.m_timeStep); }

        void TickGui() override
        {
            m_World->TickGui(m_LayerUpdateMeta.m_timeStep);

            Gui::Begin("Application");
            Gui::Text("Time Step: {0}", m_LayerUpdateMeta.m_timeStep);
            Gui::Text("Now Time: {0}", m_LayerUpdateMeta.m_nowTime);

            static std::vector<float> arr(600, 0.0);
            arr.push_back(m_LayerUpdateMeta.m_timeStep);
            arr.erase(arr.begin());
            ImGui::PlotLines("Frame Times", &arr[0], 600);
            Gui::End();

            Gui::ShowImNodesDemoWindow();
            ImGui::ShowDemoWindow();

            bool is_color_focused = false;
            Gui::ShowViewport("ViewPort :: Color", m_World->m_FrameRenderBuffer, true, is_color_focused);

            bool is_buffers_focused = false;
            Gui::ShowViewport("ViewPort :: Buffers",
                              m_World->m_FrameRenderBuffer_bufferViewport,
                              true,
                              is_buffers_focused);

            bool is_playground_focused = false;
            Gui::ShowViewport(
                "ViewPort :: Playground", m_World->m_FrameRenderBuffer_playground, false, is_playground_focused);

            m_Camera->m_IsWindowFocused = is_color_focused | is_buffers_focused | is_playground_focused;

            ImGui::ShowExampleAppLog(NULL);
        }

    private:
        Ref<Camera> m_Camera;
        Ref<UWorld> m_World;
    };

    class EngineEditor : public Application
    {
    public:
        EngineEditor(const std::string& basedir, const std::string& filepath) :
            Application("EngineEditor", 2700, 1500, basedir)
        {
            Log::Info("EngineEditor Initialization.");
            PushLayer(std::make_shared<DockSpaceLayer>(m_Running));
            PushLayer(std::make_shared<EditorLayer>());
            Log::Trace("EngineEditor Initialization Success.");
        }

        ~EngineEditor() { Log::Trace("EngineEditor Shutdown."); }
    };

    Scope<Application> CreateApplication(int argc, char** argv)
    {
        std::string basedir = argv[0];
        basedir             = basedir.substr(0, basedir.find_last_of("\\/"));

        std::string filepath;
        if (argc >= 2)
        {
            filepath = argv[1];
        }

        return std::make_unique<EngineEditor>(basedir, filepath);
    }
} // namespace Engine
