#define ENGINE_USE_ENTRYPOINT
#include <Engine/Runtime/Engine.h>

#include <ImGuizmo.h>
#include <nfd.h>

namespace Engine
{
    class EditorLayer : public Layer
    {
    public:
        EditorLayer(bool& app_open_ref) : Layer("EditorLayer"), m_app_open(app_open_ref)
        {
            m_World = std::make_shared<UWorld>();

            auto& camera_viewport = std::make_shared<PerspectiveCamera>(60.0f, 1.778f, 0.1f, 800.0f);
            auto& camera_s        = m_World->AddActor<ACamera>("viewport camera", camera_viewport);
            camera_s.GetTransformComponent().SetPosition(glm::vec3 {0.339f, 3.711f, 8.815f});
            camera_s.GetTransformComponent().SetRotation(glm::vec3 {-0.088f, -6.732f, 0.000f});
            camera_s.GetTransformComponent().SetScale(glm::vec3 {1.000f, 1.000f, 1.000f});
            camera_s.SetIsControlled(true);
            camera_s.SetIsViewportCamera(true);

            auto& camera_aa = std::make_shared<PerspectiveCamera>(60.0f, 1.778f, 0.1f, 800.0f);
            auto& camera_a  = m_World->AddActor<ACamera>("camera_aa", camera_aa);
            camera_a.GetTransformComponent().SetPosition(glm::vec3 {3.570f, 2.371f, 2.175f});
            camera_a.GetTransformComponent().SetRotation(glm::vec3 {-0.115f, 2.404f, 0.000f});
            camera_a.GetTransformComponent().SetScale(glm::vec3 {1.000f, 1.000f, 1.000f});
            camera_a.SetIsControlled(false);
            camera_a.SetIsViewportCamera(false);

            m_World->m_ControlledActor = static_cast<AActor*>(camera_s.GetCameraComponent().GetOwner());
            m_World->m_MainCamera      = &camera_s.GetCameraComponent();
            m_Camera                   = camera_viewport;

            auto skybox =
                m_World->AddActor<ASkybox>("skybox", "Assets/Editor/Skybox/TheSkyIsOnFire/the_sky_is_on_fire_8k.hdr");

            m_World->AddActor<AActor>("Actor");

            auto apawn = m_World->AddActor<APawn>("pawn",
                                                  "Assets/Editor/Animation/lafan/skeleton.ozz",
                                                  "Assets/Editor/Animation/lafan/animcraft_anim1.ozz",
                                                  "Assets/Editor/Animation/lafan/lafan.ozz");
            apawn.GetTransformComponent().SetPosition(glm::vec3 {1.0f, 0.15f, 3.3f});

            // auto red_triangle = m_World->AddActor<AStaticMesh>("red_triangle", //
            //                                                    "Assets/Editor/Object/triangle/triangle.obj",
            //                                                    "TriangleShader",
            //                                                    "basic_red_triangle",
            //                                                    "basic_red_triangle");
            // red_triangle.GetTransformComponent().SetPosition(glm::vec3 {1.0f, 5.5f, 0.0f});

            auto red_sphere = m_World->AddActor<AStaticMesh>("red_sphere", //
                                                             "Assets/Editor/Object/sphere/sphere.obj",
                                                             "BasicPbr",
                                                             "basic_red_sphere",
                                                             "basic_red_sphere");
            red_sphere.GetTransformComponent().SetPosition(glm::vec3 {1.0f, 1.5f, 0.0f});
            MBasicPbr* red_sphere_material = static_cast<MBasicPbr*>(red_sphere.GetMaterial());
            red_sphere_material->SetAlbedo(glm::vec3 {1.0f, 0.0f, 0.0f});
            red_sphere_material->SetMetallic(0.3f);
            red_sphere_material->SetRoughness(0.5f);

            auto brown_sphere = m_World->AddActor<AStaticMesh>("brown_sphere", //
                                                               "Assets/Editor/Object/sphere/sphere.obj",
                                                               "BasicPbr",
                                                               "basic_brown_sphere",
                                                               "basic_brown_sphere");
            brown_sphere.GetTransformComponent().SetPosition(glm::vec3 {3.0f, 1.5f, 0.0f});
            MBasicPbr* brown_sphere_material = static_cast<MBasicPbr*>(brown_sphere.GetMaterial());
            brown_sphere_material->SetAlbedo(glm::vec3 {0.77f, 0.37f, 0.12f});
            brown_sphere_material->SetMetallic(0.2f);
            brown_sphere_material->SetRoughness(0.8f);

            auto aqua_shpere = m_World->AddActor<AStaticMesh>("aqua_sphere", //
                                                              "Assets/Editor/Object/sphere/sphere.obj",
                                                              "BasicPbr",
                                                              "basic_aqua_sphere",
                                                              "basic_aqua_sphere");
            aqua_shpere.GetTransformComponent().SetPosition(glm::vec3 {5.0f, 1.5f, 0.0f});
            MBasicPbr* aqua_shpere_material = static_cast<MBasicPbr*>(aqua_shpere.GetMaterial());
            aqua_shpere_material->SetAlbedo(glm::vec3 {0.0f, 1.0f, 1.0f});
            aqua_shpere_material->SetMetallic(0.9f);
            aqua_shpere_material->SetRoughness(0.3f);

            auto wood_sphere = m_World->AddActor<AStaticMesh>("wood_sphere", //
                                                              "Assets/Editor/Object/sphere/sphere.obj",
                                                              "Texture",
                                                              "bamboo-wood-semigloss_sphere",
                                                              "bamboo-wood-semigloss");
            wood_sphere.GetTransformComponent().SetPosition(glm::vec3 {1.0f, 3.5f, 0.0f});

            auto metal_sphere = m_World->AddActor<AStaticMesh>("metal_sphere", //
                                                               "Assets/Editor/Object/sphere/sphere.obj",
                                                               "Texture",
                                                               "streaky-metal1_sphere",
                                                               "streaky-metal1");
            metal_sphere.GetTransformComponent().SetPosition(glm::vec3 {3.0f, 3.5f, 0.0f});

            auto plastic_sphere = m_World->AddActor<AStaticMesh>("plastic_sphere", //
                                                                 "Assets/Editor/Object/sphere/sphere.obj",
                                                                 "Texture",
                                                                 "scuffed-plastic_sphere",
                                                                 "scuffed-plastic");
            plastic_sphere.GetTransformComponent().SetPosition(glm::vec3 {5.0f, 3.5f, 0.0f});

            auto basetile_sphere = m_World->AddActor<AStaticMesh>("basetile_sphere", //
                                                                  "Assets/Editor/Object/sphere/sphere.obj",
                                                                  "Texture",
                                                                  "base-white-tile_sphere",
                                                                  "base-white-tile");
            basetile_sphere.GetTransformComponent().SetPosition(glm::vec3 {3.0f, 5.5f, 0.0f});

            auto blacktile_sphere = m_World->AddActor<AStaticMesh>("blacktile_sphere", //
                                                                   "Assets/Editor/Object/sphere/sphere.obj",
                                                                   "Texture",
                                                                   "metal_panel_sphere",
                                                                   "metal_panel");
            blacktile_sphere.GetTransformComponent().SetPosition(glm::vec3 {5.0f, 5.5f, 0.0f});

            auto wood_monkey = m_World->AddActor<AStaticMesh>("wood_mmonkey",
                                                              "Assets/Editor/Object/monkey/monkey.obj",
                                                              "Texture",
                                                              "monkey_wood",
                                                              "bamboo-wood-semigloss");
            wood_monkey.GetTransformComponent().SetPosition(glm::vec3 {5.0f, 1.5f, 3.0f});

            auto ground = m_World->AddActor<AStaticMesh>("ground", //
                                                         "Assets/Editor/Object/ground/ground.obj",
                                                         "Texture",
                                                         "black-white-tile_sphere",
                                                         "black-white-tile");
            ground.GetTransformComponent().SetPosition(glm::vec3 {0.0f, 0.0f, 0.0f});

            auto teapot = m_World->AddActor<AStaticMesh>("teapot", //
                                                         "Assets/Editor/Object/teapot/teapot.obj",
                                                         "Texture",
                                                         "streaky-metal1_teapot",
                                                         "streaky-metal1");
            teapot.GetTransformComponent().SetPosition(glm::vec3 {5.0f, 0.8f, 5.8f});
            teapot.GetTransformComponent().SetScale(glm::vec3 {0.6f, 0.6f, 0.6f});

            // auto skeleton = m_World->AddActor<ASkeleton>("Skeleton");
            // skeleton.GetTransformComponent().SetPosition(glm::vec3 {3.0f, 0.8f, 3.3f});

            // auto skinned_mesh = m_World->AddActor<ASkinnedMesh>("SkinnedMesh",
            //                                                     "Assets/Editor/Animation/ruby_skeleton.ozz",
            //                                                     "Assets/Editor/Animation/ruby_animation.ozz",
            //                                                     "Assets/Editor/Animation/ruby_mesh.ozz");
            // skinned_mesh.GetTransformComponent().SetPosition(glm::vec3 {1.0f, 0.8f, 3.3f});

            auto light0 = m_World->AddActor<APointLight>("point light_0");
            light0.GetTransformComponent().SetPosition(glm::vec3 {0.0f, 1.5f, 1.5f});

            auto light1 = m_World->AddActor<APointLight>("point light_1");
            light1.GetTransformComponent().SetPosition(glm::vec3 {1.0f, 1.5f, 1.5f});

            auto light2 = m_World->AddActor<APointLight>("point light_2");
            light2.GetTransformComponent().SetPosition(glm::vec3 {0.0f, 2.5f, 1.5f});

            auto light3 = m_World->AddActor<APointLight>("point light_3");
            light3.GetTransformComponent().SetPosition(glm::vec3 {1.0f, 2.5f, 1.5f});

            // auto pointcloud =
            //     m_World->AddActor<APointCloud>("pointcloud",
            //     "Assets/Editor/Object/longdress/longdress_vox10_1300.ply");
            // pointcloud.GetTransformComponent().SetPosition(glm::vec3 {0.0f, 0.0f, 0.0f});
            // pointcloud.GetTransformComponent().SetScale(glm::vec3 {0.01f, 0.01f, 0.01f});

            m_Callbacks["Open"] = std::function<void()>([&]() {
                std::string filepath = OpenFile();
                std::string suffix   = filepath.substr(filepath.find_last_of(".") + 1);

                if (suffix == "ply")
                {
                    auto pointcloud = m_World->AddActor<APointCloud>("pointcloud", filepath);
                    pointcloud.GetTransformComponent().SetPosition(glm::vec3 {0.0f, 0.0f, 0.0f});
                    pointcloud.GetTransformComponent().SetScale(glm::vec3 {0.01f, 0.01f, 0.01f});
                }
            });
        }

        void OnAttach() override {}

        void OnDetach() override {}

        void OnEvent(Event& event) override { m_Camera->OnEvent(event); }

        void TickLogic() override
        {
            m_World->TickLogic(m_LayerUpdateMeta.m_timeStep, m_LayerUpdateMeta.m_nowTime, m_IsWindowFocused);
        }

        void TickRender() override { m_World->TickRender(m_LayerUpdateMeta.m_timeStep); }

        void TickGui() override
        {
            Gui::DockSpace(m_app_open, m_Callbacks);

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

            bool is_color_focused = false;
            Gui::ShowViewport(
                "ViewPort :: Color", m_World->m_FrameRenderBuffer, true, is_color_focused, std::function {[&]() {
                    EditorGui();
                }});

            bool is_buffers_focused = false;
            Gui::ShowViewport(
                "ViewPort :: Buffers", m_World->m_FrameRenderBuffer_bufferViewport, true, is_buffers_focused);

            // bool is_playground_focused = false;
            // Gui::ShowViewport(
            //     "ViewPort :: Playground", m_World->m_FrameRenderBuffer_playground, false, is_playground_focused);

            m_IsWindowFocused = is_color_focused | is_buffers_focused;

            ImGui::ShowExampleAppLog(NULL);
        }

        void EditorGui()
        {
            float camDistance = 8.f;

            static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::TRANSLATE);
            static ImGuizmo::MODE      mCurrentGizmoMode(ImGuizmo::LOCAL);
            static bool                useSnap         = false;
            static float               snap[3]         = {1.f, 1.f, 1.f};
            static float               bounds[]        = {-0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f};
            static float               boundsSnap[]    = {0.1f, 0.1f, 0.1f};
            static bool                boundSizing     = false;
            static bool                boundSizingSnap = false;

            ImGuiIO& io                  = ImGui::GetIO();
            float    viewManipulateRight = io.DisplaySize.x;
            float    viewManipulateTop   = 0;

            ImGuizmo::SetDrawlist();
            float windowWidth  = (float)ImGui::GetWindowWidth();
            float windowHeight = (float)ImGui::GetWindowHeight();
            ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);
            viewManipulateRight = ImGui::GetWindowPos().x + windowWidth;
            viewManipulateTop   = ImGui::GetWindowPos().y;

            m_World->m_MainCamera->GetCamera().SetViewPort(windowWidth, windowHeight);
            m_World->m_MainCamera->GetCamera().RecalculateProjectionMatrix();

            AActor* actor_mainCamera = static_cast<AActor*>(m_World->m_MainCamera->GetOwner());

            glm::mat4 cameraView       = glm::inverse(actor_mainCamera->GetTransformComponent().GetTransform() *
                                                m_World->m_MainCamera->GetTransform());
            glm::mat4 cameraProjection = m_World->m_MainCamera->GetCamera().GetProjectionMatrix();
            glm::mat4 identityMatrix   = glm::mat4(1.0f);

            ImGuizmo::DrawGrid(
                glm::value_ptr(cameraView), glm::value_ptr(cameraProjection), glm::value_ptr(identityMatrix), 8.0f);

            if (m_World->entity_selected != entt::null &&
                m_World->GetRegistry().valid(m_World->entity_selected) == true &&
                m_World->GetRegistry().any_of<UTransformComponent>(m_World->entity_selected) == true)
            {
                auto&     tansformComponent = m_World->GetRegistry().get<UTransformComponent>(m_World->entity_selected);
                glm::mat4 transform         = tansformComponent.GetTransform();

                ImGuizmo::Manipulate(glm::value_ptr(cameraView),
                                     glm::value_ptr(cameraProjection),
                                     mCurrentGizmoOperation,
                                     mCurrentGizmoMode,
                                     glm::value_ptr(transform),
                                     NULL,
                                     useSnap ? &snap[0] : NULL,
                                     boundSizing ? bounds : NULL,
                                     boundSizingSnap ? boundsSnap : NULL);

                if (ImGuizmo::IsUsing())
                {
                    float matrixTranslation[3], matrixRotation[3], matrixScale[3];
                    ImGuizmo::DecomposeMatrixToComponents(
                        glm::value_ptr(transform), matrixTranslation, matrixRotation, matrixScale);

                    tansformComponent.SetPosition(
                        glm::vec3 {matrixTranslation[0], matrixTranslation[1], matrixTranslation[2]});

                    ImGuizmo::RecomposeMatrixFromComponents(
                        matrixTranslation, matrixRotation, matrixScale, glm::value_ptr(transform));
                }
            }

            ImGuizmo::ViewManipulate(glm::value_ptr(cameraView),
                                     camDistance,
                                     ImVec2(viewManipulateRight - 128, viewManipulateTop),
                                     ImVec2(128, 128),
                                     0x10101010);
        }

        std::string OpenFile()
        {
            std::string filepath = "";
            nfdchar_t*  outPath  = NULL;
            nfdresult_t result   = NFD_OpenDialog(NULL, NULL, &outPath);
            if (result == NFD_OKAY)
            {
                filepath = outPath;
                Log::Info("Opening file: {}", filepath);
                free(outPath);
            }
            else if (result == NFD_CANCEL)
            {
                Log::Info("User pressed cancel.");
            }
            else
            {
                Log::Error("Error: {}", NFD_GetError());
            }

            return filepath;
        }

    private:
        Ref<Camera> m_Camera;
        Ref<UWorld> m_World;
        bool&       m_app_open;

    private:
        bool m_IsWindowFocused = false;
    };

    class EngineEditor : public Application
    {
    public:
        EngineEditor(const std::string& basedir, const std::string& filepath) :
            Application("EngineEditor", 2700, 1500, basedir)
        {
            Log::Info("EngineEditor Initialization.");
            PushLayer(std::make_shared<DockSpaceLayer>(m_Running));
            PushLayer(std::make_shared<EditorLayer>(m_Running));
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
